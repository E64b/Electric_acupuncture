#include "main.h"

void sensor() {

  float voltage = ina.getShuntVoltage_mV();
  float amperage = ina.getCurrent_mA();

  if (data.oldAmperage != amperage || data.oldVoltage != data.voltage) {
    data.oldAmperage = amperage;
    data.oldVoltage = voltage;
    data.voltage = voltage;
    data.amperage_uA = amperage * COEFFICIENT;
  }
}