//
// Created by 神奇bug在哪里 on 7/27/23.
//

#ifndef TM4C_TEMPWHILE_H
#define TM4C_TEMPWHILE_H
#include "commpent.h"
#include "MicroTasks.h"

class TempWhile :virtual public MicroTasks::Task, public Commpent {
    private:
    uint64_t interval = 0;
    void (*func)(void) = nullptr;
    public:
    TempWhile():MicroTasks::Task(),Commpent(12){};
    void setup() override;
    void setFunc(void (*newFunc)(void));
    void setInterval(uint64_t newInterval);
    unsigned long loop(MicroTasks::WakeReason reason) override;
    void IQRHandler() override{};
};


#endif //TM4C_TEMPWHILE_H
