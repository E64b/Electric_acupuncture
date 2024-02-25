#include "main.h"

//#include "./prog/progTime.h"

uint8_t getExit(const uint8_t *prog, uint8_t index) {
  return pgm_read_byte(&prog[index]);
}

uint32_t getTime(uint32_t timeS) { return timeS * 1000; }

void work() {
  if (data.work && data.error == false) {

    if ((data.program == 1 || data.program == 2) &&
        millis() - data.currentMillis > data.timeStep) {

      data.currentMillis = millis();

      if (data.program == 1 && data.step < ALL_DATA) {
        data.timeStep = getTime(DataMem._1[data.step]);
        reg.clearAll();
        reg.set(data.step);
        data.step++;
        data.send = true;
        data.displayRedraw = true;
      } else if (data.step == ALL_DATA && data.program == 1) {
        reg.clearAll();
        data.send = true;
        data.work = false;
        data.step = 0;
        data.displayRedraw = true;
        data.currentState = SETTING_BEFORE_START;
      }

      if (data.program == 2 && data.step < (ALL_DATA / 2)) {
        data.timeStep = getTime(DataMem._2[data.step]);
        reg.clearAll();
        for (uint8_t i = 0; i <= 1; i++) {
          reg.set(data.step * 2);
          reg.set(data.step * 2 + 1);
        }
        data.step++;
        data.send = true;
        data.displayRedraw = true;
      } else if (data.step == ALL_DATA / 2 && data.program == 2) {
        reg.clearAll();
        data.send = true;
        data.work = false;
        data.step = 0;
        data.displayRedraw = true;
        data.currentState = SETTING_BEFORE_START;
      }
    }

    if (data.program > 2 && data.step <= ALL_DATA) {
      if (millis() - data.currentMillis > data.timeStep) {
        data.timeStep = getTime(DataMem.stepTime[data.program - 3]);
        data.currentMillis = millis();
        reg.clearAll();
        reg.clear(data.oldExit);
        const uint8_t *prog = GetProg(data.program);
        uint8_t exit = getExit(prog, data.step);
        data.oldExit = exit;
        reg.set(exit);
        //Serial.println(exit);
        //Serial.println(data.step + 1);
        data.step++;
        data.send = true;
        data.displayRedraw = true;
      } else if (data.step == ALL_DATA) {
        reg.clear(data.oldExit);
        reg.clearAll();
        data.send = true;
        data.work = false;
        data.step = 0;
        data.displayRedraw = true;
        data.currentState = SETTING_BEFORE_START;
      }
    }
  }
}