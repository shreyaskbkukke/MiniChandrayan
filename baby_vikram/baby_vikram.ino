#include <IRremote.h>
#include <Servo.h>

int receiverPin = A5; // IR receiver connected to pin A5
int servo1Pin = A4;   // Servo 1 control pin
int servo2Pin = A3;   // Servo 2 control pin

int const redLEDPin = 5;
int const greenLEDPin = 4;

int ENA = 11;         // Enable for Motor 1
int IN1 = 10;         // Motor 1 Input 1
int IN2 = 9;         // Motor 1 Input 2
int IN3 = 8;        // Motor 2 Input 1
int IN4 = 7;        // Motor 2 Input 2
int ENB = 6;         // Enable for Motor 2

IRrecv irrecv(receiverPin);
decode_results results;

unsigned long previousMillis = 0;
const unsigned long motorDuration = 500; // Half a second (500 milliseconds)

Servo servo1; // Create a servo object for servo 1
Servo servo2; // Create a servo object for servo 2

int servo1Position = 0; // Starting position for servo 1 (90 degrees)
int servo2Position = 0; // Starting position for servo 2 (90 degrees)
int deployPosition = 0;  // Deployed position (0 degrees)
int contractPosition = 90; // Contracted position (90 degrees)

bool isMoving = false; // To track if the servo is currently in motion

bool servo1Active = false; // Track the active state of servo 1
bool servo2Active = false; // Track the active state of servo 2

void setup() {
  Serial.begin(9600);
  servo1.attach(servo1Pin); // Attach servo 1 to the specified pin
  servo2.attach(servo2Pin); // Attach servo 2 to the specified pin

  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Start the IR receiver
  irrecv.enableIRIn();
}

void loop() {
  greenLED();
  redLED();
  if (irrecv.decode(&results)) {
    switch (results.value) {
      case 0xC00020: // Button 1 on remote (customize for your remote)
        moveForward();
        break;
      case 0xC00021: // Button 2 on remote
        moveBackward();
        break;
      case 0xC0003C: // Button 3 on remote (Toggle servo 1)
        toggleServo1();
        break;
      case 0xC000CB: // Button 4 on remote (Toggle servo 2)
        toggleServo2();
        break;
    }
    irrecv.resume(); // Receive the next value
  }
  if (isMoving) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= motorDuration) {
      // Stop the motors and servo after half a second
      stopMotors();
    }
  }
}

void toggleServo1() {
  if (servo1Active) {
    servo1Position = 90;
    servo1Active = false;
  } else {
    servo1Position = 0;
    servo1Active = true;
  }
  servo1.write(servo1Position);
}

void toggleServo2() {
  if (servo2Active) {
    servo2Position = 0;
    servo2Active = false;
  } else {
    servo2Position = 180;
    servo2Active = true;
  }
  servo2.write(servo2Position);
}

void moveForward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  previousMillis = millis(); // Reset the timer
  isMoving = true;
}

void stopMotors() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  previousMillis = millis(); // Reset the timer
  isMoving = true;
}

void redLED(){
  digitalWrite(redLEDPin, HIGH);
  delay(200);
  digitalWrite(redLEDPin, LOW);
  delay(800);
}

void greenLED(){
  digitalWrite(greenLEDPin, HIGH);
  delay(500);
  digitalWrite(greenLEDPin, LOW);
  delay(500);
}