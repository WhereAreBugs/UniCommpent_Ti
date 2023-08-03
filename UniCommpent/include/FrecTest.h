//
// Created by 神奇bug在哪里 on 8/2/23.
//

#ifndef TM4C_FRECTEST_H
#define TM4C_FRECTEST_H
#include "commpent.h"
#include "MicroTasks.h"
class RequestFrec : public MicroTasks::Message {
private:
    static const uint32_t ID = 0x83;
public:
    explicit RequestFrec() : Message(ID){}
};

class FrecTest : public Commpent, virtual public MicroTasks::Task{
private:
    MicroTasks::EventListener * listener = nullptr;
    int timer = TIMER1_BASE;
    uint64_t  frec = 0;
public:
    explicit FrecTest():MicroTasks::Task(),Commpent(64){
        listener = new MicroTasks::EventListener( this);
    };
    void setup() override ;
    void IQRHandler() override;
    unsigned long loop(MicroTasks::WakeReason reason) override;
    uint64_t  getFrec() const;
    ~FrecTest() override;

    void TimerValueClear(uint32_t timerBase, uint32_t Timer);
};


#endif //TM4C_FRECTEST_H
