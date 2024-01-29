#include "main.h"

Adafruit_INA219 ina;
LiquidCrystal_I2C lcd(0x27, 16, 2);
HC595<SHIFTS, HC_PINS> reg(STCP, DS, SHCP);
EncButton enc(2, 3, 4);
Data data;
DataMemory DataMem;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("Serial OK");

  if (!ina.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) {
      delay(10);
    }
  }

  ina.setCalibration_16V_400mA();

  reg.clearAll();
  reg.writeAll(false);

  for (uint16_t i = 0; i < ALL_DATA; i++) {
    data.out[i] = false;
    reg.clear(i);
  }
  reg.update();

  if (EEPROM.get(0, DataMem.testMem) != false) {
    EEPROM.put(0, DataMem);
  } else {
    EEPROM.get(0, DataMem);
  }

  lcd.init();
  lcd.setBacklight(BACKLIGHT);
  lcd.setCursor(0, 0);
  lcd.print("Electric");
  lcd.setCursor(2, 1);
  lcd.print("Acupuncture");
  delay(3000);
  lcd.clear();
}

void loop() {
  enc.tick();
  lcdDisplay();
  sensor();
  work();
  //protection();
  sendVal();
}