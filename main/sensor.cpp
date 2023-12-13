#include "main.h"

void sensor() {
  data.voltage = ina.getVoltage();
  data.current = ina.getCurrent();
  if (data.oldCurrent != data.current || data.oldVoltage != data.voltage) {
    data.displayRedraw = true;
    data.oldCurrent = data.current;
    data.oldVoltage = data.voltage;
  }
}