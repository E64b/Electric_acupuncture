#include "main.h"

void sensor(){
	data.Voltage = ina.getVoltage();
	data.Current = ina.getCurrent();
	}
