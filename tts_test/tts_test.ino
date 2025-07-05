#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

// Replace these with your keys from The Things Stack
static const u1_t PROGMEM DEVEUI[8] = { 0x16, 0xBF, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
static const u1_t PROGMEM APPEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const u1_t PROGMEM APPKEY[16] = { 0xA9, 0x05, 0x25, 0x13, 0xE7, 0x0A, 0x1A, 0xB1, 0x4D, 0xDC, 0x0D, 0xBF, 0x34, 0x02, 0x14, 0xB7 };

void os_getDevEui(u1_t* buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getArtEui(u1_t* buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevKey(u1_t* buf) { memcpy_P(buf, APPKEY, 16); }

// Pin mapping for ESP32
const lmic_pinmap lmic_pins = {
  .nss = 5,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 14,
  .dio = {26, 33, LMIC_UNUSED_PIN}
};

void onEvent(ev_t ev) {
  Serial.print(os_getTime());
  Serial.print(": ");

  switch(ev) {
    case EV_SCAN_TIMEOUT:    Serial.println("EV_SCAN_TIMEOUT"); break;
    case EV_BEACON_FOUND:    Serial.println("EV_BEACON_FOUND"); break;
    case EV_BEACON_MISSED:   Serial.println("EV_BEACON_MISSED"); break;
    case EV_BEACON_TRACKED:  Serial.println("EV_BEACON_TRACKED"); break;
    case EV_JOINING:         Serial.println("EV_JOINING"); break;
    case EV_JOINED:          Serial.println("EV_JOINED"); break;
    case EV_JOIN_FAILED:     Serial.println("EV_JOIN_FAILED"); break;
    case EV_REJOIN_FAILED:   Serial.println("EV_REJOIN_FAILED"); break;
    case EV_TXCOMPLETE:      Serial.println("EV_TXCOMPLETE"); break;
    case EV_LOST_TSYNC:      Serial.println("EV_LOST_TSYNC"); break;
    case EV_RESET:           Serial.println("EV_RESET"); break;
    case EV_RXCOMPLETE:      Serial.println("EV_RXCOMPLETE"); break;
    case EV_LINK_DEAD:       Serial.println("EV_LINK_DEAD"); break;
    case EV_LINK_ALIVE:      Serial.println("EV_LINK_ALIVE"); break;
    case EV_TXSTART:         Serial.println("EV_TXSTART"); break;
    default:                 Serial.print("Unknown event: "); Serial.println(ev); break;
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  os_init();
  LMIC_reset();

  // Set LoRaWAN parameters
  LMIC_setDrTxpow(DR_SF7, 14);
  LMIC_startJoining();
}

void loop() {
  os_runloop_once();
}
