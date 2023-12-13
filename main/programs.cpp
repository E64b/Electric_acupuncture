#include "main.h"
#include "./prog/1.h"
#include "./prog/2.h"
#include "./prog/3.h"
#include "./prog/4.h"
#include "./prog/5.h"
#include "./prog/6.h"
#include "./prog/7.h"
#include "./prog/8.h"
#include "./prog/9.h"
#include "./prog/10.h"
#include "./prog/11.h"
#include "./prog/12.h"
#include "./prog/13.h"
#include "./prog/14.h"
#include "./prog/15.h"
#include "./prog/16.h"
#include "./prog/17.h"
#include "./prog/18.h"
#include "./prog/19.h"
#include "./prog/20.h"
#include "./prog/21.h"
#include "./prog/22.h"

uint8_t *GetProg(uint8_t data.program) {
  switch (data.program) {
  case 1:
    return (uint8_t *)_1;
  case 2:
    return (uint8_t *)_2;
  case 3:
    return (uint8_t *)_3;
  case 4:
    return (uint8_t *)_4;
  case 5:
    return (uint8_t *)_5;
  case 6:
    return (uint8_t *)_6;
  case 7:
    return (uint8_t *)_7;
  case 8:
    return (uint8_t *)_8;
  case 9:
    return (uint8_t *)_9;
  case 10:
    return (uint8_t *)_10;
  case 11:
    return (uint8_t *)_11;
  case 12:
    return (uint8_t *)_12;
  case 13:
    return (uint8_t *)_13;
  case 14:
    return (uint8_t *)_14;
  case 15:
    return (uint8_t *)_15;
  case 16:
    return (uint8_t *)_16;
  case 17:
    return (uint8_t *)_17;
  case 18:
    return (uint8_t *)_18;
  case 19:
    return (uint8_t *)_19;
  case 20:
    return (uint8_t *)_20;
  case 21:
    return (uint8_t *)_21;
  case 22:
    return (uint8_t *)_22;
  default:
    return NULL;
  }
}