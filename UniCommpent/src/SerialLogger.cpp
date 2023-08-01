//
// Created by 神奇bug在哪里 on 7/12/23.
//

#include <cstdarg>
#include "SerialLogger.h"
SerialLogger logger;
void SerialLogger::log(Loggr::LOG_LEVEL thisLevel, const char *format, ...) {
    Loggr::log(thisLevel, format);
    if (logLevel > thisLevel) return;
    if (serialObj1 == nullptr) return;
    auto levelString = getLogLevelString(thisLevel);
    serialObj1->print(levelString);
    va_list args;
    va_start(args, format);
    char buf[256];
    vsprintf(buf, format, args);
    va_end(args);
    serialObj1->print(buf);
    serialObj1->print("\r\n");
    if (serialObj2 != nullptr)
    {
        serialObj2->print(levelString);
        serialObj2->print(buf);
        serialObj2->print("\r\n");
    }


}

void SerialLogger::setSerialObj1(HardwareSerial * newSerialObj) {
    serialObj1 = newSerialObj;
}

void SerialLogger::setBaudRate(uint64_t newBaudRate) {
    baudRate = newBaudRate;
    if (isInit)
    {
        serialObj1->end();
        serialObj1->begin(baudRate);
    }
    else
    {
        serialObj1->begin(baudRate);
    }
}

void SerialLogger::setup() {
    if (serialObj1 == nullptr) {
        serialObj1 = &Serial;
        log(WARN, "SerialLogger: SerialObj is nullptr, use default Serial");
    }
    serialObj1->begin(baudRate);
    if (serialObj2!= nullptr)
    {
        serialObj2->begin(baudRate);
    }
    isInit = true;
}

void SerialLogger::setSerialObj2(HardwareSerial *newSerialObj) {
    serialObj2 = newSerialObj;
}
