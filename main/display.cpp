#include "main.h"

#define START_SETTING 0
#define SETTING 1
#define WORK 2
#define
#define

void display() {
  if (data.displayRedraw) {
    data.displayRedraw = false;
    lcd.clear();

    switch (data.display) {
    case START_SETTING:
      startMenu();
      break;

    case SETTING:
      setting();
      break;

    case WORK:
      lcd.setCursor(0, 0);
      lcd.print("Step ");
      lcd.print(data.i);
      lcd.print();
      lcd.setCursor(0, 1);
      lcd.setCursor(0, 0);
      lcd.print("Setting I=");
      lcd.print(data.i);
      lcd.setCursor(0, 1);
      lcd.print("Time ");
      lcd.print(data.time);
      lcd.print(" sec");
      break;
    }
  }
}
