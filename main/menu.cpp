#include "main.h"

void menu() {
  switch (data.currentState) {

  case SETTING_BEFORE_START:
    if (enc.left()) {
      if (data.program >= 1) {
        data.program--;
        data.displayRedraw = true;
      }
    }

    if (enc.right()) {
      if (data.program <= 21) {
        data.program++;
        data.displayRedraw = true;
      }
    }

    if (enc.leftH()) {
      if (data.maxCurrent >= 1) {
        data.maxCurrent--;
        data.displayRedraw = true;
      }
    }

    if (enc.rightH()) {
      if (data.maxCurrent < 500) {
        data.maxCurrent++;
        data.displayRedraw = true;
      }
    }

    if (enc.click()) {
      data.startMenu = false;
      data.work = true;
      data.displayRedraw = true;
      data.display = 2;
    }

    if (enc.hold()) {
      if ((data.program == 1) or (data.program == 2)) {

      } else {
        data.work = true;
      }
      data.displayRedraw = true;
    }
    break;

  case SETTING_PROGRAM:
    if (enc.left()) {
      if (data.setTime >= 1000) {
        data.setTime--;
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
    break;

  case SETTING_IN_WORK:

    break;

  case WORK:

    break;
  }
}