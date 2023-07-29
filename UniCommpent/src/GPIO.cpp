//
// Created by 神奇bug在哪里 on 7/12/23.
//

#include "GPIO.h"
#include "Loggr.h"
#include "SerialLogger.h"

extern SerialLogger logger;
GPIO::GPIO(uint8_t pin, uint8_t mode):Commpent(7) {
    this->pin = pin;
    this->mode = mode;
    this->state = LOW;
}

unsigned long GPIO::loop(MicroTasks::WakeReason reason) {
    return Commpent::loop(reason);

}

void GPIO::setPin(uint8_t pin) {
    this->pin = pin;
}

void GPIO::setMode(uint8_t mode) {
    this->mode = mode;
}

void GPIO::setState(uint8_t state) {
    this->state = state;
    digitalWrite(pin, state);
}

uint8_t GPIO::getPin() {
    return pin;
}

uint8_t GPIO::getMode() {
    return mode;
}

uint8_t GPIO::getState() {
    return state;
}

void GPIO::toggle() {
    if (state == HIGH) {
        state = LOW;
    } else {
        state = HIGH;
    }
    digitalWrite(pin, state);
}

void GPIO::setup() {
    pinMode(pin, mode);
}

void GPIO::IQRHandler() {
    logger.log(Loggr::Debug, "[GPIO] IQRHandler");
}


