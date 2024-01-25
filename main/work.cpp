#include "main.h"

uint16_t getExit(const uint16_t *pChar, uint16_t index) { return pChar[index]; }

void work() {
  if (data.work) {
    if (data.program == 1 || data.program == 2) {
      data.currentMillis = millis();
      if (data.program == 1 && data.step < ALL_DATA) {
        if (data.step == 0) {
          data.timeStep = DataMem._1[0] * 1000;
        }
        DataMem._1[data.step];
        data.step++;
      }
      if (data.program == 2 && data.step < (ALL_DATA / 2)) {
          if (data.step == 0) {
            data.timeStep = DataMem._2[0] * 1000;
          }
          DataMem._2[data.step];
          data.step++;
        }
      uint16_t exit = getExit(data.program, data.i);
      data.send = true;
    }

    if (data.program >= 3 && data.program <= 22) {
      const uint16_t *prog = *GetProg(data.program);
      uint16_t exit = getExit(data.program, data.i);
      data.i++;
      data.send = true;
    }
  }
}