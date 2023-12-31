#include "main.h"

void setting() {
  if (data.settings) {
    if (enc.left()) {
      if (data.setTime >= 1000) {
        data.setTime --;
        data.displayRedraw = true;
      }
    }

    if (enc.right()) {
      if (data.setTime <= 998000) {
        data.setTime++;
        data.displayRedraw = true;
      }
    }

    if (enc.leftH()) {
      if (data.i >= 1) {
        data.i--;
        data.displayRedraw = true;
      }
    }

    if (enc.rightH()) {
      if (data.i < ALL_DATA) {
        data.displayRedraw = true;
        data.i++;
      }
    }

    if (enc.click()) {
      data.settings = false;
      data.work = true;
      data.displayRedraw = true;
      data.display = 2;
    }

    if (enc.hold()) {
      EEPROM.put(0, DataMem);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SETTING SAVE");
      lcd.setCursor(0, 1);
      lcd.print("Pls wait...");
      delay(5000);
      lcd.clear();
      data.displayRedraw = true;
    }
  }
}
