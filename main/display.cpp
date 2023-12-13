#include "main.h"
void display() {
  if (data.displayRedraw) {
    data.displayRedraw = false;
    lcd.clear();

    if (data.Settings) {
      lcd.setCursor(0, 0);
      lcd.print("Setting I=");
      lcd.print(data.i);
      lcd.setCursor(0, 1);
      lcd.print("Time ");
      lcd.print(data.time);
      lcd.print(" sec");
    }

    if (data.Work) {
      lcd.setCursor(0, 0);
      lcd.print("Step ");
      lcd.print(data.i);
      lcd.print();
      lcd.setCursor(0, 1);
    }

    if (data.StartMenu) {
    }
  }
}
