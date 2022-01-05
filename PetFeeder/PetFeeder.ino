#include <Servo.h>
#define second 1000
#define minute 60000
#define hour 3600000

Servo servo;

int servoPin = 9;
const int  buttonPin = 2;
char command;
int buttonState;         // current state of the button
int lastButtonState = LOW;     // previous state of the button
int debounce_delay = 50;
int lastFlickerableState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long lastFeeding = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("You're connected via Bluetooth");
  servo.attach(servoPin);
  pinMode(buttonPin, INPUT_PULLUP);

}

void loop() {

  if (Serial.available()) {
    bluetoothController();
  }
  buttonState = digitalRead(buttonPin);
  
  if (buttonState != lastFlickerableState) {
    lastDebounceTime = millis();
    lastFlickerableState = buttonState;
  }
  if ((millis() - lastDebounceTime) > debounce_delay) {
    if (lastButtonState == HIGH && buttonState == LOW) {
      lastFeeding = millis();
      servo.write(180);
    }
    else if (lastButtonState == LOW && buttonState == HIGH)
      servo.write(0);

    lastButtonState = buttonState;
  }
  
  if (millis() - lastFeeding > 2 * hour)
    dispense();
}
void bluetoothController() {
  servo.attach(servoPin);
  command = (Serial.read());
  if (command == '1') {
    lastFeeding = millis();
    servo.write(0);
    delay(1000);
  }
  else if (command == '2') {
    servo.write(180);
    delay(1000);
  }
}

void dispense() {
  lastFeeding=millis();
  servo.write(180);
  delay(1000);
  servo.write(0);
  delay(1000);
}
