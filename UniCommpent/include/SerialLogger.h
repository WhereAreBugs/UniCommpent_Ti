//
// Created by 神奇bug在哪里 on 7/12/23.
//

#ifndef TM4C_SERIALLOGGER_H
#define TM4C_SERIALLOGGER_H
#include "Loggr.h"
class SerialLogger  : public Loggr {
private:
    HardwareSerial *serialObj = nullptr;
    uint64_t baudRate = 115200;
    bool isInit = false;
public:
    void log(LOG_LEVEL thisLevel, const char *format, ...) override;
    void setSerialObj(HardwareSerial *newSerialObj);
    void setBaudRate(uint64_t newBaudRate);
    void setup() override;
    unsigned long loop(MicroTasks::WakeReason reason) override { return 0; };
    void IQRHandler() override {};
};
extern SerialLogger logger;
#endif //TM4C_SERIALLOGGER_H
