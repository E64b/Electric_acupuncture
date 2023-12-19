#include <Arduino.h>
#include <EEPROM.h>
#include <Print.h>
#include <Wire.h>
#include <inttypes.h>
#include <stdio.h>

// #include "./prog/1.h"
// #include "./prog/2.h"
#include "./lib/EncButton.h"
#include "./lib/HC595.h"
#include "./lib/INA219.h"
#include "./lib/LiquidCrystal_I2C.h"

#define STCP 10 // pinCS 12
#define DS 11   // pinDT 14
#define SHCP 13 // pinCLK 11

#define SHIFTS 10 // Number of chips
#define HC595_DELAY 0
#define ONE_SHIFT_DATA 8
#define ALL_DATA (SHIFTS * ONE_SHIFT_DATA)

/*SETUP*/
#define TIME_TO_STEP 60 // Step time for 3-22 program in sec

//#pragma pack(1, push)

struct DataMemory {
  bool test_mem = false;
  uint16_t _1[64] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};

  uint16_t _2[32] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60};
};

typedef struct {
  bool out[ALL_DATA]{};
  uint16_t time[ALL_DATA]{};

  uint32_t timer = 0;
  uint8_t setTime;
  uint16_t i = 0;
  uint8_t step;

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
  uint8_t program = 1;
} Data;

//#pragma pack(pop)

extern EncButton eb;
extern INA219 ina;
extern LiquidCrystal_I2C lcd;
extern HC595<SHIFTS, HC_PINS> reg;
extern Data data;
extern DataMemory DataMem;

void display();
void sensor();
void work();

void setting();
void send_val();
void startMenu();
