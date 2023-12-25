#pragma once

#include <Arduino.h>

#include "VirtButton.h"
#include "VirtEncoder.h"
#include "utils.h"

#ifdef EB_FAST_TIME
#define EB_FAST_T (EB_FAST_TIME)
#endif

class VirtEncButton : public VirtButton, public VirtEncoder {
public:
  void setFastTimeout(uint8_t tout) {
#ifndef EB_FAST_TIME
    EB_FAST_T = tout;
#endif
  }

  void clear() {
    VirtButton::clear();
    VirtEncoder::clear();
  }

  bool turnH() { return turn() && read_bf(EB_EHLD); }

  bool fast() { return read_ef(EB_FAST); }

  bool right() { return read_ef(EB_DIR) && turn() && !read_bf(EB_EHLD); }

  bool left() { return !read_ef(EB_DIR) && turn() && !read_bf(EB_EHLD); }

  bool rightH() { return read_ef(EB_DIR) && turnH(); }

  bool leftH() { return !read_ef(EB_DIR) && turnH(); }

  bool encHolding() { return read_bf(EB_EHLD); }

  uint16_t action() {
    if (turn())
      return EB_TURN;
    else
      return VirtButton::action();
  }

  int8_t tickISR(bool e0, bool e1) { return tickISR(e0 | (e1 << 1)); }

  int8_t tickISR(int8_t state) {
    state = VirtEncoder::pollEnc(state);
    if (state) {
#ifdef EB_NO_BUFFER
      set_ef(EB_ISR_F);
      write_ef(EB_DIR, state > 0);
      write_ef(EB_FAST, checkFast());
#else
      for (uint8_t i = 0; i < 15; i += 3) {
        if (!(ebuffer & (1 << i))) {
          ebuffer |= (1 << i); 
          if (state > 0)
            ebuffer |= (1 << (i + 1)); 
          if (checkFast())
            ebuffer |= (1 << (i + 2));
          break;
        }
      }
#endif
    }
    return state;
  }

  bool tick(bool e0, bool e1, bool btn) { return tick(e0 | (e1 << 1), btn); }

  bool tick(int8_t state, bool btn) {
    clear();
    bool f = tickRaw(state, btn);

#ifndef EB_NO_CALLBACK
    if (cb && f)
      cb();
#endif
    return f;
  }

  bool tick(bool btn) { return tick(-1, btn); }

  bool tickRaw(bool e0, bool e1, bool btn) {
    return tickRaw(e0 | (e1 << 1), btn);
  }

  bool tickRaw(int8_t state, bool btn) {
    btn = VirtButton::tickRaw(btn);

    bool encf = 0;
#ifdef EB_NO_BUFFER
    if (read_ef(EB_ISR_F)) {
      clr_ef(EB_ISR_F);
      encf = 1;
    }
#else
    if (ebuffer) {
      write_ef(EB_DIR, ebuffer & 0b10);
      write_ef(EB_FAST, ebuffer & 0b100);
      ebuffer >>= 3;
      encf = 1;
    }
#endif
    else if ((state >= 0) && (state = VirtEncoder::pollEnc(state))) {
      write_ef(EB_DIR, state > 0);
      write_ef(EB_FAST, checkFast());
      encf = 1;
    }
    if (encf) {
      if (read_bf(EB_PRS))
        set_bf(EB_EHLD);
      else
        clicks = 0;
      if (!read_bf(EB_TOUT))
        set_bf(EB_TOUT);
      set_ef(EB_ETRN_R);
    }
    return encf | btn;
  }

  bool tickRaw(bool btn) { return tickRaw(-1, btn); }

protected:
#ifndef EB_FAST_TIME
  uint8_t EB_FAST_T = 30;
#endif

#ifndef EB_NO_BUFFER
  uint16_t ebuffer = 0;
#endif

private:
  bool checkFast() {
    uint16_t ms = EB_UPTIME();
    bool f = 0;
    if (ms - tmr < EB_FAST_T)
      f = 1;
    tmr = ms;
    return f;
  }
};