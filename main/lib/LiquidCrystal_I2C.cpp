
#include "LiquidCrystal_I2C.h"
#include <inttypes.h>
#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#define printIIC(args) Wire.write(args)
inline size_t LiquidCrystal_I2C::write(uint8_t value) {
  send(value, Rs);
  return 1;
}

#else
#include "WProgram.h"

#define printIIC(args) Wire.send(args)
inline void LiquidCrystal_I2C::write(uint8_t value) { send(value, Rs); }

#endif
#include "Wire.h"

LiquidCrystal_I2C::LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t lcd_cols,
                                     uint8_t lcd_rows) {
  _Addr = lcd_Addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _backlightval = LCD_NOBACKLIGHT;
}

void LiquidCrystal_I2C::oled_init() {
  _oled = true;
  init_priv();
}

void LiquidCrystal_I2C::init() { init_priv(); }

void LiquidCrystal_I2C::init_priv() {
  Wire.begin();
  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  begin(_cols, _rows);
}

void LiquidCrystal_I2C::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  if ((dotsize != 0) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  delay(50);

  expanderWrite(_backlightval);
  delay(1000);

  write4bits(0x03 << 4);
  delayMicroseconds(4500);
  write4bits(0x03 << 4);
  delayMicroseconds(4500);
  write4bits(0x03 << 4);
  delayMicroseconds(150);

  write4bits(0x02 << 4);

  command(LCD_FUNCTIONSET | _displayfunction);

  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  display();

  clear();

  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

  command(LCD_ENTRYMODESET | _displaymode);

  home();
}

void LiquidCrystal_I2C::clear() {
  command(LCD_CLEARDISPLAY);
  delayMicroseconds(2000);
  if (_oled)
    setCursor(0, 0);
}

void LiquidCrystal_I2C::home() {
  command(LCD_RETURNHOME);
  delayMicroseconds(2000);
}

void LiquidCrystal_I2C::setCursor(uint8_t col, uint8_t row) {
  int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  if (row > _numlines) {
    row = _numlines - 1;
  }
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LiquidCrystal_I2C::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal_I2C::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_I2C::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LiquidCrystal_I2C::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal_I2C::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7;
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++) {
    write(charmap[i]);
  }
}

void LiquidCrystal_I2C::createChar(uint8_t location, const char *charmap) {
  location &= 0x7;
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++) {
    write(pgm_read_byte_near(charmap++));
  }
}

void LiquidCrystal_I2C::noBacklight(void) {
  _backlightval = LCD_NOBACKLIGHT;
  expanderWrite(0);
}

void LiquidCrystal_I2C::backlight(void) {
  _backlightval = LCD_BACKLIGHT;
  expanderWrite(0);
}

inline void LiquidCrystal_I2C::command(uint8_t value) { send(value, 0); }

void LiquidCrystal_I2C::send(uint8_t value, uint8_t mode) {
  uint8_t highnib = value & 0xf0;
  uint8_t lownib = (value << 4) & 0xf0;
  write4bits((highnib) | mode);
  write4bits((lownib) | mode);
}

void LiquidCrystal_I2C::write4bits(uint8_t value) {
  expanderWrite(value);
  pulseEnable(value);
}

void LiquidCrystal_I2C::expanderWrite(uint8_t _data) {
  Wire.beginTransmission(_Addr);
  printIIC((int)(_data) | _backlightval);
  Wire.endTransmission();
}

void LiquidCrystal_I2C::pulseEnable(uint8_t _data) {
  expanderWrite(_data | En);
  delayMicroseconds(1);

  expanderWrite(_data & ~En);
  delayMicroseconds(50);
}

void LiquidCrystal_I2C::cursor_on() { cursor(); }

void LiquidCrystal_I2C::cursor_off() { noCursor(); }

void LiquidCrystal_I2C::blink_on() { blink(); }

void LiquidCrystal_I2C::blink_off() { noBlink(); }

void LiquidCrystal_I2C::load_custom_character(uint8_t char_num, uint8_t *rows) {
  createChar(char_num, rows);
}

void LiquidCrystal_I2C::setBacklight(uint8_t new_val) {
  if (new_val) {
    backlight();
  } else {
    noBacklight();
  }
}

void LiquidCrystal_I2C::printstr(const char c[]) { print(c); }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void LiquidCrystal_I2C::off() {}
void LiquidCrystal_I2C::on() {}
void LiquidCrystal_I2C::setDelay(int cmdDelay, int charDelay) {}
uint8_t LiquidCrystal_I2C::status() { return 0; }
uint8_t LiquidCrystal_I2C::keypad() { return 0; }
uint8_t LiquidCrystal_I2C::init_bargraph(uint8_t graphtype) { return 0; }
void LiquidCrystal_I2C::draw_horizontal_graph(uint8_t row, uint8_t column,
                                              uint8_t len,
                                              uint8_t pixel_col_end) {}
void LiquidCrystal_I2C::draw_vertical_graph(uint8_t row, uint8_t column,
                                            uint8_t len,
                                            uint8_t pixel_row_end) {}
void LiquidCrystal_I2C::setContrast(uint8_t new_val) {}
#pragma GCC diagnostic pop
