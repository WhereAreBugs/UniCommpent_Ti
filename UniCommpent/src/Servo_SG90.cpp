//
// Created by 神奇bug在哪里 on 7/27/23.
//

#include "Servo_SG90.h"
#include "SerialLogger.h"

void Servo_SG90::setup() {
    servoObj->attach(pin);
}

void Servo_SG90::write(float newAngle) {
   servoObj->write(newAngle);
}

unsigned long Servo_SG90::loop(MicroTasks::WakeReason reason) {
    return MicroTasks::MicroTasksClass::Infinate;
}

Servo_SG90::~Servo_SG90() {
    delete servoObj;
}

Servo_SG90::Servo_SG90(uint8_t newPin) : Commpent(15), pin(newPin) {
    servoObj = new Servo();
    if (servoObj== nullptr)
    {
        logger.log(Loggr::ERROR,"Servo_SG90::Servo_SG90()","new Servo() failed!");
    }
}


