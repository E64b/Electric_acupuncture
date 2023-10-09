#pragma once
#include <Arduino.h>

#include "VirtEncButton.h"
#include "utils.h"

// ===================== CLASS =====================
class EncButton: public VirtEncButton{
    public:
    // ��������� ���� (���, ���, ������, pinmode ���, pinmode ������)
    EncButton(uint8_t encA=0, uint8_t encB=0, uint8_t btn=0, uint8_t modeEnc=INPUT, uint8_t modeBtn=INPUT_PULLUP, uint8_t btnLevel=LOW){
        init(encA, encB, btn, modeEnc, modeBtn, btnLevel);
        }

    // ��������� ���� (���, ���, ������, pinmode ���, pinmode ������)
    void init(uint8_t encA=0, uint8_t encB=0, uint8_t btn=0, uint8_t modeEnc=INPUT, uint8_t modeBtn=INPUT_PULLUP, uint8_t btnLevel=LOW){
        e0=encA;
        e1=encB;
        b=btn;
        pinMode(e0, modeEnc);
        pinMode(e1, modeEnc);
        pinMode(b, modeBtn);
        setBtnLevel(btnLevel);
        initEnc(readEnc());
        }

    // ====================== TICK ======================
    // ������� ��������� ��� ������ � ���������� ��������
    int8_t tickISR(){
        return VirtEncButton::tickISR(readEnc());
        }

    // ������� ���������, �������� � loop
    bool tick(){
        if(read_ef(EB_EISR)) return VirtEncButton::tick(EBread(b));
        else return VirtEncButton::tick(readEnc(), EBread(b));
        }

    // ������� ��������� ��� ������ �������
    bool tickRaw(){
        if(read_ef(EB_EISR)) return VirtEncButton::tickRaw(EBread(b));
        else return VirtEncButton::tickRaw(readEnc(), EBread(b));
        }

    // ====================== READ ======================
    // ��������� �������� ������
    bool readBtn(){
        return EBread(b)^read_bf(EB_INV);
        }

    // ��������� �������� ��������
    int8_t readEnc(){
        return EBread(e0)|(EBread(e1)<<1);
        }

    // ===================== PRIVATE =====================
    private:
    uint8_t e0, e1, b;
    };

// ===================== T CLASS =====================
template <uint8_t ENCA, uint8_t ENCB, uint8_t BTN>
class EncButtonT: public VirtEncButton{
    public:
    // ��������� ���� (���, ���, ������, pinmode ���, pinmode ������)
    EncButtonT(uint8_t modeEnc=INPUT, uint8_t modeBtn=INPUT_PULLUP, uint8_t btnLevel=LOW){
        init(modeEnc, modeBtn, btnLevel);
        }

    // ��������� ���� (pinmode ���, pinmode ������)
    void init(uint8_t modeEnc=INPUT, uint8_t modeBtn=INPUT_PULLUP, uint8_t btnLevel=LOW){
        pinMode(ENCA, modeEnc);
        pinMode(ENCB, modeEnc);
        pinMode(BTN, modeBtn);
        setBtnLevel(btnLevel);
        initEnc(readEnc());
        }

    // ====================== TICK ======================
    // ������� ��������� ��� ������ � ���������� ��������
    int8_t tickISR(){
        return VirtEncButton::tickISR(readEnc());
        }

    // ������� ���������, �������� � loop
    bool tick(){
        if(read_ef(EB_EISR)) return VirtEncButton::tick(EBread(BTN));
        else return VirtEncButton::tick(readEnc(), EBread(BTN));
        }

    // ������� ��������� ��� ������ �������
    bool tickRaw(){
        if(read_ef(EB_EISR)) return VirtEncButton::tickRaw(EBread(BTN));
        else return VirtEncButton::tickRaw(readEnc(), EBread(BTN));
        }

    // ====================== READ ======================
    // ��������� �������� ������
    bool readBtn(){
        return EBread(BTN)^read_bf(EB_INV);
        }

    // ��������� �������� ��������
    int8_t readEnc(){
        return EBread(ENCA)|(EBread(ENCB)<<1);
        }

    // ===================== PRIVATE =====================
    private:
    };