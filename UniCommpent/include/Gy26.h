//
// Created by 神奇bug在哪里 on 7/27/23.
//

#ifndef TM4C_GY26_H
#define TM4C_GY26_H
#include "commpent.h"
#include "MicroTasksTask.h"
#include "GY26Compass.h"
class Gy26 : public Commpent,virtual public MicroTasks::Task{
private:
    GY26_I2C_Compass *gy26Obj = nullptr;
    uint8_t updateInterval = 200;
    float yaw{};
public:
    ///构造函数和析构函数
    Gy26();
    explicit Gy26(uint8_t address);
    Gy26(uint8_t address, TwoWire * wire);
    ~Gy26() override;
    ///设置函数
    void setUpdateInterval(uint8_t newUpdateInterval) { updateInterval = newUpdateInterval; }
    void setup() override;
    unsigned long loop(MicroTasks::WakeReason reason) override;

};


#endif //TM4C_GY26_H
