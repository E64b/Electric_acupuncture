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
      if (DataMem.setTimeToStep >= 1 && data.program >= 3) {
        DataMem.setTimeToStep--;
        data.displayRedraw = true;
      }
    }

    if (enc.rightH()) {
      if (DataMem.setTimeToStep < 500 && data.program >= 3) {
        DataMem.setTimeToStep++;
        data.displayRedraw = true;
      }
    }

    if (enc.click()) {
      data.work = true;
      data.displayRedraw = true;
      data.currentState = 2;
    }

    if (enc.hold()) {
      if ((data.program == 1) or (data.program == 2)) {
        data.currentState = SETTING_PROGRAM;
      } else {
        data.work = true;
        data.currentState = WORK;
        if (EEPROM.get(0, DataMem.setTimeToStep) = !DataMem.setTimeToStep) {
          EEPROM.put(0, DataMem.setTimeToStep);
          delay(50);
        }
      }
      data.displayRedraw = true;
    }
    break;

  case SETTING_PROGRAM:
    if (enc.left()) {
      if (data.program == 1 && DataMem._1[data.i] >= 1) {
        DataMem._1[data.i]--;
      }
      if (data.program == 2 && DataMem._2[data.i] >= 1) {
        DataMem._2[data.i]--;
      }
      data.displayRedraw = true;
    }

    if (enc.right()) {
      if (data.program == 1 && DataMem._2[data.i] <= 1000) {
        DataMem._1[data.i]++;
      }
      if (data.program == 2 && DataMem._2[data.i] <= 1000) {
        DataMem._2[data.i]++;
      }
      data.displayRedraw = true;
    }

    if (enc.leftH()) {
      if (data.i >= 1) {
        data.i--;
        data.displayRedraw = true;
      }
    }

    if (enc.rightH()) {
      if (data.i < ALL_DATA) {
        data.i++;
        data.displayRedraw = true;
      }
    }

    if (enc.click()) {
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
      data.currentState = SETTING_PROGRAM;
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
      }
    }

    if (enc.hold()) {
      data.work = false;
      data.currentState = SETTING_IN_WORK;
    }
    break;
  }
}