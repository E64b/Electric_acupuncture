#pragma once

#include "main.h"
/*
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
*/
const uint16_t *GetProg(uint16_t data.program) {
  switch (data.program) {
  /* case 3:
    return _3;
  case 4:
    return _4;
  case 5:
    return _5;
  case 6:
    return _6;
  case 7:
    return _7;
  case 8:
    return _8;
  case 9:
    return _9;
  case 10:
    return _10;
  case 11:
    return _11;
  case 12:
    return _12;
  case 13:
    return _13;
  case 14:
    return _14;
  case 15:
    return _15;
  case 16:
    return _16;
  case 17:
    return _17;
  case 18:
    return _18;
  case 19:
    return _19;
  case 20:
    return _20;
  case 21:
    return _21;
  case 22:
    return _22
    */
  default:
    return NULL;
  }
}