//
// Created by 神奇bug在哪里 on 7/11/23.
//
#include "commpent.h"

unsigned long Commpent::loop(MicroTasks::WakeReason reason) {
    if (reason == WakeReason_Event) {
        IQRHandler();
        return defaultWakeupInterval;
    }
    return defaultWakeupInterval;
}
