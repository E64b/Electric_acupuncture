#include "main.h"

void lcdDisplay() {
  if (data.displayRedraw) {
    data.displayRedraw = false;
    lcd.clear();

    switch (data.currentState) {
    case SETTING_BEFORE_START:
      lcd.setCursor(0, 0);
      lcd.print("Prog");
      lcd.print(' ');
      lcd.print(data.program);
      lcd.print("of 22");
      lcd.setCursor(0, 1);
      lcd.print("Step");
      lcd.print(' ');
      lcd.print(DataMem.setTimeToStep);
      lcd.print("sec");
       break;

    case SETTING_PROGRAM:
      lcd.setCursor(0, 0);
      lcd.print("Prog");
      lcd.print(' ');
      lcd.print(data.program);
      lcd.print(' ');
      lcd.print("step");
      lcd.print(' ');
      lcd.print(data.i);
      lcd.setCursor(0, 1);
      if (data.program == 1) {
        lcd.print(DataMem._1[data.i]);
      }
      if (data.program == 2) {
        lcd.print(DataMem._2[data.i]);
      }
      lcd.print("sec");
      break;

    case WORK:
      lcd.setCursor(0, 0);
      lcd.print("Step");
      lcd.print(' ');
      lcd.print(data.step);
      lcd.print(' ');
      lcd.print(data.amperage_uA);
      lcd.print("uA");
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