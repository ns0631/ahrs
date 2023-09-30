.. _blinky-sample:

Reading WT901 AHRS data over i2c
######

Overview
********

This repository contains the Zephyr RTOS code to read angular velocity and angle from a WT901 AHRS.
The data is read over the i2c bus, in contrast to code in Nautical/Maritime, which reads data over UART.

Requirements
************

The overlay is only configured with the STM32 Nucleo-F446RE board at the moment.
You can add an overlay in ``boards``.

Wire the 3v3 pin to VCC on the Witmotion.
Wire the ground on the nucleo to ground on Witmotion.
Wire the serial clock (SCL) pin on the AHRS to PB_8.
Wire the serial data (SDA) pin on the AHRS to PB_9.

You can change the last two in the overlay, if you'd like.

Building and Running
********************

For a STM32 Nucleo-F446RE, run the following from the parent directory:
``west build -b nucleo_f446re -p auto ahrs``
``west flash``

To see the output, run:
``minicom -b 115200 -D /dev/ttyACM0``

Further Reading
***************

If you'd like to learn more about Zephyr's i2c peripheral, you can check out [this fantastic tutorial](https://academy.nordicsemi.com/courses/nrf-connect-sdk-fundamentals/lessons/lesson-6-serial-com-i2c/).