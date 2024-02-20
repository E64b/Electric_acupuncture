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
      lcd.print(' ');
      lcd.print("of 22");
      lcd.setCursor(0, 1);
      if (data.program < 3) {
        lcd.print("Hold for setup");
      } else if (data.program > 2) {
        lcd.print("Step");
        lcd.print(' ');
        lcd.print(stepTime[data.program - 3]);
        lcd.print(' ');
        lcd.print("sec");
      }
      break;

    case SETTING_PROGRAM:
      lcd.setCursor(0, 0);
      lcd.print("Prog");
      lcd.print(' ');
      lcd.print(data.program);
      lcd.print(' ');
      lcd.print("Step");
      lcd.print(' ');
      lcd.print(data.step + 1);
      lcd.setCursor(0, 1);
      lcd.print("Time");
      lcd.print(' ');
      if (data.program == 1) {
        lcd.print(DataMem._1[data.step - 1]);
      }
      if (data.program == 2) {
        lcd.print(DataMem._2[data.step - 1]);
      }
      lcd.print(' ');
      lcd.print("sec");
      break;

    case WORK:
      lcd.setCursor(0, 0);
      lcd.print("Step");
      lcd.print(' ');
      if (data.program > 3) {
        lcd.print(data.step + 1);
      } else {
        lcd.print(data.step);
      }
      lcd.print(' ');
      lcd.print(data.amperage_uA);
      lcd.print("uA");
      lcd.setCursor(0, 1);
      lcd.print("Time");
      lcd.print(' ');
      if (data.program == 1) {
        lcd.print(DataMem._1[data.step]);
      }
      if (data.program == 2) {
        lcd.print(DataMem._2[data.step]);
      }
      if (data.program > 2 && data.program <= 22) {
        lcd.print(data.timeStep / 1000);
      }
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