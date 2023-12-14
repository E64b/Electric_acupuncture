#pragma once
#pragma pack(1, push)

#include <Arduino.h>
#include <EEPROM.h>
#include <Print.h>
#include <Wire.h>
#include "EncButton.h"
#include "GyverHC595.h"
#include "INA219.h"
#include "LiquidCrystal_I2C.h"

#define STCP 10 // pinCS 12
#define DS 11   // pinDT 14
#define SHCP 13 // pinCLK 11

#define SHIFTS 10 // Number of chips
#define HC595_DELAY 0
#define ONE_SHIFT_DATA 8
#define ALL_DATA (SHIFTS * ONE_SHIFT_DATA)

typedef struct {
  bool out[ALL_DATA]{};

  uint32_t timer = 0;
  uint32_t time = 0;
  
  float voltage;
  float oldVoltage;
  float current;
  float oldCurrent;
  float maxCurrent;

  bool displayRedraw = true;
  bool startMenu = true;
  bool work = false;
  bool send = false;
  bool settings = false;
  bool workMenu = false;

  uint8_t display;
  uint8_t program;
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