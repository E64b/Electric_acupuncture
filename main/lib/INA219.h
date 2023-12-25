#ifndef _INA219_h
#define _INA219_h

#include <Arduino.h>
#include <Wire.h>

#define INA219_VBUS true
#define INA219_VSHUNT false
#define INA219_RES_9BIT 0b0000
#define INA219_RES_10BIT 0b0001
#define INA219_RES_11BIT 0b0010
#define INA219_RES_12BIT 0b0011
#define INA219_RES_12BIT_X2 0b1001
#define INA219_RES_12BIT_X4 0b1010
#define INA219_RES_12BIT_X8 0b1011
#define INA219_RES_12BIT_X16 0b1100
#define INA219_RES_12BIT_X32 0b1101
#define INA219_RES_12BIT_X64 0b1110
#define INA219_RES_12BIT_X128 0b1111

#define INA219_CFG_REG_ADDR 0x00
#define INA219_SHUNT_REG_ADDR 0x01
#define INA219_VBUS_REG_ADDR 0x02
#define INA219_POWER_REG_ADDR 0x03
#define INA219_CUR_REG_ADDR 0x04
#define INA219_CAL_REG_ADDR 0x05

class INA219 {
public:
  INA219(const float r_shunt = 0.1f, const float i_max = 3.2f,
         const uint8_t address = 0x40)
      : _r_shunt(r_shunt), _i_max(i_max), _iic_address(address) {}

  INA219(const uint8_t address)
      : _r_shunt(0.1f), _i_max(3.2f), _iic_address(address) {}

  bool begin(int __attribute__((unused)) sda = 0,
             int __attribute__((unused)) scl = 0) {
#if defined(ESP32) || defined(ESP8266)
    if (sda || scl)
      Wire.begin(sda, scl);
    else
      Wire.begin();
#else
    Wire.begin();
#endif
    if (!testConnection())
      return false;
    calibrate();
    return true;
  }

  void sleep(bool state) {
    uint16_t cfg_register = readRegister(INA219_CFG_REG_ADDR) & ~(0b111);
    writeRegister(INA219_CFG_REG_ADDR, cfg_register | (state ? 0b000 : 0b111));
  }

  void adjCalibration(int16_t adj) {
    setCalibration(getCalibration() + adj);
    _cal_value = _cal_value + adj;
  }

  void setCalibration(uint16_t cal) {
    writeRegister(INA219_CAL_REG_ADDR, cal);
    _cal_value = cal;
  }

  uint16_t getCalibration(void) {
    _cal_value = readRegister(INA219_CAL_REG_ADDR);
    return _cal_value;
  }

  void setResolution(bool ch, uint8_t mode) {
    uint16_t cfg_register = readRegister(INA219_CFG_REG_ADDR);
    cfg_register &= ~((0b1111) << (ch ? 7 : 3));
    cfg_register |= mode << (ch ? 7 : 3);
    writeRegister(INA219_CFG_REG_ADDR, cfg_register);
  }

  float getShuntVoltage(void) {
    setCalibration(_cal_value);

    int16_t value = readRegister(INA219_SHUNT_REG_ADDR);
    return value * 0.00001f;
  }

  float getVoltage(void) {
    uint16_t value = readRegister(INA219_VBUS_REG_ADDR);
    return (value >> 3) * 0.004f;
  }

  float getCurrent(void) {
    setCalibration(_cal_value);

    int16_t value = readRegister(INA219_CUR_REG_ADDR);
    return value * _current_lsb;
  }

  float getPower(void) {
    setCalibration(_cal_value);

    uint16_t value = readRegister(INA219_POWER_REG_ADDR);
    return value * _power_lsb;
  }

private:
  const uint8_t _iic_address = 0x00;
  const float _r_shunt = 0.0;
  const float _i_max = 0.0;

  float _current_lsb = 0.0;
  float _power_lsb = 0.0;
  uint16_t _cal_value = 0;

  void writeRegister(uint8_t address, uint16_t data) {
    Wire.beginTransmission(_iic_address);
    Wire.write(address);
    Wire.write(highByte(data));
    Wire.write(lowByte(data));
    Wire.endTransmission();
  }

  uint16_t readRegister(uint8_t address) {
    Wire.beginTransmission(_iic_address);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(_iic_address, (uint8_t)2);
    return Wire.read() << 8 | Wire.read();
  }

  bool testConnection(void) {
    Wire.beginTransmission(_iic_address);
    return (bool)!Wire.endTransmission();
  }

  void calibrate(void) {
    writeRegister(INA219_CFG_REG_ADDR, 0x8000);

    _current_lsb = _i_max / 32768.0f;
    _power_lsb = _current_lsb * 20.0f;
    _cal_value = trunc(0.04096f / (_current_lsb * _r_shunt));

    setCalibration(_cal_value);

    uint16_t cfg_register = readRegister(INA219_CFG_REG_ADDR) & ~(0b11 << 11);

    uint16_t vshunt_max_mv = (_r_shunt * _i_max) * 1000;

    if (vshunt_max_mv <= 40)
      cfg_register |= 0b00 << 11;
    else if (vshunt_max_mv <= 80)
      cfg_register |= 0b01 << 11;
    else if (vshunt_max_mv <= 160)
      cfg_register |= 0b10 << 11;
    else
      cfg_register |= 0b11 << 11;
    writeRegister(INA219_CFG_REG_ADDR, cfg_register);
  }
};
#endif