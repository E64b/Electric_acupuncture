#include "main.h"

INA219 ina(0.01f, 0.5f, 0x41);
LiquidCrystal_I2C lcd(0x27, 16, 2);
GyverHC595<SHIFTS, HC_PINS> reg(STCP, DS, SHCP);
EncButton eb(2, 3, 4);
Data data;

void setup(){
	Serial.begin(115200);
	while(!Serial);
	Serial.println("Serial OK");
	while (!ina.begin());
	Serial.println("INA OK");

	lcd.begin();
	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print("Electric");
	lcd.setCursor(2, 1);
	lcd.print("Acupuncture");
	delay(3000);
	lcd.clear();

	reg.clearAll();
	reg.update();
	reg.writeAll(false);	
	}

void loop(){
	display();
	setting();
	sensor();
	work();
	enc();
	}