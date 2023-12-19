#include "main.h"

void startMenu() {
  if (data.startMenu) {
    if (enc.left()) {
      if (data.program >= 1) {
        data.program--;
        data.displayRedraw = true;
      }
    }

    if (enc.right()) {
      if (data.program <= 21) {
        data.program++;
        data.displayRedraw = true;
      }
    }

    if (enc.leftH()) {
      if (data.maxCurrent >= 1) {
        data.maxCurrent--;
        data.displayRedraw = true;
      }
    }

    if (enc.rightH()) {
      if (data.maxCurrent < 500) {
        data.maxCurrent++;
        data.displayRedraw = true;
      }
    }

    if (enc.click()) {
      data.startMenu = false;
      data.work = true;
      data.displayRedraw = true;
      data.display = 2;
    }

    if (enc.hold()) {
      data.startMenu = false;
      if ((data.program == 1) or (data.program == 2)) {
        data.settings = true;
        data.display = 1;
      } else {
        data.work = true;
        data.display = 2;
      }
      data.displayRedraw = true;
    }
  }
}
