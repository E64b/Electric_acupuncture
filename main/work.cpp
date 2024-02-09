#include "main.h"

uint16_t getExit(const uint8_t *pChar, uint16_t index) { return pChar[index]; }

uint16_t getTime(uint16_t timeS) { return timeS * 1000; }

void work() {
  if (data.work && data.error == false) {
    if ((data.program == 1 || data.program == 2) &&
        millis() - data.currentMillis >= data.timeStep) {

      data.currentMillis = millis();

      if (data.program == 1 && data.step < ALL_DATA) {
        data.timeStep = getTime(DataMem._2[data.step]);
        reg.clearAll();
        reg.set(data.step);
        data.step++;
        data.send = true;
        data.displayRedraw = true;
      } else if (data.step == ALL_DATA) {
        data.work = false;
        data.step = 0;
        reg.clearAll();
        data.send = true;
        data.currentState = SETTING_BEFORE_START;
        data.displayRedraw = true;
      }

      if (data.program == 2 && data.step < (ALL_DATA / 2)) {
        data.timeStep = getTime(DataMem._2[data.step]);
        data.oldExit = data.step;
        reg.clearAll();
        for (uint8_t i = 0; i <= 1; i++) {
          reg.set(data.step * 2);
          reg.set(data.step * 2 + 1);
        }
        data.step++;
        data.send = true;
        data.displayRedraw = true;
      } else if (data.step == ALL_DATA / 2) {
        data.work = false;
        reg.clearAll();
        data.step = 0;
        data.send = true;
        data.displayRedraw = true;
        data.currentState = SETTING_BEFORE_START;
      }
    }

    if (data.program >= 3 && data.program <= 22 && data.step < ALL_DATA) {

      if (millis() - data.currentMillis >= getTime(DataMem.setTimeToStep) {

        data.currentMillis = millis();

        reg.clearAll();
        reg.clear(data.oldExit);
        uint16_t exit = getExit(data.program, data.step);
        data.oldExit = exit;
        reg.set(exit);
        data.step++;
        data.send = true;
        data.displayRedraw = true;
      }
      if (data.step == ALL_DATA) {
        data.work = false;
        data.currentState = 1;
        data.step = 0;
        reg.clearAll();
        data.displayRedraw = true;
        data.currentState = SETTING_BEFORE_START;
      }
    }
  }
}