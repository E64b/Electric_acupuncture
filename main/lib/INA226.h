#ifndef _INA226_h
#define _INA226_h
#include <Wire.h>
#include <Arduino.h>

/*
�����������:
    INA226 ina226 (������������� �����, ������������ ��������� ���, ����� �� ���� I2c)

    INA226 ina226;						// ����������� �������� ��� ������ INA226 (0.1 ��, 0.8 �, ����� 0x40) - �������� ��� ������ ������
    INA226 ina226 (0x41);				// ���� � ����. ��� �� ���������, ����� 0x41 - �������� ��� ���������� �������
    INA226 ina226 (0.05f);				// ���� 0.05 ��, ����. ��� � ����� �� ��������� (0.8 �, 0x40) - ���������� ������ ��� ����� �/�
    INA226 ina226 (0.05f, 1.5f);		// ���� 0.05 ���, ����. ��������� ��� 1.5 �, ����� �� ��������� (0x40) - ���������� ������ ��� ����� �/�
    INA226 ina226 (0.05f, 1.5f, 0x41);	// ���� 0.05 ���, ����. ��������� ��� 1.5 �, ����� 0x41  - ���������� ������ ��� ����� �/�

������:
    bool begin();							    // ������������� ������ � �������� �����������, ������ false ���� INA226 �� �������
    bool begin(SCL, SDA);						// ������������� ������ ��� ESP8266/ESP32
    void sleep(true / false);				    // ��������� � ���������� ������ ������� �����������������, � ����������� �� ���������
    void setAveraging(avg);					    // ��������� ���������� ���������� ��������� (��. ������� ����)
    void setSampleTime(ch, time);			    // ��������� ������� ������� ���������� � ���� (INA226_VBUS / INA226_VSHUNT), �� ��������� INA226_CONV_1100US

    float getShuntVoltage(); 				    // ��������� ���������� �� �����
    float getVoltage();	 					    // ��������� ����������
    float getCurrent();  					    // ��������� ���
    float getPower(); 						    // ��������� ��������

    uint16_t getCalibration();	 			    // ��������� ������������� �������� (����� ������ �������������� �������������)
    void setCalibration(calibration value);	    // �������� ������������� �������� 	(����� ������� ��� � EEPROM)
    void adjCalibration(calibration offset);    // ���������� ������������� �������� �� ��������� �������� (����� ������ �� ����)

    ������ 1.0 �� 31.10.2021
*/

/* Public-����������� (���������) */
#define INA226_VBUS           true    // ����� ���, ���������� ���������� ���� (0-36�)
#define INA226_VSHUNT         false   // ����� ���, ���������� ���������� �� �����

#define INA226_CONV_140US     0b000   // ����� ������� (���������� ������� ��� ���������)
#define INA226_CONV_204US     0b001
#define INA226_CONV_332US     0b010
#define INA226_CONV_588US     0b011
#define INA226_CONV_1100US    0b100
#define INA226_CONV_2116US    0b101
#define INA226_CONV_4156US    0b110
#define INA226_CONV_8244US    0b111

#define INA226_AVG_X1         0b000   // ���������� ���������� (��������������� ����������� ����� ���������)
#define INA226_AVG_X4         0b001
#define INA226_AVG_X16        0b010
#define INA226_AVG_X64        0b011
#define INA226_AVG_X128       0b100
#define INA226_AVG_X256       0b101
#define INA226_AVG_X512       0b110
#define INA226_AVG_X1024      0b111

/* Private-����������� (������) */
#define INA226_CFG_REG_ADDR   0x00
#define INA226_SHUNT_REG_ADDR 0x01
#define INA226_VBUS_REG_ADDR  0x02
#define INA226_POWER_REG_ADDR 0x03
#define INA226_CUR_REG_ADDR   0x04
#define INA226_CAL_REG_ADDR   0x05

class INA226{
    public:
    INA226(const float r_shunt=0.1f, const float i_max=0.8f, const uint8_t address=0x40)
        : _r_shunt(r_shunt), _i_max(i_max), _iic_address(address){}

    INA226(const uint8_t address)
        : _r_shunt(0.1f), _i_max(0.8f), _iic_address(address){}



    // ������������� � ��������
    bool begin(int __attribute__((unused)) sda=0, int __attribute__((unused)) scl=0){
        #if defined(ESP32) || defined (ESP8266)
        if(sda||scl) Wire.begin(sda, scl);	// ������������� ��� ESP
        else Wire.begin();
        #else
        Wire.begin();                           // ������������� ���� I2c		
        #endif		
        if(!testConnection()) return false;    // �������� �����������
        calibrate();                            // ������ �������������� �������� � �������������
        return true;                            // ������� true ���� ��� ��
        }

    // ��������� / ������ ������ ���
    void sleep(bool state){
        uint16_t cfg_register=readRegister(INA226_CFG_REG_ADDR)&~(0b111);       // ��������� ����. ������� � ������� ���� ������
        writeRegister(INA226_CFG_REG_ADDR, cfg_register|(state?0b000:0b111)); // �������� ����� �������� ����. �������� � ��������� �������
        }

    // ���������� �������������� ��������
    void adjCalibration(int16_t adj){
        setCalibration(getCalibration()+adj);     // ������ � ������������ ��������
        _cal_value=_cal_value+adj;              // ��������� ���������� ����������
        }

    // ��������� �������������� ��������
    void setCalibration(uint16_t cal){
        writeRegister(INA226_CAL_REG_ADDR, cal);    // ����� �������� � ������� ����������
        _cal_value=cal;                           // ��������� ���������� ����������
        }

    // ������ �������������� ��������
    uint16_t getCalibration(void){
        _cal_value=readRegister(INA226_CAL_REG_ADDR);     // ��������� ���������� ���������
        return _cal_value;                                  // ���������� ��������
        }

    // ��������� ����������� ���������� �������
    void setAveraging(uint8_t avg){
        uint16_t cfg_register=readRegister(INA226_CFG_REG_ADDR)&~(0b111<<9);  // ������ ����. �������, ������� ���� AVG2-0
        writeRegister(INA226_CFG_REG_ADDR, cfg_register|avg<<9);                // ����� ����� �������� ����. �������
        }

    // ��������� ���������� ��� ���������� ������
    void setSampleTime(bool ch, uint8_t mode){
        uint16_t cfg_register=readRegister(INA226_CFG_REG_ADDR);  // ������ ����. �������
        cfg_register&=~((0b111)<<(ch?6:3));                // ���������� ������ ����� ���, � ����������� �� ������
        cfg_register|=mode<<(ch?6:3);                       // ����� ������ ����� ���, � ����������� �� ������
        writeRegister(INA226_CFG_REG_ADDR, cfg_register);           // ����� ����� �������� ����. ��������
        }

    // ������ ���������� �� �����
    float getShuntVoltage(void){
        setCalibration(_cal_value);                             // �������������� ���������� ���������� (�� ������ ���������� ������ INA219)
        int16_t value=readRegister(INA226_SHUNT_REG_ADDR);    // ������ �������� ���������� �����
        return value*0.0000025f;                              // LSB = 2.5uV = 0.0000025V, �������� � ����������
        }

    // ������ ����������
    float getVoltage(void){
        uint16_t value=readRegister(INA226_VBUS_REG_ADDR);    // ������ �������� ����������
        return value*0.00125f;                                // LSB = 1.25mV = 0.00125V, �������� �������� �� 12 ��� � ��������
        }

    // ������ ����
    float getCurrent(void){
        setCalibration(_cal_value);                             // �������������� ���������� ���������� (�� ������ ���������� ������ INA219)
        int16_t value=readRegister(INA226_CUR_REG_ADDR);      // ������ �������� ����
        return value*_current_lsb;                            // LSB �������������� �� ������ ����. ���������� ����, �������� � ����������
        }

    // ������ ��������
    float getPower(void){
        setCalibration(_cal_value);                             // �������������� ���������� ���������� (�� ������ ���������� ������ INA219)
        uint16_t value=readRegister(INA226_POWER_REG_ADDR);   // ������ �������� ��������
        return value*_power_lsb;                              // LSB � 25 ��� ������ LSB ��� ����, �������� ����������
        }

    private:
    const uint8_t _iic_address=0x00;  // ����� �� ���� I2c
    const float _r_shunt=0.0;         // ������������� �����
    const float _i_max=0.0;           // ����. ��������� ���

    float _current_lsb=0.0;           // LSB ��� ����
    float _power_lsb=0.0;             // LSB ��� ��������
    uint16_t _cal_value=0;            // ������������� ��������

    // ������ 16-�� ������� �������� INA219
    void writeRegister(uint8_t address, uint16_t data){
        Wire.beginTransmission(_iic_address);   // �������� ��������
        Wire.write(address);                    // ���������� �����
        Wire.write(highByte(data));             // ���������� ������� ����
        Wire.write(lowByte(data));              // ���������� ������� ����
        Wire.endTransmission();                 // ����������� ��������
        }

    // ������ 16-�� ������� �������� INA219
    uint16_t readRegister(uint8_t address){
        Wire.beginTransmission(_iic_address);   // �������� ��������
        Wire.write(address);                    // ���������� �����
        Wire.endTransmission();                 // ����������� ��������
        Wire.requestFrom(_iic_address, (uint8_t)2); // ����������� 2 �����
        return Wire.read()<<8|Wire.read();  // ����� � ���������� ���������
        }

    // �������� �����������
    bool testConnection(void){
        Wire.beginTransmission(_iic_address);   // �������� ��������
        return (bool)!Wire.endTransmission();   // ����� �����������, ����������� ���������
        }

    // ��������� ������� �������������� �������� � �������������
    void calibrate(void){
        writeRegister(INA226_CFG_REG_ADDR, 0x8000);                 // �������������� �����

        _current_lsb=_i_max/32768.0f;                           // ������ LSB ��� ���� (��. ���� INA219)
        _power_lsb=_current_lsb*25.0f;                          // ������ LSB ��� �������� (��. ���� INA219)
        _cal_value=trunc(0.00512f/(_current_lsb*_r_shunt));   // ������ �������������� �������� (��. ���� INA219)

        setCalibration(_cal_value);                                 // ���������� ����������� ������������� ��������
        }
    };
#endif
