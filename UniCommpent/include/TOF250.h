//
// Created by 神奇bug在哪里 on 7/27/23.
//

#ifndef TM4C_TOF250_H
#define TM4C_TOF250_H
#include "Commpent.h"
#include <Wire.h>
class TOF250 : public Commpent,virtual public MicroTasks::Task{
private:
    TwoWire * wireHandler = nullptr;
    HardwareSerial * serialHandler = nullptr;
    int address = 0x52;
    uint8_t updateInterval = 200;
    bool SerialMode = false;
    double result = 0;
    void Get_LidarDataFromIIC();
    void Get_liarDataFromSerial();
public:
    void setWireHandler(TwoWire *newWireHandler) { wireHandler = newWireHandler; }
    void setSerialHandler(HardwareSerial *newSerialHandler) { serialHandler = newSerialHandler; }
    void setSerialMode(bool newSerialMode) { SerialMode = newSerialMode; }
    void setAddress(uint8_t newAddress) { address = newAddress; }
    void setUpdateInterval(uint8_t newUpdateInterval) { updateInterval = newUpdateInterval; }
    double getResult() const { return result; }
    void setup() override;
    void IQRHandler() override {};
    unsigned long loop(MicroTasks::WakeReason reason) override;
};


#endif //TM4C_TOF250_H
