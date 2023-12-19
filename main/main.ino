#include "main.h"

INA219 ina(0.1f, 0.5f, 0x40);
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

  while (!ina.begin()) {
  }
  Serial.println("INA OK");

  ina.setResolution(INA219_VSHUNT, INA219_RES_12BIT);
  ina.setResolution(INA219_VBUS, INA219_RES_12BIT);

  reg.clearAll();
  reg.writeAll(false);

  for (uint16_t i = 0; i < ALL_DATA; i++) {
    data.out[i] = false;
    reg.clear(i);
  }
  reg.update();

  if (EEPROM.get(0, DataMem.test_mem) != false) {
    EEPROM.put(0, DataMem);
  } else {
    EEPROM.get(0, DataMem);
  }

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Electric");
  lcd.setCursor(2, 1);
  lcd.print("Acupuncture");
  delay(3000);
  lcd.clear();
}

void loop() {
  enc.tick();
  display();
  sensor();
  work();
  send_val();
}