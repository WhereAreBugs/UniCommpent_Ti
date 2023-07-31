//
// Created by 神奇bug在哪里 on 7/31/23.
//

#include "ADC.h"
#include "SerialLogger.h"

void ADC::setup() {
    pinMode(pin, INPUT);
}

unsigned long ADC::loop(MicroTasks::WakeReason reason) {
    if (reason == WakeReason_Scheduled)
    {
        value = analogRead(pin);
    }
    logger.log(Loggr::Debug, "ADC: value %d", value);
    return 100;
}

void ADC::IQRHandler() {
}
