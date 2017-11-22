#!/bin/sh

# Raspberry Pi specific script

# Turn off pull-up resistor on pin 4
gpio -g 4 mode tri

# Set UART baud rate to 2400
stty -F /dev/serial0 2400

