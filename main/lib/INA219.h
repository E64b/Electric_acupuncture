#ifndef _INA219_h
#define _INA219_h
#include <Wire.h>
#include <Arduino.h>

/*
�����������:
    INA219 ina219 (������������� �����, ������������ ��������� ���, ����� �� ���� I2c)

    INA219 ina219;						// ����������� �������� ��� ������ INA219 (0.1 ��, 3.2�, ����� 0x40) - �������� ��� ������ ������
    INA219 ina219 (0x41);				// ���� � ����. ��� �� ���������, ����� 0x41 - �������� ��� ���������� �������
    INA219 ina219 (0.05f);				// ���� 0.05 ��, ����. ��� � ����� �� ��������� (3.2�, 0x40) - ���������� ������ ��� ����� �/�
    INA219 ina219 (0.05f, 2.0f);		// ���� 0.05 ���, ����. ��������� ��� 2�, ����� �� ��������� (0x40) - ���������� ������ ��� ����� �/�
    INA219 ina219 (0.05f, 2.0f, 0x41);	// ���� 0.05 ���, ����. ��������� ��� 2�, ����� 0x41  - ���������� ������ ��� ����� �/�

������:
    bool begin();							    // ������������� ������ � �������� �����������, ������ false ���� INA226 �� �������
    bool begin(SCL, SDA);						// ������������� ������ ��� ESP8266/ESP32
    void sleep(true / false);				    // ��������� � ���������� ������ ������� �����������������, � ����������� �� ���������
    void setResolution(channel, mode);		    // ��������� ���������� � ������ ���������� ��� ��������� ���������� � ����

    float getShuntVoltage(); 				    // ��������� ���������� �� �����
    float getVoltage();	 					    // ��������� ����������
    float getCurrent();  					    // ��������� ���
    float getPower(); 						    // ��������� ��������

    uint16_t getCalibration();	 			    // ��������� ������������� �������� (����� ������ �������������� �������������)
    void setCalibration(calibration value);	    // �������� ������������� �������� 	(����� ������� ��� � EEPROM)
    void adjCalibration(calibration offset);    // ���������� ������������� �������� �� ��������� �������� (����� ������ �� ����)
*/

/* Public-����������� (���������) */
#define INA219_VBUS           true		// ����� ���, ���������� ���������� ���� (0-26�)
#define INA219_VSHUNT         false		// ����� ���, ���������� ���������� �� �����
#define INA219_RES_9BIT       0b0000	// 9 ��� - 84���
#define INA219_RES_10BIT      0b0001	// 10 ��� - 148��� 
#define INA219_RES_11BIT      0b0010	// 11 ��� - 276��� 
#define INA219_RES_12BIT      0b0011	// 12 ��� - 532���
#define INA219_RES_12BIT_X2   0b1001	// 12 ���, ������� �� 2� - 1.06 ��
#define INA219_RES_12BIT_X4   0b1010	// 12 ���, ������� �� 4� - 2.13 ��
#define INA219_RES_12BIT_X8   0b1011	// 12 ���, ������� �� 8� - 4.26 ��
#define INA219_RES_12BIT_X16  0b1100	// 12 ���, ������� �� 16� - 8.51 ��
#define INA219_RES_12BIT_X32  0b1101	// 12 ���, ������� �� 32� - 17.02 ��
#define INA219_RES_12BIT_X64  0b1110	// 12 ���, ������� �� 64� - 34.05 ��
#define INA219_RES_12BIT_X128 0b1111	// 12 ���, ������� �� 128� - 68.10 ��

/* Private-����������� (������) */
#define INA219_CFG_REG_ADDR   0x00
#define INA219_SHUNT_REG_ADDR 0x01
#define INA219_VBUS_REG_ADDR  0x02
#define INA219_POWER_REG_ADDR 0x03
#define INA219_CUR_REG_ADDR   0x04
#define INA219_CAL_REG_ADDR   0x05

class INA219{
    public:
    // �����������
    INA219(const float r_shunt=0.1f, const float i_max=3.2f, const uint8_t address=0x40)
        : _r_shunt(r_shunt), _i_max(i_max), _iic_address(address){}

    // �����������
    INA219(const uint8_t address)
        : _r_shunt(0.1f), _i_max(3.2f), _iic_address(address){}

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
        uint16_t cfg_register=readRegister(INA219_CFG_REG_ADDR)&~(0b111);			// ��������� ����. ������� � ������� ���� ������
        writeRegister(INA219_CFG_REG_ADDR, cfg_register|(state?0b000:0b111));	// �������� ����� �������� ����. �������� � ��������� �������
        }

    // ���������� �������������� ��������
    void adjCalibration(int16_t adj){
        setCalibration(getCalibration()+adj);     // ������ � ������������ ��������
        _cal_value=_cal_value+adj;              // ��������� ���������� ����������
        }

    // ��������� �������������� �������� 
    void setCalibration(uint16_t cal){
        writeRegister(INA219_CAL_REG_ADDR, cal);    // ����� �������� � ������� ����������
        _cal_value=cal;                           // ��������� ���������� ���������� 
        }

    // ������ �������������� �������� 
    uint16_t getCalibration(void){
        _cal_value=readRegister(INA219_CAL_REG_ADDR); // ��������� ���������� ���������
        return _cal_value;                              // ���������� ��������
        }

    // ��������� ���������� ��� ���������� ������
    void setResolution(bool ch, uint8_t mode){
        uint16_t cfg_register=readRegister(INA219_CFG_REG_ADDR);      // ������ ����. �������
        cfg_register&=~((0b1111)<<(ch?7:3));                   // ���������� ������ ����� ���, � ����������� �� ������
        cfg_register|=mode<<(ch?7:3);                           // ����� ������ ����� ���, � ����������� �� ������
        writeRegister(INA219_CFG_REG_ADDR, cfg_register);               // ����� ����� �������� ����. ��������
        }

    // ������ ���������� �� �����
    float getShuntVoltage(void){
        setCalibration(_cal_value);                             // �������������� ���������� ���������� (�� ������ ���������� ������ INA219)
        int16_t value=readRegister(INA219_SHUNT_REG_ADDR);    // ������ �������� ���������� �����
        return value*0.00001f;                                // LSB = 10uV = 0.00001V, �������� � ����������
        }

    // ������ ���������� 
    float getVoltage(void){
        uint16_t value=readRegister(INA219_VBUS_REG_ADDR);    // ������ �������� ���������� 
        return (value>>3)*0.004f;                           // LSB = 4mV = 0.004V, �������� �������� �� 12 ��� � �������� 
        }

    // ������ ����
    float getCurrent(void){
        setCalibration(_cal_value);                             // �������������� ���������� ���������� (�� ������ ���������� ������ INA219)
        int16_t value=readRegister(INA219_CUR_REG_ADDR);      // ������ �������� ����
        return value*_current_lsb;                            // LSB �������������� �� ������ ����. ���������� ����, �������� � ����������
        }

    // ������ �������� 
    float getPower(void){
        setCalibration(_cal_value);                             // �������������� ���������� ���������� (�� ������ ���������� ������ INA219)
        uint16_t value=readRegister(INA219_POWER_REG_ADDR);   // ������ �������� ��������
        return value*_power_lsb;                              // LSB � 20 ��� ������ LSB ��� ����, �������� ����������
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
        Wire.beginTransmission(_iic_address);               // �������� ��������
        Wire.write(address);                                // ���������� �����
        Wire.write(highByte(data));                         // ���������� ������� ����
        Wire.write(lowByte(data));                          // ���������� ������� ����
        Wire.endTransmission();                             // ����������� ��������
        }

    // ������ 16-�� ������� �������� INA219
    uint16_t readRegister(uint8_t address){
        Wire.beginTransmission(_iic_address);       // �������� ��������
        Wire.write(address);                        // ���������� �����	
        Wire.endTransmission();                     // ����������� ��������
        Wire.requestFrom(_iic_address, (uint8_t)2); // ����������� 2 �����
        return Wire.read()<<8|Wire.read();      // ����� � ���������� ���������
        }

    // �������� �����������
    bool testConnection(void){
        Wire.beginTransmission(_iic_address);       // �������� �������� 
        return (bool)!Wire.endTransmission();       // ����� �����������, ����������� ���������
        }

    // ��������� �������� �������������� �������� � �������������
    void calibrate(void){
        writeRegister(INA219_CFG_REG_ADDR, 0x8000);                 // �������������� �����

        _current_lsb=_i_max/32768.0f;                           // ������� LSB ��� ���� (��. ���� INA219)
        _power_lsb=_current_lsb*20.0f;                          // ������� LSB ��� �������� (��. ���� INA219)
        _cal_value=trunc(0.04096f/(_current_lsb*_r_shunt));   // ������� �������������� �������� (��. ���� INA219)

        setCalibration(_cal_value);                                 // ���������� ����������� ������������� ��������

        uint16_t cfg_register=readRegister(INA219_CFG_REG_ADDR)&~(0b11<<11);  // ������ ������� ����������, ���������� ���� ��������� ���������� �����
        uint16_t vshunt_max_mv=(_r_shunt*_i_max)*1000;        // ������� ���� ���������� �� ����� ��� ���� ��������� ����

        if(vshunt_max_mv<=40) cfg_register|=0b00<<11;        // +/- 40mV  - �������� ��������
        else if(vshunt_max_mv<=80) cfg_register|=0b01<<11;   // +/- 80mV
        else if(vshunt_max_mv<=160) cfg_register|=0b10<<11;  // +/- 160mV
        else cfg_register|=0b11<<11;                            // +/- 320mV
        writeRegister(INA219_CFG_REG_ADDR, cfg_register);           // ����� ����. �������
        }
    };
#endif
