#include <Arduino.h>
#include <inttypes.h>
#include <stdio.h>
#include <Print.h>
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

#include "./lib/HC595.h"
#include "./lib/INA219.h"
#include "./lib/LiquidCrystal_I2C.h"
#include "./lib/Enc.h"

#define STCP 10 // pinCS 12
#define DS 11   // pinDT 14
#define SHCP 13 // pinCLK 11

#define SHIFTS 10 // Number of chips
#define HC595_DELAY 0
#define ONE_SHIFT_DATA 8
#define ALL_DATA (SHIFTS * ONE_SHIFT_DATA)

/*SETUP*/

#define TIME_TO_STEP 60 // Step time for 3-22 program in sec
#define BACKLIGHT true // lcd backlight true or false
#define MAX_AMPERAGE 500 // Max current uA

/*END SETUP*/

struct DataMemory {
  /*First init, DON`T TOUCH*/
  bool testMem = false;
  /*First init, DON`T TOUCH*/
  uint16_t _1[64] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};
  /*First init, DON`T TOUCH*/
  uint16_t _2[32] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                     60, 60, 60, 60, 60, 60, 60, 60, 60, 60};
  /*First init, DON`T TOUCH*/
};

#pragma pack(push, 1)

typedef struct {
  bool out[ALL_DATA]{};
  uint16_t time[ALL_DATA]{};

  uint32_t timer = 0;
  uint8_t setTimeToStep;
  uint16_t i = 0;
  uint8_t step;
  uint8_t currentState = 1;
  uint32_t currentMillis;
  uint32_t timeStep;

  float voltage;
  float oldVoltage;
  float oldAmperage;
  float maxAmperage = MAX_CURRENT;
  float amperage_uA;

  bool displayRedraw = true;
  bool work = false;
  bool send = false;
  bool error = false;

  uint8_t program = 1;
} Data;
 
extern EncButton enc;
extern INA219 ina;
extern LiquidCrystal_I2C lcd;
extern HC595<SHIFTS, HC_PINS> reg;
extern Data data;
extern DataMemory DataMem;
extern const uint8_t *GetProg(uint8_t prog);

void lcdDisplay();
void sensor();
void work();
void menu();
void send_val();
void protection();

/*================ defines ==============*/

#define SETTING_BEFORE_START 1
#define SETTING_PROGRAM 2
#define WORK 3
#define ERROR 10

#pragma pack(pop)