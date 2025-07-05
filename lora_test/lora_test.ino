#include <SPI.h>
#include <LoRa.h>

#define SS      5
#define RST     14
#define DIO0    26

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa sender");

  LoRa.setPins(SS, RST, DIO0);

  while(!LoRa.begin(868E6)) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("LoRa init succeeded");
}

void loop() {
  LoRa.beginPacket();
  LoRa.print("Hello from ESP32");
  LoRa.endPacket();

  Serial.println("Sent packet");
  delay(2000);
}
