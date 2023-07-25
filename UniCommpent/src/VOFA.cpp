//
// Created by 神奇bug在哪里 on 7/13/23.
//

#include "VOFA.h"

VOFA::VOFA(HardwareSerial *serial) : MicroTasks::Task() {
    serialObj = serial;
}

void VOFA::setup() {
    serialObj->begin(buadRate);

}


#ifdef ESP32
void VOFA::setPins(uint8_t txPin, uint8_t rxPin) {
    this->txPin = txPin;
    this->rxPin = rxPin;
    serialObj->setPins(txPin,rxPin);
}
#elif defined(ENERGIA_EK_TM4C123GXL)
void VOFA::setPins(unsigned long pins) {
    serialObj->setPins(pins);
}

unsigned long VOFA::loop(MicroTasks::WakeReason reason) {
    if (WakeReason_Scheduled == reason)
    {

        for (auto &reportData : reportDatas) {
            serialObj->print(*reportData);
            serialObj->print(",");
        }
        serialObj->println();
    }
    return reportInterval;
}

void VOFA::addReportData(float *data) {
    reportDatas.push_back(data);
}

void VOFA::setSerial(HardwareSerial *serialObj) {
    VOFA::serialObj = serialObj;
}

void VOFA::setBuadRate(uint32_t buadRate) {
    VOFA::buadRate = buadRate;
}

void VOFA::setReportInterval(uint32_t reportInterval) {
    VOFA::reportInterval = reportInterval;
}

#endif
