/*
  this is the source code for the living room functionality of the smart home. it features:
    - esp32 38pin   - Microcontroller
    - dht11 module  - Temperature and humidity readings
    - OLED display  - TV
    - LED           - Bulb
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
  dht.begin();
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 0);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    display.print("Humidity: "); display.print(humidity); display.println("%");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    display.print("Temperature: "); display.print(temperature); display.println("°C");
  }

  display.display();
  delay(1000);
}
