#pragma once

#include <Arduino.h>

#include "VirtEncoder.h"
#include "utils.h"

class Encoder : public VirtEncoder {
public:
  Encoder(uint8_t encA = 0, uint8_t encB = 0, uint8_t mode = INPUT) {
    init(encA, encB, mode);
  }

  void init(uint8_t encA = 0, uint8_t encB = 0, uint8_t mode = INPUT) {
    e0 = encA;
    e1 = encB;
    pinMode(e0, mode);
    pinMode(e1, mode);
    initEnc(readEnc());
  }

  int8_t tickISR() { return VirtEncoder::tickISR(readEnc()); }

  int8_t tick() {
    if (read_ef(EB_EISR))
      return VirtEncoder::tick();
    else
      return VirtEncoder::tick(readEnc());
  }

  int8_t tickRaw() {
    if (read_ef(EB_EISR))
      return VirtEncoder::tickRaw();
    else
      return VirtEncoder::tickRaw(readEnc());
  }

private:
  uint8_t e0, e1;

  int8_t readEnc() { return EBread(e0) | (EBread(e1) << 1); }
};

template <uint8_t ENCA, uint8_t ENCB> class EncoderT : public VirtEncoder {
public:
  EncoderT(uint8_t mode = INPUT) { init(mode); }

  void init(uint8_t mode = INPUT) {
    pinMode(ENCA, mode);
    pinMode(ENCB, mode);
    initEnc(readEnc());
  }

  int8_t tickISR() { return VirtEncoder::tickISR(readEnc()); }

  int8_t tick() {
    if (read_ef(EB_EISR))
      return VirtEncoder::tick();
    else
      return VirtEncoder::tick(readEnc());
  }

  int8_t tickRaw() {
    if (read_ef(EB_EISR))
      return VirtEncoder::tickRaw();
    else
      return VirtEncoder::tickRaw(readEnc());
  }

  int8_t readEnc() { return EBread(ENCA) | (EBread(ENCB) << 1); }

private:
};