//
// Created by 神奇bug在哪里 on 7/26/23.
//

#include "Motor.h"
#include "SerialLogger.h"
#include "MicroTasks.h"

void Motor::addTarget(uint8_t pinA, uint8_t pinB) {
    pinAs.push_back(pinA);
    pinBs.push_back(pinB);

}



void Motor::IQRHandler() {
    logger.log(Loggr::INFO, "[Motor] IQRHandler");
}

void Motor::setup() {
    for (auto pinA : pinAs) {
        pinMode(pinA, OUTPUT);
    }
    for (auto pinB : pinBs) {
        pinMode(pinB, OUTPUT);
    }
    logger.log(Loggr::INFO, "[Motor] setup");
}

unsigned long Motor::loop(MicroTasks::WakeReason reason) {
    return MicroTasks::MicroTasksClass::Infinate;
}

void Motor::setPWM(uint8_t motor, int16_t pwm) {
    if (motor<0||motor>1)
        return;
    if (pwm <0){
        analogWrite(pinAs[motor], 0);
        analogWrite(pinBs[motor], -pwm);
    }
    else {
        analogWrite(pinAs[motor], pwm);
        analogWrite(pinBs[motor], 0);
    }
}

Motor::Motor(uint8_t pinA1, uint8_t pinB1, uint8_t pinA2, uint8_t pinB2) {
    addTarget(pinA1,pinB1);
    addTarget(pinA2,pinB2);
}




