#include "main.h"

void sensor() {
  data.Voltage = ina.getVoltage();
  data.Current = ina.getCurrent();
  if ((data.OldCurrent != data.Current) or (data.OldVoltage != data.Voltage)) {
    data.DisplayRedraw = true;
    data.OldCurrent = data.Current;
    data.OldVoltage = data.Voltage;
  }
}
