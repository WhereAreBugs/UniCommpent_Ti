//
// Created by 神奇bug在哪里 on 7/26/23.
//

#ifndef TM4C_MOTOR_H
#define TM4C_MOTOR_H

#include <vector>
#include "commpent.h"


class Motor : public Commpent, virtual public MicroTasks::Task {
private:
    std::vector<uint8_t> pinAs;
    std::vector<uint8_t> pinBs;
public:
    Motor() = default;
    Motor(uint8_t pinA1,uint8_t pinB1,uint8_t pinA2,uint8_t pinB2);
    void setup() override;
    unsigned long loop(MicroTasks::WakeReason reason) override;
    void IQRHandler() override;
    void addTarget(uint8_t pinA,uint8_t pinB);
    void setPWM(uint8_t motor, int16_t pwm);
};


#endif //TM4C_MOTOR_H
