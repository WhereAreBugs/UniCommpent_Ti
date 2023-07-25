//
// Created by 神奇bug在哪里 on 7/13/23.
//

#ifndef TM4C_VOFA_H
#define TM4C_VOFA_H
#include <vector>
#include "commpent.h"

class VOFA : public Commpent{
    std::vector<float * > reportDatas;
    HardwareSerial * serialObj;
    uint32_t buadRate = 115200;
    uint8_t  txPin;
    uint8_t  rxPin;
    uint32_t reportInterval = 100;
public:
    VOFA(HardwareSerial * serial) ;
    void setup() override;
    unsigned long loop(MicroTasks::WakeReason reason) override ;
    void setSerial(HardwareSerial * serialObj);
    void setBuadRate(uint32_t buadRate);
    void setReportInterval(uint32_t reportInterval);

#ifdef ESP32
    void setPins(uint8_t txPin,uint8_t rxPin);
#elif defined(ENERGIA_EK_TM4C123GXL)
    void setPins(unsigned long pins);
#endif
    void addReportData(float * data);
};


#endif //TM4C_VOFA_H
