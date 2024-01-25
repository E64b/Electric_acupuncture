#include "main.h"

void lcdDisplay() {
  if (data.displayRedraw) {
    data.displayRedraw = false;
    lcd.clear();

    switch (data.currentState) {
    case SETTING_BEFORE_START:
      startMenu();
      lcd.setCursor(0, 0);
      lcd.print("Prog");
      lcd.print(' ');
      lcd.print(data.program);
      lcd.print("of 22");
      lcd.setCursor(0, 1);
      lcd.print("Max I="); 
      lcd.print(' '); 
      lcd.print(data.maxAmperage);
      break;

    case SETTING_PROGRAM:
      setting();
      break;

    case WORK:
      work();
      lcd.setCursor(0, 0);
      lcd.print("Step");
      lcd.print(' ');
      lcd.print(data.step);
      lcd.print(' ');
      lcd.print("I=");
      lcd.print(data.current);
      lcd.setCursor(0, 1);
      lcd.print("Time");
      lcd.print(' ');
      lcd.print(data.timeStep);
      lcd.print(' ');
      lcd.print("sec");
      break;

    case ERROR:
      lcd.setCursor(0, 0);
      lcd.print("!!!ERROR!!!");
      lcd.setCursor(0, 1);
      lcd.print("!!!OVERLOAD!!!");
      break;
    }
  }
}