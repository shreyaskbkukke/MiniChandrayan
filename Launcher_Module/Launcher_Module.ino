const int Start_buttonPin = 8;    // Pin for the "start" button
const int Reset_buttonPin = 9;    // Pin for the "reset" button
const int FanPin = 11;            // Pin for controlling the fan
const int ExLEDPin = 10;          // Pin for controlling the LED
const int RocketLiftPin = 7;
const int VikramPin = 6;

int start_buttonState = 0;        // Current state of the "start" button
int start_lastButtonState = 0;    // Previous state of the "start" button
int reset_buttonState = 0;        // Current state of the "reset" button
int reset_lastButtonState = 0;    // Previous state of the "reset" button

bool fanOn = false;
bool lightOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(Start_buttonPin, INPUT_PULLUP);
  pinMode(Reset_buttonPin, INPUT_PULLUP);
  pinMode(FanPin, OUTPUT);
  pinMode(ExLEDPin, OUTPUT);
  pinMode(RocketLiftPin, OUTPUT);
  pinMode(VikramPin, OUTPUT);

  digitalWrite(FanPin, LOW);
  digitalWrite(ExLEDPin, LOW);
  digitalWrite(RocketLiftPin, LOW);
  digitalWrite(VikramPin, LOW);
}

void loop() {
  checkStart();
  checkReset();
}

void checkStart() {
  start_buttonState = digitalRead(Start_buttonPin);
  if (start_buttonState != start_lastButtonState) {
    if (start_buttonState == LOW) {
      // "Start" button is pressed
      Serial.println("Start button pressed");
      if (!fanOn) {
        digitalWrite(FanPin, HIGH);
        delay(4000);
        digitalWrite(ExLEDPin, HIGH);
        delay(5000);

        

        // Turn on the VikramPin for 3 seconds
        digitalWrite(VikramPin, HIGH);
        delay(3000);
        digitalWrite(VikramPin, LOW);

        digitalWrite(FanPin, LOW);
        digitalWrite(ExLEDPin, LOW);
        fanOn = false;
        lightOn = false;
      }
    }
    delay(50);
  }
  start_lastButtonState = start_buttonState;
}

void checkReset() {
  reset_buttonState = digitalRead(Reset_buttonPin);
  if (reset_buttonState != reset_lastButtonState) {
    if (reset_buttonState == LOW) {
      // "Reset" button is pressed
      Serial.println("Reset button pressed");
      // Add your code for handling the "reset" button here
      // Turn on the RocketLiftPin for 3 seconds
        digitalWrite(RocketLiftPin, HIGH);
        delay(3000);
        digitalWrite(RocketLiftPin, LOW);

        delay(1000);
    }
    delay(50);
  }
  reset_lastButtonState = reset_buttonState;
}
