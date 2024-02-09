#include <Arduino.h>
#include <EEPROM.h>
#include <Print.h>
#include <SPI.h>
#include <Wire.h>
#include <inttypes.h>
#include <stdio.h>

#include "./lib/Enc.h"
#include "./lib/HC595.h"
#include "./lib/INA219.h"
#include "./lib/LiquidCrystal_I2C.h"

#define STCP 10 // pinCS 12
#define DS 11   // pinDT 14
#define SHCP 13 // pinCLK 11

#define SHIFTS 8 // Number of chips
#define HC595_DELAY 0
#define ONE_SHIFT_DATA 8
#define ALL_DATA (SHIFTS * ONE_SHIFT_DATA)

/*================ defines ==============*/

#define SETTING_BEFORE_START 1
#define SETTING_PROGRAM 2
#define SETTING_IN_WORK 3
#define WORK 4
#define ERROR 255

/*SETUP*/

#define BACKLIGHT true   // lcd backlight true or false
#define MAX_AMPERAGE 500 // Max current uA
#define COEFFICIENT 1000 // current COEFFICIENT default 1000

/*END SETUP*/

struct DataMemory {
  /*First init, DON`T TOUCH*/
  bool testMem = false;
  /*First init, DON`T TOUCH*/
  uint8_t _1[64] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};
  /*First init, DON`T TOUCH*/
  uint8_t _2[32] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                    60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                    60, 60, 60, 60, 60, 60, 60, 60, 60, 60};
  /*First init, DON`T TOUCH*/
  uint8_t setTimeToStep = 60;
  /*First init, DON`T TOUCH*/
  uint8_t stepTime[20] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
                          60, 60, 60, 60, 60, 60, 60, 60, 60, 60};
};

#pragma pack(push, 1)

typedef struct {
  uint32_t timer = 0;
  uint16_t i = 0;
  uint8_t step;
  uint8_t currentState = SETTING_BEFORE_START;
  uint32_t currentMillis = 0;
  uint32_t timeStep;
  uint16_t oldExit;

  float voltage;
  float oldVoltage;
  float oldAmperage;
  float maxAmperage = MAX_AMPERAGE;
  uint16_t amperage_uA;

  bool displayRedraw = true;
  bool work = false;
  bool send = false;
  bool error = false;
  bool settingsChanged = false;
  uint8_t program = 1;

} Data;

extern EncButton enc;
extern Adafruit_INA219 ina;
extern LiquidCrystal_I2C lcd;
extern HC595<SHIFTS, HC_PINS> reg;
extern Data data;
extern DataMemory DataMem;
extern const uint16_t *GetProg(uint8_t prog);

void lcdDisplay();
void sensor();
void work();
void menu();
void sendVal();
void protection();

#pragma pack(pop)