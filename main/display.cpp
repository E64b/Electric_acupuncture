#include "main.h"
void display() {
  if (data.DisplayRedraw) {
    data.DisplayRedraw = false;
    lcd.clear();

    if (data.Settings) {
      lcd.setCursor(0, 0);
      lcd.printstr("Setting I=");
      lcd.printstr(data.i);
      lcd.setCursor(0, 1);
      lcd.printstr("Time ");
      lcd.printstr(data.time);
      lcd.printstr(" sec");
    }

    if (data.Work) {
      lcd.setCursor(0, 0);
      lcd.printstr("Step ");
      lcd.printstr(data.i);
      lcd.printstr()
      lcd.setCursor(0, 1);
    }

    if (data.StartMenu) {
    }
  }
}
