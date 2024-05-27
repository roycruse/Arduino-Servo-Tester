#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
Servo myservo3;  // create servo object to control a servo
Servo myservo4;  // create servo object to control a servo
// twelve servo objects can be created on most boards

#define led1 LED_BUILTIN_TX
#define led2 LED_BUILTIN_RX

#define modePin 2
#define downPin 3
#define upPin 4

int min = 1100;
int max = 1900;
int mid = 1500;
int trim = 0;

int mode = 0;

byte modeLastState = HIGH;
byte downLastState = HIGH;
byte upLastState = HIGH;
unsigned long debounceDuration = 100; // millis
unsigned long lastTimeButtonStateChanged = 0;


void setup() {
  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(8);  // attaches the servo on pin 9 to the servo object
  myservo3.attach(7);  // attaches the servo on pin 9 to the servo object
  myservo4.attach(6);  // attaches the servo on pin 9 to the servo object
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(modePin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(upPin, INPUT_PULLUP);
  gotoMid();
}

void loop() {
  if (millis() - lastTimeButtonStateChanged > debounceDuration) {
      byte modeState = digitalRead(modePin);
      byte downState = digitalRead(downPin);
      byte upState = digitalRead(upPin);
      if (modeState != modeLastState || downState != downLastState || upState != upLastState) {
        lastTimeButtonStateChanged = millis();
      }
      if (modeState == HIGH && modeLastState == LOW) {
        mode ++;
      }
      else if (downState == HIGH && downLastState == LOW) {
        trim = trim + 10;
      }
      else if (upState == HIGH && upLastState == LOW) {
        trim = trim - 10;
      }
      else if (downLastState == LOW && upLastState == LOW) {
        trim = 0;
      }
      modeLastState = modeState;
      downLastState = downState;
      upLastState = upState;
      if (modeState == LOW && modeLastState == LOW && millis() - lastTimeButtonStateChanged > 1000){
      modeState = HIGH;
      modeLastState = HIGH;
      flashLights();
    }
}
  if (mode > 3){
    mode = 0;
  }
  if (mode == 0){
    gotoMid();
  }
  else if (mode == 1){
    gotoMin();
  }
  else if (mode == 2){
    gotoMid();
  }
  else if (mode == 3){
    gotoMax();
  }
  else {
    delay(3000);
    gotoMin();
    delay(3000);
    gotoMid();
    delay(3000);
    gotoMax();
    delay(3000);
    gotoMid();
  }
}

void gotoMid() {
  digitalWrite(led1,0);
  digitalWrite(led2,0);
  myservo1.write(mid + trim);
  myservo2.write(mid + trim);
  myservo3.write(mid - trim);
  myservo4.write(mid - trim);
}

void gotoMax() {
  digitalWrite(led1,0);
  digitalWrite(led2,1);
  myservo1.write(max + trim);
  myservo2.write(max + trim);
  myservo3.write(min - trim);
  myservo4.write(min - trim);
}

void gotoMin() {
  digitalWrite(led1,1);
  digitalWrite(led2,0);
  myservo1.write(min + trim);
  myservo2.write(min + trim);
  myservo3.write(max - trim);
  myservo4.write(max - trim);
}

void flashLights() {
  digitalWrite(led1,1);
  digitalWrite(led2,1);
  delay(1000);
  digitalWrite(led1,0);
  digitalWrite(led2,0);
  delay(1000);
  digitalWrite(led1,1);
  digitalWrite(led2,1);
  delay(1000);
  if (trim < 0){
    digitalWrite(led2,0);
    for (int i = 0;i < abs(trim)/10; i++){
      delay(600);
      digitalWrite(led1,0);
      delay(300);
      digitalWrite(led1,1);
    }
  }
  if (trim > 0){
    digitalWrite(led1,0);
    for (int i = 0;i < abs(trim)/10; i++){
      delay(600);
      digitalWrite(led2,0);
      delay(300);
      digitalWrite(led2,1);
    }
  }
  digitalWrite(led1,1);
  digitalWrite(led2,1);
  delay(1000);
  lastTimeButtonStateChanged = millis();
}