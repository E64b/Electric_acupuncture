#pragma once
#pragma pack(1, push)

#include "./lib/EncButton.h"
#include "./lib/GyverHC595.h"
#include "./lib/GyverINA.h"
#include "./lib/LiquidCrystal_I2C.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <Print.h>
#include <Wire.h>

#define STCP 10 // pinCS 12
#define DS 11   // pinDT 14
#define SHCP 13 // pinCLK 11

#define SHIFTS 6 // Number of chips
#define HC595_DELAY 0
#define ONE_SHIFT_DATA 8
#define ALL_DATA (SHIFTS * ONE_SHIFT_DATA)

typedef struct {
  bool out[ALL_DATA]{};
  uint32_t timer = 0;
  uint32_t time = 0;
  uint16_t i = 0;
  float Voltage;
  float OldVoltage;
  float Current;
  float OldCurrent;
  float MaxCurrent;
  bool StartMenu = true;
  bool Work = false;
  bool Send = false;
  bool Settings = false;
  bool WorkMenu = false;
  uint8_t Display;
  uint8_t Program;  
  bool DisplayRedraw;
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

#pragma pack(pop)