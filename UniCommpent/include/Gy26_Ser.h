//
// Created by 神奇bug在哪里 on 7/29/23.
//

#ifndef TM4C_GY26_SER_H
#define TM4C_GY26_SER_H
#include "commpent.h"
#include "MicroTasks.h"
#include "GY26_Uart_Compass.h"
class Gy26_Ser : public Commpent,virtual public MicroTasks::Task{
private:
    uint8_t updateInterval = 50;
    float yaw{};
    GY26_Uart_Compass *gy26Obj = nullptr;
    HardwareSerial * serial = nullptr;
public:
    ///构造函数和析构函数
    Gy26_Ser();
    ~Gy26_Ser() override;
    ///设置函数
    void setUpdateInterval(uint8_t newUpdateInterval) { updateInterval = newUpdateInterval; }
    void setup() override;
    void IQRHandler() override {};
    void setSerial(HardwareSerial * serial) { this->serial = serial; }
    unsigned long loop(MicroTasks::WakeReason reason) override;
};


#endif //TM4C_GY26_SER_H
