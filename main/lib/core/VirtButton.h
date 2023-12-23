#pragma once
#include <Arduino.h>

#include "utils.h"

#define EB_PRESS (1 << 0)
#define EB_HOLD (1 << 1)
#define EB_STEP (1 << 2)
#define EB_RELEASE (1 << 3)
#define EB_CLICK (1 << 4)
#define EB_CLICKS (1 << 5)
#define EB_TURN (1 << 6)
#define EB_REL_HOLD (1 << 7)
#define EB_REL_HOLD_C (1 << 8)
#define EB_REL_STEP (1 << 9)
#define EB_REL_STEP_C (1 << 10)

#define EB_SHIFT 4

#ifdef EB_DEB_TIME
#define EB_DEB_T (EB_DEB_TIME)
#endif

#ifdef EB_CLICK_TIME
#define EB_CLICK_T (EB_CLICK_TIME)
#endif

#ifdef EB_HOLD_TIME
#define EB_HOLD_T (EB_HOLD_TIME)
#endif

#ifdef EB_STEP_TIME
#define EB_STEP_T (EB_STEP_TIME)
#endif

#define EB_FOR_SCALE 6

#define EB_CLKS_R (1 << 0)
#define EB_PRS_R (1 << 1)
#define EB_HLD_R (1 << 2)
#define EB_STP_R (1 << 3)
#define EB_REL_R (1 << 4)

#define EB_PRS (1 << 5)
#define EB_HLD (1 << 6)
#define EB_STP (1 << 7)
#define EB_REL (1 << 8)

#define EB_BUSY (1 << 9)
#define EB_DEB (1 << 10)
#define EB_TOUT (1 << 11)
#define EB_INV (1 << 12)
#define EB_BOTH (1 << 13)
#define EB_BISR (1 << 14)

#define EB_EHLD (1 << 15)

class VirtButton {
public:
  void setHoldTimeout(uint16_t tout) {
#ifndef EB_HOLD_TIME
    EB_HOLD_T = tout >> EB_SHIFT;
#endif
  }

  void setStepTimeout(uint16_t tout) {
#ifndef EB_STEP_TIME
    EB_STEP_T = tout >> EB_SHIFT;
#endif
  }

  void setClickTimeout(uint16_t tout) {
#ifndef EB_CLICK_TIME
    EB_CLICK_T = tout >> EB_SHIFT;
#endif
  }

  void setDebTimeout(uint8_t tout) {
#ifndef EB_DEB_TIME
    EB_DEB_T = tout;
#endif
  }

  void setBtnLevel(bool level) { write_bf(EB_INV, !level); }

  void pressISR() {
    if (!read_bf(EB_DEB))
      tmr = EB_UPTIME();
    set_bf(EB_DEB | EB_BISR);
  }

  void reset() {
    clicks = 0;
    clr_bf(~EB_INV);
  }

  void clear() {
    if (read_bf(EB_CLKS_R))
      clicks = 0;
    if (read_bf(EB_CLKS_R | EB_STP_R | EB_PRS_R | EB_HLD_R | EB_REL_R)) {
      clr_bf(EB_CLKS_R | EB_STP_R | EB_PRS_R | EB_HLD_R | EB_REL_R);
    }
  }

  void attach(void (*handler)()) {
#ifndef EB_NO_CALLBACK
    cb = *handler;
#endif
  }

  void detach() {
#ifndef EB_NO_CALLBACK
    cb = nullptr;
#endif
  }

  bool press() { return read_bf(EB_PRS_R); }

  bool release() { return eq_bf(EB_REL_R | EB_REL, EB_REL_R | EB_REL); }

  bool click() { return eq_bf(EB_REL_R | EB_REL | EB_HLD, EB_REL_R); }

  bool pressing() { return read_bf(EB_PRS); }

  bool hold() { return read_bf(EB_HLD_R); }

  bool hold(uint8_t num) { return clicks == num && hold(); }

  bool holding() { return eq_bf(EB_PRS | EB_HLD, EB_PRS | EB_HLD); }

  bool holding(uint8_t num) { return clicks == num && holding(); }

  bool step() { return read_bf(EB_STP_R); }

  bool step(uint8_t num) { return clicks == num && step(); }

  bool hasClicks() { return eq_bf(EB_CLKS_R | EB_HLD, EB_CLKS_R); }

  bool hasClicks(uint8_t num) { return clicks == num && hasClicks(); }

  uint8_t getClicks() { return clicks; }

  uint16_t getSteps() {
#ifndef EB_NO_FOR
#ifdef EB_STEP_TIME
    return ftmr ? ((stepFor() + EB_STEP_T - 1) / EB_STEP_T) : 0;
#else
    return ftmr ? ((stepFor() + (EB_STEP_T << EB_SHIFT) - 1) /
                   (EB_STEP_T << EB_SHIFT))
                : 0;
#endif
#endif
    return 0;
  }

  bool releaseHold() {
    return eq_bf(EB_REL_R | EB_REL | EB_HLD | EB_STP, EB_REL_R | EB_HLD);
  }

  bool releaseHold(uint8_t num) {
    return clicks == num &&
           eq_bf(EB_CLKS_R | EB_HLD | EB_STP, EB_CLKS_R | EB_HLD);
  }

  bool releaseStep() {
    return eq_bf(EB_REL_R | EB_REL | EB_STP, EB_REL_R | EB_STP);
  }

  bool releaseStep(uint8_t num) {
    return clicks == num && eq_bf(EB_CLKS_R | EB_STP, EB_CLKS_R | EB_STP);
  }

  bool waiting() { return clicks && eq_bf(EB_PRS | EB_REL, 0); }

  bool busy() { return read_bf(EB_BUSY); }

  uint16_t action() {
    switch (flags & 0b111111111) {
    case (EB_PRS | EB_PRS_R):
      return EB_PRESS;
    case (EB_PRS | EB_HLD | EB_HLD_R):
      return EB_HOLD;
    case (EB_PRS | EB_HLD | EB_STP | EB_STP_R):
      return EB_STEP;
    case (EB_REL | EB_REL_R):
    case (EB_REL | EB_REL_R | EB_HLD):
    case (EB_REL | EB_REL_R | EB_HLD | EB_STP):
      return EB_RELEASE;
    case (EB_REL_R):
      return EB_CLICK;
    case (EB_CLKS_R):
      return EB_CLICKS;
    case (EB_REL_R | EB_HLD):
      return EB_REL_HOLD;
    case (EB_CLKS_R | EB_HLD):
      return EB_REL_HOLD_C;
    case (EB_REL_R | EB_HLD | EB_STP):
      return EB_REL_STEP;
    case (EB_CLKS_R | EB_HLD | EB_STP):
      return EB_REL_STEP_C;
    }
    return 0;
  }

  bool timeout(uint16_t tout) {
    if (read_bf(EB_TOUT) && (uint16_t)((uint16_t)EB_UPTIME() - tmr) > tout) {
      clr_bf(EB_TOUT);
      return 1;
    }
    return 0;
  }

  uint16_t pressFor() {
#ifndef EB_NO_FOR
    if (ftmr)
      return (uint16_t)EB_UPTIME() - ftmr;
#endif
    return 0;
  }

  bool pressFor(uint16_t ms) { return pressFor() > ms; }

  uint16_t holdFor() {
#ifndef EB_NO_FOR
    if (read_bf(EB_HLD)) {
#ifdef EB_HOLD_TIME
      return pressFor() - EB_HOLD_T;
#else
      return pressFor() - (EB_HOLD_T << EB_SHIFT);
#endif
    }
#endif
    return 0;
  }

  bool holdFor(uint16_t ms) { return holdFor() > ms; }

  uint16_t stepFor() {
#ifndef EB_NO_FOR
    if (read_bf(EB_STP)) {
#ifdef EB_HOLD_TIME
      return pressFor() - EB_HOLD_T * 2;
#else
      return pressFor() - (EB_HOLD_T << EB_SHIFT) * 2;
#endif
    }
#endif
    return 0;
  }

  bool stepFor(uint16_t ms) { return stepFor() > ms; }

  bool tick(VirtButton &b0, VirtButton &b1) {
    if (read_bf(EB_BOTH)) {
      if (!b0.pressing() && !b1.pressing())
        clr_bf(EB_BOTH);
      if (!b0.pressing())
        b0.reset();
      if (!b1.pressing())
        b1.reset();
      b0.clear();
      b1.clear();
      return tick(1);
    } else {
      if (b0.pressing() && b1.pressing())
        set_bf(EB_BOTH);
      return tick(0);
    }
  }

  bool tick(bool s) {
    clear();
    s = pollBtn(s);
#ifndef EB_NO_CALLBACK
    if (cb && s)
      cb();
#endif
    return s;
  }

  bool tickRaw(bool s) { return pollBtn(s); }

  uint8_t clicks;

  void setButtonLevel(bool level) { write_bf(EB_INV, !level); }

protected:
  bool pollBtn(bool s) {
    if (read_bf(EB_BISR)) {
      clr_bf(EB_BISR);
      s = 1;
    } else
      s ^= read_bf(EB_INV);

    if (!read_bf(EB_BUSY)) {
      if (s)
        set_bf(EB_BUSY);
      else
        return 0;
    }

    uint16_t ms = EB_UPTIME();
    uint16_t deb = ms - tmr;

    if (s) {
      if (!read_bf(EB_PRS)) {
        if (!read_bf(EB_DEB) && EB_DEB_T) {
          set_bf(EB_DEB);
          tmr = ms;
        } else {
          if (deb >= EB_DEB_T || !EB_DEB_T) {
            set_bf(EB_PRS | EB_PRS_R);
#ifndef EB_NO_FOR
            ftmr = ms;
#endif
            tmr = ms;
          }
        }
      } else {
        if (!read_bf(EB_EHLD)) {
          if (!read_bf(EB_HLD)) {
#ifdef EB_HOLD_TIME
            if (deb >= (uint16_t)EB_HOLD_T) {
#else
            if (deb >= (uint16_t)(EB_HOLD_T << EB_SHIFT)) {
#endif
              set_bf(EB_HLD_R | EB_HLD);
              tmr = ms;
            }
          } else {
#ifdef EB_STEP_TIME
            if (deb >= (uint16_t)(read_bf(EB_STP) ? EB_STEP_T : EB_HOLD_T)) {
#else
            if (deb >= (uint16_t)(read_bf(EB_STP) ? (EB_STEP_T << EB_SHIFT)
                                                  : (EB_HOLD_T << EB_SHIFT))) {
#endif
              set_bf(EB_STP | EB_STP_R);
              tmr = ms;
            }
          }
        }
      }
    } else {
      if (read_bf(EB_PRS)) {
        if (deb >= EB_DEB_T) {
          if (!read_bf(EB_HLD))
            clicks++;
          if (read_bf(EB_EHLD))
            clicks = 0;
          set_bf(EB_REL | EB_REL_R);
          clr_bf(EB_PRS);
        }
      } else if (read_bf(EB_REL)) {
        if (!read_bf(EB_EHLD)) {
          set_bf(EB_REL_R);
        }
        clr_bf(EB_REL | EB_EHLD);
        tmr = ms;
      } else if (clicks) {
#ifdef EB_CLICK_TIME
        if (read_bf(EB_HLD | EB_STP) || deb >= (uint16_t)EB_CLICK_T)
          set_bf(EB_CLKS_R);
#else
        if (read_bf(EB_HLD | EB_STP) ||
            deb >= (uint16_t)(EB_CLICK_T << EB_SHIFT))
          set_bf(EB_CLKS_R);
#endif
#ifndef EB_NO_FOR
        else if (ftmr)
          ftmr = 0;
#endif
      } else if (read_bf(EB_BUSY)) {
        clr_bf(EB_HLD | EB_STP | EB_BUSY);
        set_bf(EB_TOUT);
#ifndef EB_NO_FOR
        ftmr = 0;
#endif
        tmr = ms;
      }
      if (read_bf(EB_DEB))
        clr_bf(EB_DEB);
    }
    return read_bf(EB_CLKS_R | EB_PRS_R | EB_HLD_R | EB_STP_R | EB_REL_R);
  }

  uint16_t tmr = 0;
#ifndef EB_NO_FOR
  uint16_t ftmr = 0;
#endif

#ifndef EB_NO_CALLBACK
  void (*cb)() = nullptr;
#endif

#ifndef EB_DEB_TIME
  uint8_t EB_DEB_T = 50;
#endif
#ifndef EB_CLICK_TIME
  uint8_t EB_CLICK_T = (500 >> EB_SHIFT);
#endif
#ifndef EB_HOLD_TIME
  uint8_t EB_HOLD_T = (600 >> EB_SHIFT);
#endif
#ifndef EB_STEP_TIME
  uint8_t EB_STEP_T = (200 >> EB_SHIFT);
#endif

  inline void set_bf(const uint16_t x) __attribute__((always_inline)) {
    flags |= x;
  }
  inline void clr_bf(const uint16_t x) __attribute__((always_inline)) {
    flags &= ~x;
  }
  inline bool read_bf(const uint16_t x) __attribute__((always_inline)) {
    return flags & x;
  }
  inline void write_bf(const uint16_t x, bool v)
      __attribute__((always_inline)) {
    if (v)
      set_bf(x);
    else
      clr_bf(x);
  }
  inline bool eq_bf(const uint16_t x, const uint16_t y)
      __attribute__((always_inline)) {
    return (flags & x) == y;
  }

private:
  uint16_t flags = 0;
};