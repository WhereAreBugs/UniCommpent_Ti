//
// Created by 神奇bug在哪里 on 7/12/23.
//

#include <cstdarg>
#include "SerialLogger.h"
SerialLogger logger;
void SerialLogger::log(Loggr::LOG_LEVEL thisLevel, const char *format, ...) {
    Loggr::log(thisLevel, format);
    if (serialObj == nullptr) return;
    auto levelString = getLogLevelString(thisLevel);
    serialObj->print(levelString);
    va_list args;
    va_start(args, format);
    char buf[256];
    vsprintf(buf, format, args);
    serialObj->print(buf);
    va_end(args);
    serialObj->print("\r\n");

}

void SerialLogger::setSerialObj(HardwareSerial * newSerialObj) {
    serialObj = newSerialObj;
}

void SerialLogger::setBaudRate(uint64_t newBaudRate) {
    baudRate = newBaudRate;
    if (isInit)
    {
        serialObj->end();
        serialObj->begin(baudRate);
    }
    else
    {
        serialObj->begin(baudRate);
    }
}

void SerialLogger::setup() {
    if (serialObj == nullptr) {
        serialObj = &Serial;
        log(WARN, "SerialLogger: SerialObj is nullptr, use default Serial");
    }
    serialObj->begin(baudRate);
    isInit = true;
}
