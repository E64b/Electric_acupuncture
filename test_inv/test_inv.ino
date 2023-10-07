#include <Arduino.h>
#include <SPI.h>
#include <stdio.h>

#include "GyverHC595.h"
#include "LiquidCrystal_I2C.h"

#define STCP 10  // pinCS 12
#define DS 11    // pinDT 14
#define SHCP 13  // pinCLK 11

#define SHIFTS 1
#define HC595_DELAY 0
#define ONE_SHIFT_DATA 8
#define ALL_DATA (SHIFTS * ONE_SHIFT_DATA)

GyverHC595<SHIFTS, HC_PINS> reg (STCP, DS, SHCP);
LiquidCrystal_I2C lcd (0x27, 16, 2);

uint16_t CURRENT_PIN = 0;
uint32_t TIMER = 0;
uint16_t OLD_PIN = 0;
bool lcdRedraw = false;

void setup (){
	/*Init Serial*/
	Serial.begin (9600);
	while (!Serial)
		;
	Serial.print ("Serial OK");

	/*Init reg*/
	reg.setAll ();
	reg.update ();

	/*Init LCD*/
	lcd.init ();
	lcd.backlight ();
	lcd.clear ();
	lcd.setCursor (1, 0);
	lcd.print ("by e64b");
	lcd.setCursor (2, 1);
	lcd.print ("Electric acu");
	delay (1000);
	lcd.clear ();
	}

void loop (){
	/*We turn on the pins in turn in increments of time*/
	if ((millis () - TIMER >= 1000) or (TIMER == 0)){
		TIMER = millis ();
		lcdRedraw = true;
		OLD_PIN = CURRENT_PIN;
		if (CURRENT_PIN < ALL_DATA){
			CURRENT_PIN++;
			if (CURRENT_PIN >= ALL_DATA){
				CURRENT_PIN = 0;
				}

			reg.clear (CURRENT_PIN);
			Serial.println (CURRENT_PIN);

			if (TIMER != 0){
				Serial.println (OLD_PIN);
				reg.set (OLD_PIN);
				}
			}
		reg.update ();
		Serial.println ("UPDATE");
		}

	/*If the value has changed, we change it on the screen*/
	if (OLD_PIN != CURRENT_PIN and lcdRedraw){
		lcdRedraw = false;
		lcd.clear ();
		OLD_PIN = CURRENT_PIN;
		lcd.home ();
		lcd.print ("PIN");
		lcd.print (' ');
		lcd.print ('Q');
		lcd.print (CURRENT_PIN);
		lcd.setCursor (0, 1);
		lcd.print ("OF");
		lcd.print (' ');
		lcd.print (ALL_DATA);
		}
	}