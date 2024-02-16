#include "main.h"

void menu() {
  switch (data.currentState) {
  case SETTING_BEFORE_START:
    if (enc.left()) {
      if (data.program > 1) {
        data.program--;
        data.displayRedraw = true;
      }
    }

    if (enc.right()) {
      if (data.program < 22) {
        data.program++;
        data.displayRedraw = true;
      }
    }

    if (enc.leftH()) {
    }

    if (enc.rightH()) {
    }

    if (enc.click()) {
      if (data.settingsChanged) {
        EEPROM.put(0, DataMem);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SETTING SAVE");
        lcd.setCursor(0, 1);
        lcd.print("Pls wait...");
        delay(5000);
        lcd.clear();
        data.settingsChanged = false;
      }
      data.work = true;
      data.step = 0;
      data.displayRedraw = true;
      data.currentState = WORK;
    }

    if (enc.hold()) {
      if ((data.program == 1) or (data.program == 2)) {
        data.currentState = SETTING_PROGRAM;
      } else {
        data.work = true;
        data.step = 0;
        data.currentState = WORK;
      }
      data.displayRedraw = true;
    }
    break;

  case SETTING_PROGRAM:
    if (enc.left()) {
      if (data.program == 1 && DataMem._1[data.step] >= 1) {
        DataMem._1[data.step]--;
      }
      if (data.program == 2 && DataMem._2[data.step] >= 1) {
        DataMem._2[data.step]--;
      }
      data.displayRedraw = true;
      data.settingsChanged = true;
    }

    if (enc.right()) {
      if (data.program == 1 && DataMem._1[data.step] < 255) {
        DataMem._1[data.step]++;
      }
      if (data.program == 2 && DataMem._2[data.step] < 255) {
        DataMem._2[data.step]++;
      }
      data.displayRedraw = true;
      data.settingsChanged = true;
    }

    if (enc.leftH()) {
      if (data.step >= 1) {
        data.step--;
        data.displayRedraw = true;
      }
    }

    if (enc.rightH()) {
      if (data.program == 1 && data.step < 63) {
        data.step++;
      }
      if (data.program == 2 && data.step < 31) {
        data.step++;
      }
    }

    if (enc.click()) {
      if (data.settingsChanged) {
        EEPROM.put(0, DataMem);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SETTING SAVE");
        lcd.setCursor(0, 1);
        lcd.print("Pls wait...");
        delay(5000);
        lcd.clear();
        data.settingsChanged = false;
      }
      data.step = 0;
      data.work = true;
      data.displayRedraw = true;
      data.currentState = WORK;
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
      data.step = 0;
      data.currentState = SETTING_BEFORE_START;
      data.displayRedraw = true;
    }
    break;

  case SETTING_IN_WORK:
    if (enc.hold()) {
      data.work = true;
      data.currentState = WORK;
    }
    break;

  case WORK:
    if (enc.click()) {
      if (data.work == true) {
        data.work = false;
      } else if (data.work == false) {
        data.work = true;
        data.currentMillis = millis();
      }
    }

    if (enc.hold()) {
      data.work = false;
      data.step = 0;
      data.currentState = SETTING_BEFORE_START;
    }
    break;
  }
}