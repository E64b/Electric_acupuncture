#include "main.h"

void StartMenu() {
  if (data.StartMenu) {
    if (eb.left()) {
      if (data.Program >= 1) {
        data.Program--;
        data.DisplayRedraw = true;
      }
    }

    if (eb.right()) {
      if (data.Program <= 21) {
        data.Program++;
        data.DisplayRedraw = true;
      }
    }

    if (eb.leftH()) {
      if (data.MaxCurrent >= 1) {
        data.MaxCurrent--;
        data.DisplayRedraw = true;
      }
    }

    if (eb.rightH()) {
      if (data.MaxCurrent < 500) {
        data.MaxCurrent++;
        data.DisplayRedraw = true;
      }
    }

    if (eb.click()) {
      data.StartMenu = false;
      data.Work = true;
      data.DisplayRedraw = true;
    }

    if (eb.hold()) {
      data.StartMenu = false;
      if ((data.Program == 1) or (data.Program == 2)) {
        data.Settings = true;
      } else {
        data.Work = true;
      }
      data.DisplayRedraw = true;
    }
  }
}
