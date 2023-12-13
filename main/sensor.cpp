#include "main.h"

void sensor() {
  data.Voltage = ina.getVoltage();
  data.Current = ina.getCurrent();
  if (data.OldCurrent != data.Current || data.OldVoltage != data.Voltage) {
    data.displayRedraw = true;
    data.OldCurrent = data.Current;
    data.OldVoltage = data.Voltage;
  }
}