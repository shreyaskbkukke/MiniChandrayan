# Mini Chandrayan Project

This repository combines three distinct control systems implemented using Arduino and ESP32 microcontrollers for different robotic and IoT applications. Below are brief overviews of each system:

## 1. Arduino Rover Control System

### Overview
The Arduino Rover Control System enables manual and autonomous control of a rover equipped with infrared (IR) remote communication, obstacle avoidance using ultrasonic sensors, and various deployment mechanisms. It utilizes an Arduino board, IR receiver, servo motors, and motor drivers for precise control.

![DSC_2092](https://github.com/shreyaskbkukke/mini_chandrayan/assets/96857515/c480f3a2-5f66-47ee-8e68-4c316ae8aba3)

![DSC_2095](https://github.com/shreyaskbkukke/mini_chandrayan/assets/96857515/84a739e4-5fb6-405c-9062-91e968926995)

![DSC_2097](https://github.com/shreyaskbkukke/mini_chandrayan/assets/96857515/3e93d356-5716-4796-aa98-b90f4cad4cc8)

![DSC_2099](https://github.com/shreyaskbkukke/mini_chandrayan/assets/96857515/c67dcdb3-94dd-49ef-bb72-685921124761)

![DSC_2100](https://github.com/shreyaskbkukke/mini_chandrayan/assets/96857515/13fd8ad4-99e7-49ae-a049-5013c99d2960)

### Features
- Manual control via IR remote
- Autonomous mode with obstacle avoidance
- Servo motor-controlled deployment of spectrometer, solar panel, and laser beam
- Motor control using the L298N motor driver

### Hardware Requirements
- Arduino board
- IR receiver
- Ultrasonic sensor
- L298N motor driver
- Servo motors
- LEDs for status indication

### Setup
1. Connect the hardware components according to the specified pin configurations.
2. Upload the provided Arduino code to the Arduino board.
3. Power on the rover and use the IR remote to control its movements or switch to autonomous mode.

### Configuration
Adjust the configuration parameters in the code as needed.

### Author
[Shreyas KB](https://www.linkedin.com/in/shreyaskb333/)

### License
This project is licensed under the [CC License](LICENSE).

## 2. Rocket Launch Control System

### Overview
The Rocket Launch Control System facilitates the control of a rocket launch sequence using buttons and LEDs. It includes features such as start, reset, and control of additional components like a fan and LED. Implemented with Arduino, this system offers a simple interface for rocket launch experiments.

### Features
- Start and reset buttons for launch control
- Fan and LED control for additional effects
- User-friendly interface for easy operation

### Hardware Requirements
- Arduino board
- Buttons for start and reset
- LEDs for status indication
- Fan for additional effects

### Usage
Press the start button to initiate the launch sequence. The reset button can be used to reset the system after a launch.

### Author
[Your Name](https://www.linkedin.com/in/your-linkedin-profile/)

### License
This project is licensed under the [CC License](LICENSE).

## 3. ESP32 Camera Control System

### Overview
The ESP32 Camera Control System allows remote control of a camera using an ESP32 microcontroller. It features pan and tilt control, adjustable lighting, and real-time camera feed streaming to a web interface. The system employs WebSocket communication for responsive servo control.

### Features
- Pan and tilt control for camera positioning
- Real-time camera feed streaming
- Adjustable lighting control
- WebSocket communication for servo control

### Hardware Components
- ESP32 microcontroller
- Camera module with GPIO connections
- Servo motors for pan and tilt control
- LED for adjustable lighting

### Setup
1. Connect the hardware components according to the specified pin configurations.
2. Upload the provided Arduino code to the ESP32 board.
3. Connect to the ESP32's Wi-Fi network and access the camera control interface via a web browser.

### Author
[Shreyas KB](https://www.linkedin.com/in/shreyaskb333/)

### License
This project is licensed under the [CC License](LICENSE).
  
