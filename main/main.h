#pragma once

#include "lib/GyverHC595.h"
#include "lib/GyverINA.h"
#include "lib/LiquidCrystal_I2C.h"
#include "lib/EncButton.h"
#include <Arduino.h>
#include <Wire.h>

#define STCP 10 //pinCS 12
#define DS 11 //pinDT 14
#define SHCP 13 //pinCLK 11

#define SHIFTS 1 //Number of chips
#define HC595_DELAY 0
#define ONE_SHIFT_DATA 8
#define ALL_DATA (SHIFTS * ONE_SHIFT_DATA)

typedef struct{
	bool VAL[ALL_DATA]{};
	uint16_t i=0;
	float Voltage;
	float Current;
	bool StartMenu;
	bool Work;
	uint8_t Setting;
	bool WorkMenu;
	} Data;

extern EncButton eb;
extern INA219 ina;
extern LiquidCrystal_I2C lcd;
extern GyverHC595<SHIFTS, HC_PINS> reg;
extern Data data;

void display();
void setting();
void sensor();
void work();
void enc();