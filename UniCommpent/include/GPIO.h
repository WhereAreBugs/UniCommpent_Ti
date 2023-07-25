//
// Created by 神奇bug在哪里 on 7/12/23.
//

#ifndef TM4C_GPIO_H
#define TM4C_GPIO_H


#include "commpent.h"

class GPIO : public Commpent {
private:
    uint8_t pin;
    uint8_t mode;
    uint8_t state;
public:
    GPIO(uint8_t pin, uint8_t mode);
    virtual void setup();
    virtual unsigned long loop(MicroTasks::WakeReason reason);
    virtual void IQRHandler();
    void setPin(uint8_t pin);
    void setMode(uint8_t mode);
    void setState(uint8_t state);
    uint8_t getPin();
    uint8_t getMode();
    uint8_t getState();
    void toggle();
};


#endif //TM4C_GPIO_H
