//
// Created by 神奇bug在哪里 on 7/27/23.
//

#ifndef TM4C_SERVO_SG90_H
#define TM4C_SERVO_SG90_H

#include <wiring_private.h>
#include "commpent.h"
#include "MicroTasks.h"
#include "Servo.h"
class Servo_SG90 : public Commpent, virtual public MicroTasks::Task {
private:
    uint8_t pin;
    float angle = 0;
    Servo * servoObj = nullptr;
public:
    explicit Servo_SG90(uint8_t newPin);
    ~Servo_SG90() override;
    void setup() override;
    void IQRHandler() override{};
    unsigned long loop(MicroTasks::WakeReason reason) override;
    void write(float newAngle);
};


#endif //TM4C_SERVO_SG90_H
