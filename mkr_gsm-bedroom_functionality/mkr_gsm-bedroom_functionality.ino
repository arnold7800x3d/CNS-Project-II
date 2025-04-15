/*
this is the source code for the bedroom functionality of the smart home. it features:
  - arduino mkr gsm 1400      - Microcontroller
  - ultrasonic sensor         - Detect approaching objects
  - light dependent resistor  - Detect for any bright lights
  - buzzer                    - Alarm
  - lED                       - Bulb
*/

// libraries
#include <Servo.h> // servo motor library

// variables
const int servoPin = 6; // pin for the servo motor
const int ledPin = 0; // pin for the LED
const int buzzerPin = 1; // pin for the buzzer
const int ldrPin = A0; // pin for the LDR
const int trigPin = 7; // pin for the ultrasonic trigger
const int echoPin = 5; // pin for the ultrasonic echo
long duration, cm, inches; // ultrasonic distance variables

// servo timing variables and objects
Servo servoMotor; // servo motor object
int angle = 0; // angle of the servo motor
unsigned long lastMotionTime = 0; // track the last time the servo motor moved

// buzzer state variable
bool buzzerOn = false; // track buzzer state

//led state variable
bool ledOn = false;

// light detection variables
unsigned long lastLightDetectionTime = 0; // track the last time light was detected 

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT); // set ledPin as an output
  pinMode(buzzerPin, OUTPUT); // set buzzerPin as an output
  pinMode(ldrPin, INPUT); // set ldrPin as an output
  pinMode(trigPin, OUTPUT); // set trigPin as an output
  pinMode(echoPin, INPUT); // set echoPin as an input
  
  // attach the servo to the pin number
  servoMotor.attach(servoPin);
}

void loop() {
  detectObject(); // call function to detect objects
  detectLight(); // call function to detect light

  digitalWrite(ledPin, ledOn ? HIGH : LOW); // set the LED state 
  ledOn = false; // reset flag for next flag
  delay(1000);
}

// this function utilizes the ultrasonic sensor and the servo motor to detect approaching objects
void detectObject() {
  unsigned long currentTime = millis();

  if (currentTime - lastMotionTime >= 500) {
    lastMotionTime = currentTime;

    if (angle <= 120) {
      servoMotor.write(angle);
      measureDistance(angle);
      angle += 10;
    } else { 
      angle = 0; // reset for full scan again
    }
  }
}

// function to measure the distance between the objects at specific angles
void measureDistance(int angle) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  cm = (duration / 2) / 29.1;
  inches = (duration / 2) / 74;

  Serial.print("Angle: "); Serial.println(angle); // print the angle at which the distance was taken
  Serial.print("Distance: "); Serial.print(cm); Serial.println(" cm"); // print the distance

  // if distance to object is less than 10cm, sound the alarm
  if (cm <= 10) {
    soundBuzzer();
    ledOn = true;
  } else {
    turnBuzzerOff();
  }
}

// function to sound the buzzer
void soundBuzzer() {
  if (!buzzerOn) {
    tone(buzzerPin, 2000);
    buzzerOn = true;
  }
}

// function to turn off the buzzer
void turnBuzzerOff() {
  if (buzzerOn) {
    noTone(buzzerPin);
    buzzerOn = false;
  }
}

// function to detect light
void detectLight() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastLightDetectionTime >= 300) {
    lastLightDetectionTime = currentTime;

    int ldrValue = analogRead(ldrPin); // read value from the ldr
    Serial.print("LDR value: "); Serial.println(ldrValue); // print the value

    if (ldrValue >= 400) {
      tone(buzzerPin, 2000);
      ledOn = true;
    } else {
      if (!buzzerOn) {
        noTone(buzzerPin); // if buzzer is active via the object detection have the buzzer off
      }
    }
  }
}
