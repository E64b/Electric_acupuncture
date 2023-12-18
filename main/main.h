#pragma once
#pragma pack(1, push)

#include "./prog/1.h"
#include "./prog/2.h"
#include "lib/EncButton.h"
#include "lib/HC595.h"
#include "lib/INA219.h"
#include "lib/LiquidCrystal_I2C.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <Print.h>
#include <Wire.h>
#include <inttypes.h>
#include <stdio.h>

#define STCP 10 // pinCS 12
#define DS 11   // pinDT 14
#define SHCP 13 // pinCLK 11

#define SHIFTS 10 // Number of chips
#define HC595_DELAY 0
#define ONE_SHIFT_DATA 8
#define ALL_DATA (SHIFTS * ONE_SHIFT_DATA)

/*SETUP*/
#define TIME_TO_STEP 60 // Step time for 3-22 program in sec

struct Memory {
  bool test_mem = false;
  uint16_t _1[64];
  uint16_t _2[32];
};

typedef struct {
  bool out[ALL_DATA]{};
  uint16_t time[ALL_DATA]{};

  uint32_t timer = 0;
  uint8_t time = 0;
  uint16_t i = 0;

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

  uint32_t timeStep = TIME_TO_STEP * 1000;
  uint8_t display = 0;
  uint8_t program = 0;
} Data;

extern EncButton eb;
extern INA219 ina;
extern LiquidCrystal_I2C lcd;
extern HC595<SHIFTS, HC_PINS> reg;
extern Data data;
extern Memory mem;

void display();
void setting();
void sensor();
void work();
void enc();

#pragma pack(pop)