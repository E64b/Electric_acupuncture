#include "GyverHC595.h"
#include <Arduino.h>
#include <SPI.h>

// Поменяй пины под ESP
#define STCP 10 // pinCS 12
#define DS 11   // pinDT 14
#define SHCP 13 // pinCLK 11

#define SHIFTS 10
#define HC595_DELAY 0
#define ONE_SHIFT_DATA 8
#define ALL_DATA (SHIFTS * ONE_SHIFT_DATA)

GyverHC595<SHIFTS, HC_SPI> reg(STCP);
uint16_t OLD_PIN = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.print("Serial OK");

  delay(1000);
  reg.clearAll();
  reg.update();
  delay(1000);
}

void loop() {
  /*Включаем все по одному потом выключаем и опять */
  for (uint16_t CURRENT_PIN = 0; CURRENT_PIN < ALL_DATA; CURRENT_PIN++) {
    reg.clear(OLD_PIN);
    reg.update();
    reg.set(CURRENT_PIN);
    reg.update();
    delay(60000);
    OLD_PIN = CURRENT_PIN;
  }
  reg.clearAll();
  reg.update();
}
