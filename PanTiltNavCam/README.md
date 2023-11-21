# ESP32 Camera Control System

## Overview
This project utilizes an ESP32 microcontroller to control a camera remotely. The system includes features like pan and tilt control for the camera, as well as adjustable lighting. The camera feed is streamed in real-time to a web interface, allowing for remote monitoring and control.

## Features
- Pan and tilt control for adjusting the camera's view
- Real-time camera feed streaming
- Adjustable lighting control
- WebSocket communication for servo control

## Hardware Components
- ESP32 microcontroller
- Camera module with appropriate GPIO connections
- Servo motors for pan and tilt control
- LED for adjustable lighting

## Pin Configuration
- Pan Servo Pin: 14
- Tilt Servo Pin: 15
- Light PWM Pin: 4
- Camera GPIO Pins: (Refer to the code comments for specific pin assignments)

## Setup
1. Connect the hardware components according to the specified pin configurations.
2. Upload the provided Arduino code to the ESP32 board.
3. Connect to the ESP32's Wi-Fi network (SSID: baby_pragyan, Password: 9108243172).
4. Open a web browser and navigate to the ESP32's IP address to access the camera control interface.

## Web Interface
- The web interface allows you to view the camera feed in real-time.
- Use the sliders to control pan, tilt, and lighting.
- WebSocket communication ensures responsive and real-time servo control.

## Author
[Shreyas KB](https://www.linkedin.com/in/shreyaskb333/)

## License
This project is licensed under the [CC License](LICENSE).
