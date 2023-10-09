#include "main.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
GyverHC595<SHIFTS, HC_PINS> reg(STCP, DS, SHCP);
Data data;

void setup(){
	Serial.begin(115200);
	while(!Serial);
	reg.clearAll();
	reg.update();
	reg.writeAll(false);
	Serial.println("Serial OK");
	}

void loop(){
	display();
	setting();
	sensor();
	work();
	enc();
	}