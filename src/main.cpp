#define HELTEC_WIRELESS_STICK_LITE
#include <heltec_unofficial.h>

void setup() {
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

  radio.setCRC(true); 
  radio.setIQ(false);
}

void loop() {
  RADIOLIB(radio.transmit("test"));
  delay(1000);
  heltec_loop();   // hält Button-/Power-Logic am Leben
}


// #include <heltec_unofficial.h>
// #include <RadioLib.h>

// void setup() {
//   heltec_setup();                       // setzt u.a. ANT_SW_CTRL korrekt (V3)
//   SPIClass spi(FSPI);
//   spi.begin(9, 11, 10, 8);              // SCK,MISO,MOSI,SS  -> GPIO9,11,10,8
//   static SX1262 radio = new Module(8,   // NSS
//                                    14,  // DIO1
//                                    12,  // RESET
//                                    13,  // BUSY
//                                    spi);

//   RADIOLIB_OR_HALT(radio.begin(868.100));    // oder exakt 868.125, aber identisch zum Pi
//   RADIOLIB_OR_HALT(radio.setBandwidth(125.0));
//   RADIOLIB_OR_HALT(radio.setSpreadingFactor(7));
//   RADIOLIB_OR_HALT(radio.setCodingRate(5));      // 4/5
//   RADIOLIB_OR_HALT(radio.setPreambleLength(8));
//   RADIOLIB_OR_HALT(radio.setSyncWord(0x12));
//   RADIOLIB_OR_HALT(radio.setCRC(true));
//   RADIOLIB_OR_HALT(radio.setIQ(false));
//   RADIOLIB_OR_HALT(radio.setOutputPower(10));    // 10–14 dBm
// }

// void loop() {
//   RADIOLIB(radio.transmit("test"));
//   delay(5000); // Duty-Cycle im 1%-Band beachten
//   heltec_loop();
// }
