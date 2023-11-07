#include "main.h"

void setting() {
  if (data.Settings) {
    if (eb.left()) {
      if (data.time >= 1000) {
        data.time - 1000;
        data.DisplayRedraw = true;
      }
    }

    if (eb.right()) {
      if (data.time <= 998000) {
        data.time + 1000;
        data.DisplayRedraw = true;
      }
    }

    if (eb.leftH()) {
      if (data.i >= 1) {
        i--;
        data.DisplayRedraw = true;
      }
    }

    if (eb.rightH()) {
      if (i < ALL_DATA) {
        data.DisplayRedraw = true;
        i++
      }
    }

    if (eb.click()) {
      data.Settings = false;
      data.DisplayRedraw = true;
    }

    if (eb.hold()) {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.printstr("SETTING SAVE");
      lcd.setCursor(0, 1);
      lcd.printstr("Pls wait...");
      delay(5000);
      lcd.clear();
      data.DisplayRedraw = true;
    }
  }
}
