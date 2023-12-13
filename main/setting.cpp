#include "main.h"

void setting() {
  if (data.settings) {
    if (eb.left()) {
      if (data.time >= 1000) {
        data.time - 1000;
        data.displayRedraw = true;
      }
    }

    if (eb.right()) {
      if (data.time <= 998000) {
        data.time + 1000;
        data.displayRedraw = true;
      }
    }

    if (eb.leftH()) {
      if (data.i >= 1) {
        i--;
        data.displayRedraw = true;
      }
    }

    if (eb.rightH()) {
      if (i < ALL_DATA) {
        data.displayRedraw = true;
        i++
      }
    }

    if (eb.click()) {
      data.Settings = false;
      data.displayRedraw = true;
    }

    if (eb.hold()) {

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
