#include "main.h"

#define START_SETTING 0
#define SETTING 1
#define WORK 2

void display() {
  if (data.displayRedraw) {
    data.displayRedraw = false;
    lcd.clear();

    switch (data.display) {
    case START_SETTING:
      startMenu();
      lcd.setCursor(0, 0);
      lcd.print("Prog ");
      lcd.print(data.program);
      lcd.print(" of 22");
      lcd.setCursor(0, 1);
      lcd.print("Max I= ");
      lcd.print(data.maxCurrent);
      break;

    case SETTING:
      setting();
      break;

    case WORK:
      work();
      lcd.setCursor(0, 0);
      lcd.print("Step ");
      lcd.print(data.step);
      lcd.print(" I= ");
      lcd.print(data.current);
      lcd.setCursor(0, 1);
      lcd.print("Time ");
      lcd.print(data.timeStep);
      lcd.print(" sec");
      break;
    }
  }
}
