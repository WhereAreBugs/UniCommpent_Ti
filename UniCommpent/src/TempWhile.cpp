//
// Created by 神奇bug在哪里 on 7/27/23.
//

#include "TempWhile.h"
#include "SerialLogger.h"
void TempWhile::setup() {
    logger.log(Loggr::Debug, "[TempWhile] setup");
}

void TempWhile::setFunc(void (*newFunc)(void)) {
    func = newFunc;
}

void TempWhile::setInterval(uint64_t newInterval) {
    interval = newInterval;
}

unsigned long TempWhile::loop(MicroTasks::WakeReason reason) {
    if (reason == WakeReason_Scheduled) {
        if (func != nullptr) {
            func();
        } else {
            logger.log(Loggr::ERROR, "[TempWhile] func is nullptr");
        }
    }
    return interval;
}
