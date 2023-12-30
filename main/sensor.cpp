#include "main.h"

void sensor() {
  data.voltage = ina.getShuntVoltage_mV();
  data.current = ina.getPower_mW();
  if (data.oldCurrent != data.current || data.oldVoltage != data.voltage) {
    data.displayRedraw = true;
    data.oldCurrent = data.current;
    data.oldVoltage = data.voltage;
  }
}