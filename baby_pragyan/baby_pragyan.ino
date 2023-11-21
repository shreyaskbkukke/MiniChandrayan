// Header Declaration
#include <IRremote.h>
#include <Servo.h>

// Configuring the IR Receiver
const int RECV_PIN = A3;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long lastIRTime = 0;
const unsigned long IRDebounceDelay = 500; // Adjust this value as needed

unsigned long lastReceivedTime = 0;
unsigned long debounceDelay = 1000;  // Adjust this value as needed

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

int IR_data = -1;  // variable to receive data from the serial port and IRremote
int Speed = 255;
int mode = 0;

bool isSpectDiployed = false;  // Track the Spectrometer deployment state
bool isSolarDeployed = false;  // Track the solar panel deployment state
bool isLaserDeployed = false;  // Track the Laser Beam deployment state

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

  //Configure the IR Receiver
  delay(1000);          // Wait for 1 second
  irrecv.enableIRIn();  // Start the receiver
  irrecv.blink13(true);

  Serial.begin(9600);  // start serial communication at 9600bps

  // Configure the Servo motor
  pinMode(objServo, OUTPUT);
  pinMode(spectServo, OUTPUT);
  pinMode(solarServo, OUTPUT);

  // LED Configuration 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(spectLED, OUTPUT);
  pinMode(laserLight, OUTPUT);

  // Serial.println();
  // Serial.println("Rover Initialized.............");
  // for (int i = 0; i < 30; i++) {
  //   Serial.print("#");
  //   delay(200);
  // }
  // Serial.println();

  // Check spectLED
  for (int angle = 65; angle <= 140; angle += 1) {
    delay(10);
    servoPulse(spectServo, angle);
  }
  digitalWrite(spectLED, LOW);
  delay(1000);
  digitalWrite(spectLED, HIGH);
  for (int angle = 140; angle >= 65; angle -= 1) {
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
    if (irrecv.decode(&results)) {
    // Check for debounce
    unsigned long currentTime = millis();
    if (currentTime - lastIRTime >= IRDebounceDelay) {
      lastIRTime = currentTime;
      if (millis() - lastReceivedTime >= debounceDelay) {
        lastReceivedTime = millis();  // Update the last received time
        IR_data = IRremote_data();
        irrecv.resume();  // Receive the next value
        delay(100);  // Add a delay to ignore additional signals
      }
    }
  }

  analogWrite(enA, Speed);  // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed
  analogWrite(enB, Speed);  // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed

  if (mode == 0) {
    //===============================================================================
    //                          Manual Control Command
    //===============================================================================
    if (IR_data == 1) {
      // if the IR_data is '1' the DC motor will go forward
      forword();
      delay(commmandDelay);
      IR_data = 5; 
    } else if (IR_data == 2) {
      // if the IR_data is '2' the motor will Reverse
      backword();
      delay(commmandDelay);
      IR_data = 5;
    } else if (IR_data == 3) {
      // if the IR_data is '3' the motor will turn left
      turnLeft();
      delay(commmandDelay);
      IR_data = 5;
    } else if (IR_data == 4) {
      // if the IR_data is '4' the motor will turn right
      turnRight();
      delay(commmandDelay);
      IR_data = 5;
    } else if (IR_data == 5) { 
      // if the IR_data '5' the motor will Stop
      Stop(); 
      IR_data = -1;
    } else if (IR_data == 6) {
      // Manual IR Remote Control Command
      IR_data = -1;
      Stop();
      Serial.println();
      Serial.println("# >> Rover is on Manual Mode.");
      mode = 0;
    } else if (IR_data == 7) {
      // Autonomous Mode Command
      IR_data = -1;
      Serial.println();
      Serial.println("# >> Rover is on Autonomous Mode.");
      mode = 1;
    } else if (IR_data == 8) {
      // Deploy the Spectometer
      IR_data = -1;
      spectDeploy(isSpectDiployed);
    } else if (IR_data == 9) {
      // Deploy the Solar Panel
      IR_data = -1;
      solarDeploy(isSpectDiployed);
    } else if (IR_data == 10) {
      // Initialize the Laser Spectrometer 
      IR_data = -1;
      laserBeam(isLaserDeployed);
    }
  }

  if (mode == 1) {
    //===============================================================================
    //                          Autonomous Control Mode
    //===============================================================================
    distance_F = Ultrasonic_read();
    Serial.println("Next obstacle is in " + String(distance_F) + "cm ");
    if (IR_data == 5) {
      // Stop the rover if IR command to stop is received
      Stop();
      mode = 0;
      IR_data = -1;
    } else if (IR_data == 6) {
      // Switch to Manual Mode if IR command received
      mode = 0;
      IR_data = -1;
    } else if (distance_F > safetyDistance) {
      forword();
    } else {
      Check_side();
    }
  }
  delay(10);
}

long IRremote_data() {
  if (results.value == 0xC00058) { IR_data = 1; }       // BlueUp Button
  else if (results.value == 0xC00059) { IR_data = 2; }  // BlueDown Button
  else if (results.value == 0xC0005A) { IR_data = 3; }  // BlueLeft Button
  else if (results.value == 0xC0005B) { IR_data = 4; }  // BlueRight Button
  else if (results.value == 0xC0005C) { IR_data = 5; }  // Select Button
  else if (results.value == 0xC000CC) { IR_data = 6; }  // Blue "Guide Button"
  else if (results.value == 0xC0007D) { IR_data = 7; }  // Blue "On Demand"
  else if (results.value == 0xC0006E) { IR_data = 8; }  // Yellow Color Button
  else if (results.value == 0xC00070) { IR_data = 9; }  // Blue Color Button
  else if (results.value == 0xC0006E) { IR_data = 10;}  // Red Color Button
  return IR_data;
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
  IR_data = -1;
}

void spectDeploy(bool &isSpectDeployed) {
  if (!isSpectDeployed) {
    for (int angle = 65; angle <= 140; angle += 1) {
      servoPulse(spectServo, angle);
    }
    isSpectDeployed = true;
    digitalWrite(spectLED, LOW);
    Serial.println("# >> Rover is Deploying the Spectrometer.");
  } else {
    digitalWrite(spectLED, HIGH);
    for (int angle = 140; angle >= 65; angle -= 1) {
      servoPulse(spectServo, angle);
    }
    isSpectDeployed = false;
    Serial.println("# >> Rover is Contracting the spectrometer.");
  }
}

void solarDeploy(bool &isSolarDeployed) {
  if (!isSolarDeployed) {
    for (int angle = 80; angle >= 10; angle -= 1) {
      servoPulse(solarServo, angle);
    }
    isSolarDeployed = true;
    Serial.println("# >> Rover is Deploying the Solar Panel.");
  } else {
    for (int angle = 10; angle <= 80; angle += 1) {
      servoPulse(solarServo, angle);
    }
    isSolarDeployed = false;
    Serial.println("# >> Rover is Contracting the Solar Panel.");
  }
}

void laserBeam(bool &isLaserDeployed) {
  if (!isLaserDeployed) {
    digitalWrite(laserLight, HIGH);
    isLaserDeployed = true;
    Serial.println("# >> Rover is Activating the Laser Beam.");
  } else {
    digitalWrite(laserLight, LOW);
    isLaserDeployed = false;
    Serial.println("# >> Rover is Deactivating the Laser Beam.");
  }
}
