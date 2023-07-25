//
// Created by 神奇bug在哪里 on 7/11/23.
//

#ifndef TM4C_COMMPENT_H
#define TM4C_COMMPENT_H
#include <Arduino.h>
#include "MicroTasksTask.h"

class Commpent : virtual public MicroTasks::Task {
private:
    long defaultWakeupInterval = 500;
public:
    Commpent() : MicroTasks::Task() {};
    virtual void setup() = 0;
    virtual unsigned long loop(MicroTasks::WakeReason reason) override;
    virtual void IQRHandler() = 0;
};
#endif //TM4C_COMMPENT_H
