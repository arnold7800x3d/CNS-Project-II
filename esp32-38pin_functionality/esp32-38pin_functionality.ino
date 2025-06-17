/*
  this is the source code for the esp32 38-pin functionality of the bank monitoring system. it features:
    - esp32 38pin   - microcontroller
    - dht11 module  - temperature and humidity readings
    - OLED display  - display
    - LED           - status indicators
    - PIR           - detect motion
    - buzzer        - alarm indicator
*/

// libraries
#include "DHT.h"  // DHT11 library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// variables
#define DHTPIN 0  // pin for the DHT module
#define DHTTYPE DHT11
#define SCREEN_WIDTH 128  // OLED display width (px)
#define SCREEN_HEIGHT 64  // OLED display height (px)

const int ledPin = 2;  // pin for the LED

// objects
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);

  pinMode(ledPin, OUTPUT);
  dht.begin(); // initialize the dht11 module
}

void loop() {
  // oled display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // dht11 module functionality
  float humidity = dht.readHumidity(); // variable for humidity
  float temperature = dht.readTemperature(); // variable for temperature

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!"); // display if dht module reads an invalid value
    display.print("DHT11 module error");
  } else {
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println("%"); // display humidity values on the serial monitor
    display.print("Humidity: "); display.print(humidity); display.println("%"); // display humidity values on the oled display
    Serial.print("Temperature: "); Serial.print(temperature); Serial.println("°C"); // display temperature values on the serial monitor
    display.print("Temperature: "); display.print(temperature); display.println("°C"); // display temperature values on the oled display
  }

  display.display();
  delay(1000);
}
