# Arduino Rover Control System

## Overview
This Arduino-based rover control system allows for manual and autonomous control of a rover using an infrared (IR) remote. The rover is equipped with features like obstacle avoidance using ultrasonic sensors, manual movement controls, and deployment mechanisms for a spectrometer, solar panel, and laser beam.

## Features
- Manual control via IR remote (forward, backward, left, right, stop)
- Autonomous mode with obstacle avoidance
- Ultrasonic sensors for measuring distances
- Servo motor-controlled deployment of a spectrometer, solar panel, and laser beam
- Motor control using the L298N motor driver

## Hardware Requirements
- Arduino board
- Infrared (IR) receiver
- Ultrasonic sensor
- L298N motor driver
- Servo motors for various deployments
- LEDs for status indication

## Setup
1. Connect the hardware components according to the specified pin configurations.
2. Upload the provided Arduino code to the Arduino board.
3. Power on the rover and use the IR remote to control its movements or switch to autonomous mode.

## Usage
- Use the IR remote to control the rover manually (forward, backward, left, right, stop).
- Switch to autonomous mode to enable obstacle avoidance using ultrasonic sensors.
- Deploy the spectrometer, solar panel, and laser beam using specific IR commands.

## Configuration
Adjust the configuration parameters in the code as needed:
- IR debounce delay
- Command frequency and rotational command delay
- Motor and servo pin configurations
- Ultrasonic sensor pin configurations
- Safety distances and deployment states

## Author
[Shreyas KB](https://www.linkedin.com/in/shreyaskb333/)

## License
This project is licensed under the [CC License](LICENSE).
