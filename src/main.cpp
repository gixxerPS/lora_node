#include <Arduino.h>           // NEU: unbedingt als erstes

#define HELTEC_WIRELESS_STICK_LITE
#include <heltec_unofficial.h>

unsigned long lastTx = 0;

void setup() {
  // Serial.begin(115200); -> macht heltec_setup()

  heltec_setup();  // initialisiert Pins, RF-Switch, Serial
  RADIOLIB_OR_HALT(
    radio.begin(
      868.125,   // Frequency MHz. Muss exakt zum Waveshare passen.
      125.0,     // Bandwidth kHz. Standard LoRa EU.
      7,         // Spreading Factor. Balance Reichweite/Rate.
      5,         // Coding Rate 4/5. Fehlerkorrektur.
      12,        // Preamble Length. Default SX1262. Muss identisch sein.
      0x12,      // Sync Word. Private LoRa. Muss identisch sein.
      10         // Sendeleistung dBm. EU-Grenzen beachten.
    )
  );

  RADIOLIB_OR_HALT(radio.setCRC(true));
  RADIOLIB_OR_HALT(radio.invertIQ(false));
}

void loop() {
  static uint16_t data = 0;
  if (millis() - lastTx > 2000) {
    lastTx = millis();

    String msg = "test " + String(data++);

    int txState = radio.transmit(msg.c_str());   // blocking
    Serial.println("TX ("+String(txState) + "): " + msg);
  }

  // --- Empfangen (alles was reinkommt printen) ---
  // kurze Timeout-Zeit, damit loop nicht ewig blockiert
  uint8_t buf[256];
  int16_t state = radio.receive(buf, sizeof(buf), 50);  // 50 ms timeout

  if (state == RADIOLIB_ERR_NONE) {
    // Länge bestimmen (RadioLib nutzt 0-terminiert für Strings hier meist nicht)
    // -> simplest: alles als C-String behandeln, abschließen
    size_t len = radio.getPacketLength();
    if (len > sizeof(buf) - 1) {
      len = sizeof(buf) - 1;
    }
    buf[len] = '\0';

    Serial.print("RX: ");
    Serial.println((char*)buf);
    Serial.print("RSSI: ");
    Serial.println(radio.getRSSI());
    Serial.print("SNR: ");
    Serial.println(radio.getSNR());
  }

  // ein paar Millisekunden Luft lassen
  delay(5);
}

