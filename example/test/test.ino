#include "GyverHC595.h"
#include <Arduino.h>
#include <SPI.h>
#include "LiquidCrystal_I2C.h"

#define STCP 10 // pinCS 12
#define DS 11   // pinDT 14
#define SHCP 13 // pinCLK 11

#define SHIFTS 10 //Колличество чипов 595 подключенных последовательно
#define HC595_DELAY 0
#define ONE_SHIFT_DATA 8
#define ALL_DATA (SHIFTS * ONE_SHIFT_DATA)

GyverHC595<SHIFTS, HC_SPI> reg(STCP);
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint16_t OLD_PIN = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.print("Serial OK");
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.printstr("LCD OK");
  reg.clearAll();
  reg.update();
  delay(1000);
}

void loop() {
  /*Включаем все по одному потом выключаем и опять */
  for (uint16_t CURRENT_PIN = 0; CURRENT_PIN < ALL_DATA; CURRENT_PIN++) {
    reg.write(OLD_PIN, false);
    reg.write(CURRENT_PIN, true);
    reg.update();
    lcd.clear();
    lcd.home();
    lcd.printstr("PIN = ");
    lcd.printstr(CURRENT_PIN);
    delay(30000); //Тут время на выход в Миллисекундах
    OLD_PIN = CURRENT_PIN;
  }
  reg.clearAll();
  reg.update();
}
