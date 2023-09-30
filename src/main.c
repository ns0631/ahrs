#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Datasheet can be found here: https://images-na.ssl-images-amazon.com/images/I/B11fVGszLsS.pdf

/* The devicetree node identifier for the ahrs */
#define I2C0_NODE DT_NODELABEL(ahrs)

//Start byte for roll register
#define angleregister 0x3d
#define angvelregister 0x37

static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);

uint8_t attitude_reading[6];
uint8_t angvel_reading[6];


//Reads and outputs attitude in degrees
bool read_angle(float angle[]){
	//Burst read = read all registers at once
	int ret = i2c_burst_read_dt(&dev_i2c, angleregister, attitude_reading, sizeof(attitude_reading));
	if(ret != 0){
		return false;
	}

	//Lowbyte + highbyte conversion
	//Roll
	angle[0] = ( (float) ( attitude_reading[1] * 256 + attitude_reading[0] ) ) / 32768. * 180.;

	//Pitch
	angle[1] = ( (float) ( attitude_reading[3] * 256 + attitude_reading[2] ) )  / 32768. * 180.;

	//Yaw
	angle[2] = ( (float) ( attitude_reading[5] * 256 + attitude_reading[4] ) )  / 32768. * 180.;

	return true;
}

//Reads & outputs angular velocity in degrees/sec
//Returns weird data (~4000) when turning counterclockwise
bool read_angvel(float angvel[]){
	//Burst read = read all registers at once
	int ret = i2c_burst_read_dt(&dev_i2c, angvelregister, angvel_reading, sizeof(angvel_reading));
	if(ret != 0){
		return false;
	}

	//Lowbyte + highbyte conversion
	//Velocity x
	angvel[0] = ( (float) ( angvel_reading[1] * 256 + angvel_reading[0] ) ) / 32768. * 2000.;

	//Velocity y
	angvel[1] = ( (float) ( angvel_reading[3] * 256 + angvel_reading[2] ) )  / 32768. * 2000.;

	//Velocity z
	angvel[2] = ( (float) ( angvel_reading[5] * 256 + angvel_reading[4] ) )  / 32768. * 2000.;

	return true;
}

int main(void)
{

	//Configure i2c bus
	int ret;
	if (!device_is_ready(dev_i2c.bus)) {
		printk("I2C bus %s is not ready!\n\r",dev_i2c.bus->name);
		return 1;
	}

	float angle[3];
	float angvel[3];

	//Read measurements continuously
	while (1) {		
		if( read_angle(&angle) && read_angvel(&angvel)){
			printk("Yaw: %f; Pitch: %f; Roll: %f\n", angle[2], angle[1], angle[0]);
			printk("Angular velocity: X: %f; Y: %f; Z: %f\n", angvel[0], angvel[1], angvel[2]);
		}

		//Sleep for 0.1 seconds before repeating
		k_msleep(100);
	}

	return 0;
}
