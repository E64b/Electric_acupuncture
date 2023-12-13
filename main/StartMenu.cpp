#include "main.h"

void StartMenu() {
  if (data.StartMenu) {
    if (eb.left()) {
      if (data.program >= 1) {
        data.program--;
        data.displayRedraw = true;
      }
    }

    if (eb.right()) {
      if (data.program <= 21) {
        data.program++;
        data.displayRedraw = true;
      }
    }

    if (eb.leftH()) {
      if (data.MaxCurrent >= 1) {
        data.MaxCurrent--;
        data.displayRedraw = true;
      }
    }

    if (eb.rightH()) {
      if (data.MaxCurrent < 500) {
        data.MaxCurrent++;
        data.displayRedraw = true;
      }
    }

    if (eb.click()) {
      data.StartMenu = false;
      data.Work = true;
      data.displayRedraw = true;
    }

    if (eb.hold()) {
      data.StartMenu = false;
      if ((data.program == 1) or (data.program == 2)) {
        data.Settings = true;
      } else {
        data.Work = true;
      }
      data.displayRedraw = true;
    }
  }
}
