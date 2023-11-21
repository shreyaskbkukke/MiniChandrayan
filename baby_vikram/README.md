# Arduino Rover Control System

## Overview
This Arduino-based rover control system allows for manual and autonomous control of a rover using an infrared (IR) remote. The rover is equipped with features like obstacle avoidance using ultrasonic sensors, manual movement controls, and deployment mechanisms for a spectrometer, solar panel, and laser beam.

## Features
- Manual control via IR remote (forward, backward, left, right, stop)
- Autonomous mode with obstacle avoidance
- Servo motor-controlled deployment of a spectrometer, solar panel, and laser beam
- Motor control using the L298N motor driver
- Status indication with LEDs

## Hardware Requirements
- Arduino board
- Infrared (IR) receiver
- Ultrasonic sensor
- L298N motor driver
- Servo motors for various deployments
- LEDs for status indication

## Pin Configuration
- IR Receiver: A5
- Servo 1 Control Pin: A4
- Servo 2 Control Pin: A3
- Red LED Pin: 5
- Green LED Pin: 4
- Motor Driver Configuration:
  - ENA: 11
  - IN1: 10
  - IN2: 9
  - IN3: 8
  - IN4: 7
  - ENB: 6

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
- Command duration for motors
- Motor and servo pin configurations

## Author
[ Shreyas KB](https://your.linkedin.profile)

## License
This project is licensed under the [CC License](LICENSE).
