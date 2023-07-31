//
// Created by 神奇bug在哪里 on 7/31/23.
//

#ifndef TM4C_ADC_H
#define TM4C_ADC_H
#include "MicroTasks.h"
#include "commpent.h"

class ADC : public Commpent,virtual public MicroTasks::Task{
private:
    uint8_t pin;
    uint16_t value;
public:
    explicit ADC(uint8_t pin) : MicroTasks::Task(), Commpent(25),pin(pin), value(0) {}
    void setup() override;
    unsigned long loop(MicroTasks::WakeReason reason) override;
    void IQRHandler() override;
    uint16_t getValue() const { return value; }
    void setPin(uint8_t newPin) { this->pin = newPin; }
};


#endif //TM4C_ADC_H
