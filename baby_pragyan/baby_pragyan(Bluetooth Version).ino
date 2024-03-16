// Header Declaration
#include <Servo.h>

//Configure Bluetooth 
String Command;

// Command frequency configuration
unsigned long commmandDelay = 1000;
unsigned long rotationalCommandDelay = 5000;

//Wheel configuration
#define enA 11                    //Enable1 L298 Pin enA
#define rightMotorForwordPin 10   //Motor1  L298 Pin rightMotorForwordPin
#define rightMotorBackwardPin 9   //Motor1  L298 Pin rightMotorForwordPin
#define leftMotorBackwardPin 8    //Motor2  L298 Pin rightMotorForwordPin
#define leftMotorForwordPin 7     //Motor2  L298 Pin rightMotorForwordPin
#define enB 6                     //Enable2 L298 Pin enB

// Ultrasonic Configuration
#define echo A0     //Echo pin
#define trigger A1  //Trigger pin

// Servo configuration
#define objServo A2
#define spectServo A4
#define solarServo A5

// LED Configuration 
#define spectLED 5
#define laserLight 12

// Movement Configuration
int distance_L, distance_F = 30, distance_R;
long distance;
int safetyDistance  = 20;  // Safty distance to be followed

int BT_data = -1;
int Speed = 255;
int mode = 0;

void setup() {  // put your setup code here, to run once
  //Configures the Ultrasonic Sensor of objServo
  pinMode(echo, INPUT);      // declare ultrasonic sensor Echo pin as input
  pinMode(trigger, OUTPUT);  // declare ultrasonic sensor Trigger pin as Output

  //Configure the Modor Driver L298N
  pinMode(enA, OUTPUT);                    // declare as output for L298 Pin enA
  pinMode(rightMotorForwordPin, OUTPUT);   // declare as output for L298 Pin rightMotorForwordPin
  pinMode(rightMotorBackwardPin, OUTPUT);  // declare as output for L298 Pin rightMotorBackwardPin
  pinMode(leftMotorBackwardPin, OUTPUT);   // declare as output for L298 Pin leftMotorBackwardPin
  pinMode(leftMotorForwordPin, OUTPUT);    // declare as output for L298 Pin leftMotorForwordPin
  pinMode(enB, OUTPUT);                    // declare as output for L298 Pin enB

  Serial.begin(9600);  // start serial communication at 9600bps

  // Configure the Servo motor
  pinMode(objServo, OUTPUT);
  pinMode(spectServo, OUTPUT);
  pinMode(solarServo, OUTPUT);

  // LED Configuration 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(spectLED, OUTPUT);
  pinMode(laserLight, OUTPUT);

  // Check spectLED
  for (int angle = 50; angle <= 150; angle += 3) {
    delay(10);
    servoPulse(spectServo, angle);
  }
  digitalWrite(spectLED, LOW);
  delay(1000);
  digitalWrite(spectLED, HIGH);
  for (int angle = 150; angle >= 50; angle -= 3) {
    servoPulse(spectServo, angle);
  }

  // Check objServo
  for (int angle = 80; angle <= 185; angle += 5) {
    servoPulse(objServo, angle);
  }
  for (int angle = 185; angle >= 10; angle -= 5) {
    servoPulse(objServo, angle);
  }
  for (int angle = 10; angle <= 80; angle += 5) {
    servoPulse(objServo, angle);
  }
  delay(500);
}

void loop() {
  if (Serial.available()){
    Command = Serial.readString();
    Serial.print(Command); 
    BT_data = BTremote_data();
  }

  analogWrite(enA, Speed);  // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed
  analogWrite(enB, Speed);  // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed

  if (mode == 0) {
    //===============================================================================
    //                          Manual Control Command
    //===============================================================================
    if (BT_data == 1) {
      // if the BT_data is '1' the DC motor will go forward
      forword();
      delay(commmandDelay);
      BT_data = 5; 
    } else if (BT_data == 2) {
      // if the BT_data is '2' the motor will Reverse
      backword();
      delay(commmandDelay);
      BT_data = 5;
    } else if (BT_data == 3) {
      // if the BT_data is '3' the motor will turn left
      turnLeft();
      delay(commmandDelay);
      BT_data = 5;
    } else if (BT_data == 4) {
      // if the BT_data is '4' the motor will turn right
      turnRight();
      delay(commmandDelay);
      BT_data = 5;
    } else if (BT_data == 5) { 
      // if the BT_data '5' the motor will Stop
      Stop(); 
      BT_data = -1;
    } else if (BT_data == 6) {
      // Manual Control Command
      BT_data = -1;
      Stop();
      mode = 0;
    } else if (BT_data == 7) {
      // Autonomous Mode Command
      BT_data = -1;
      mode = 1;
    } else if (BT_data == 8) {
      // Deploy the Solar Panels 
      BT_data = -1;
      solarDeploy();
    } else if (BT_data == 9) {
      // Contract the Solar Panel
      BT_data = -1;
      solarContract();
    } else if (BT_data == 10) {
      // Deploy the Spectrometer 
      BT_data = -1;
      spectDeploy();
    } else if (BT_data == 11) {
      // Contract the Spectrometer 
      BT_data = -1;
      spectContract();
    } else if (BT_data == 12) {
      // Deploy the Laser Spectrometer 
      BT_data = -1;
      laserBeamOn();
    } else if (BT_data == 13) {
      // Contract the Laser Spectrometer 
      BT_data = -1;
      laserBeamOff();
    }
  }

  if (mode == 1) {
    //===============================================================================
    //                          Autonomous Control Mode
    //===============================================================================
    distance_F = Ultrasonic_read();
    Serial.println("Next obstacle is in " + String(distance_F) + "cm ");
    if (BT_data == 5) {
      // Stop the rover if IR command to stop is received
      Stop();
      mode = 0;
      BT_data = -1;
    } else if (BT_data == 6) {
      // Switch to Manual Mode if IR command received
      mode = 0;
      BT_data = -1;
    } else if (distance_F > safetyDistance) {
      forword();
    } else {
      Check_side();
    }
  }
  delay(10);
}

long BTremote_data() {
  if (Command == "F") { BT_data = 1; }                  // Forward Button
  else if (Command == "B") { BT_data = 2; }             // Backward Button
  else if (Command == "L") { BT_data = 3; }             // Left Button
  else if (Command == "R") { BT_data = 4; }             // Right Button
  else if (Command == "S") { BT_data = 5; }             // Stop Button
  else if (Command == "Manuel") { BT_data = 6; }        // Manual mode
  else if (Command == "Autonomous") { BT_data = 7; }    // Autonomous mode
  else if (Command == "SD") { BT_data = 8; }            // Solar Deploy
  else if (Command == "SC") { BT_data = 9; }            // Solar Contracts 
  else if (Command == "AD") { BT_data = 10; }           // APXS Deploy 
  else if (Command == "AC") { BT_data = 11; }           // APXS Contracts
  else if (Command == "LD") { BT_data = 12; }           // LIBS Deploy 
  else if (Command == "LC") { BT_data = 13; }           // LIBS Contracts
  return BT_data;
}

void servoPulse(int pin, int angle) {
  // Calculate the pulse width in microseconds based on the desired angle.
  int pwm = (angle * 11) + 500;

  // safetyDistance the specified GPIO pin to HIGH to start the pulse to the servo motor.
  digitalWrite(pin, HIGH);

  // Delay for the calculated pulse width in microseconds, controlling the servo's position.
  delayMicroseconds(pwm);

  // safetyDistance  the GPIO pin to LOW, indicating the end of the pulse.
  digitalWrite(pin, LOW);

  // Add a delay for 50 milliseconds to allow the servo to settle in its new position.
  delay(50);
}

// Function to measure distance using an ultrasonic sensor
long Ultrasonic_read() {
  Serial.println("# >> Reading Ultrasonic........");
  // Step 1: Set the trigger pin to LOW
  digitalWrite(trigger, LOW);
  // Small delay to ensure the sensor is in a stable state
  delayMicroseconds(2);

  // Step 2: Set the trigger pin to HIGH to send an ultrasonic pulse
  digitalWrite(trigger, HIGH);
  // Delay for a specific duration to generate an ultrasonic pulse
  delayMicroseconds(10);

  // Step 3: Measure the duration of the echo pulse
  // The pulseIn function measures the time in microseconds for the echo pin to go HIGH
  // This duration represents the time it took for the ultrasonic pulse to travel to the object and back
  distance = pulseIn(echo, HIGH);

  // Step 4: Calculate the distance in centimeters
  // Divide the measured pulse duration by a constant (29) and then divide by 2 to account for the round trip
  // This formula converts the time into distance
  return distance / 29 / 2;
}

void compareDistance() {
  if (distance_L > distance_R) {
    // Rover Turns left if the distaste of left side is grater than right side.
    turnLeft();
    delay(rotationalCommandDelay);
  } else if (distance_R > distance_L) {
    // Rover Turns right if the distaste of right side is grater than left side.
    turnRight();
    delay(rotationalCommandDelay);
  } else {
    // Take reverse then turns right if both side obstacles
    backword();
    delay(3000);
    turnRight();
    delay(rotationalCommandDelay);
  }
}

void Check_side() {
  Serial.println("# >> Rover measuring both side distance.");
  Stop();
  delay(100);
  for (int angle = 80; angle <= 185; angle += 5) {
    // Ultrasonic sensor rotated right side from centre
    servoPulse(objServo, angle);
  }
  delay(600);
  distance_R = Ultrasonic_read();  // Assign the value of distance to varible
  delay(500);

  for (int angle = 185; angle >= 10; angle -= 5) {
    // Ultrasonic sensor rotated left from the left side
    servoPulse(objServo, angle);
  }
  delay(500);
  distance_L = Ultrasonic_read();  // Assign the value of distance to varible
  delay(500);

  for (int angle = 10; angle <= 80; angle += 5) {
    // Ultrasonic rotated centered from the right side
    servoPulse(objServo, angle);
  }
  delay(500);

  compareDistance();
}

void forword() {
  //forword Motion
  Serial.println("# >> Rover Moving Forward.");
  digitalWrite(rightMotorForwordPin, HIGH);  // Right Motor forword Pin
  digitalWrite(rightMotorBackwardPin, LOW);  // Right Motor backword Pin
  digitalWrite(leftMotorBackwardPin, LOW);   // Left Motor backword Pin
  digitalWrite(leftMotorForwordPin, HIGH);   // Left Motor forword Pin
}

void backword() {
  //backword Motion
  Serial.println("# >> Rover Taking Reverse.");
  digitalWrite(rightMotorForwordPin, LOW);    // Right Motor forword Pin
  digitalWrite(rightMotorBackwardPin, HIGH);  // Right Motor backword Pin
  digitalWrite(leftMotorBackwardPin, HIGH);   // Left Motor backword Pin
  digitalWrite(leftMotorForwordPin, LOW);     // Left Motor forword Pin
}

void turnRight() {
  //turnLeft 
  Serial.println("# >> Rover turning left.");
  digitalWrite(rightMotorForwordPin, HIGH);  // Right Motor forword Pin
  digitalWrite(rightMotorBackwardPin, LOW);  // Right Motor backword Pin
  digitalWrite(leftMotorBackwardPin, HIGH);  // Left Motor backword Pin
  digitalWrite(leftMotorForwordPin, LOW);    // Left Motor forword Pin
}

void turnLeft() {
  //turnRight
  Serial.println("# >> Rover turning right");
  digitalWrite(rightMotorForwordPin, LOW);    // Right Motor forword Pin
  digitalWrite(rightMotorBackwardPin, HIGH);  // Right Motor backword Pin
  digitalWrite(leftMotorBackwardPin, LOW);    // Left Motor backword Pin
  digitalWrite(leftMotorForwordPin, HIGH);    // Left Motor forword Pin
}

void Stop() {
  //stop the motion
  Serial.println("# >> Rover stopped.");
  digitalWrite(rightMotorForwordPin, LOW);   // Right Motor forword Pin
  digitalWrite(rightMotorBackwardPin, LOW);  // Right Motor backword Pin
  digitalWrite(leftMotorBackwardPin, LOW);   // Left Motor backword Pin
  digitalWrite(leftMotorForwordPin, LOW);    // Left Motor forword Pin
  BT_data = -1;
}

void spectDeploy(){
  for (int angle = 65; angle <= 140; angle += 1) {
    servoPulse(spectServo, angle);
  }
  digitalWrite(spectLED, LOW);
}

void spectContract(){
  digitalWrite(spectLED, HIGH);
  for (int angle = 140; angle >= 65; angle -= 1) {
    servoPulse(spectServo, angle);
  }
}

void solarDeploy(){
  for (int angle = 80; angle >= 10; angle -= 5) {
    servoPulse(solarServo, angle);
  }
}

void solarContract(){
  for (int angle = 10; angle <= 80; angle += 3) {
    servoPulse(solarServo, angle);
  }
}

void laserBeamOn(){
  digitalWrite(laserLight, LOW);
}

void laserBeamOff(){
  digitalWrite(laserLight, HIGH);
}
