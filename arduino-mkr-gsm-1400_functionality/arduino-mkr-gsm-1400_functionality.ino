/*
this is the source code for the arduino mkr gsm 1400 functionality of the bank monitoring file. it features:
  - arduino mkr gsm 1400      - microcontroller
  - ultrasonic sensor         - detect approaching objects
  - light dependent resistor  - detect for light based tampering
  - buzzer                    - alarm indicator
  - LED                       - status indicator
  - OLED screen               - display
*/

// libraries
#include <Servo.h> // servo motor library
// OLED display libraries
#include <Adafruit_SSD1306.h> 
#include <Adafruit_GFX.h>

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0X3C

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

// objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT); // set ledPin as an output
  pinMode(buzzerPin, OUTPUT); // set buzzerPin as an output
  pinMode(ldrPin, INPUT); // set ldrPin as an output
  pinMode(trigPin, OUTPUT); // set trigPin as an output
  pinMode(echoPin, INPUT); // set echoPin as an input

  // initializion of the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing display");
  display.display();
  delay(2000);
  
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

  // print on the OLED display
  display.print("Angle: "); display.println(angle);
  display.print("Distance: "); display.print(cm); display.println("cm");

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
