//
// Created by 神奇bug在哪里 on 7/12/23.
//

#ifndef TM4C_HC_SR04_H
#define TM4C_HC_SR04_H
#include "GPIO.h"
#include "MicroTasksEventListener.h"
class HC_SR04 : public GPIO ,virtual public MicroTasks::Task{
private:
    GPIO *trig = nullptr;
    GPIO *echo = this;
    MicroTasks::EventListener pinListener;
    bool isWorking = false;
    uint32_t result=0;
    uint32_t startTime=0;
    uint32_t endTime=0;
    uint32_t distance=0;
    uint32_t timeout=1000;
public:
    HC_SR04(uint8_t trigPin, uint8_t echoPin);
    void setup() override;
    virtual unsigned long loop(MicroTasks::WakeReason reason);
    void IQRHandler() override;
    void setTrig(GPIO *trig);
    void setEcho(GPIO *echo);
    void setTrig(uint8_t trig);
    void setEcho(const uint8_t *echo);
    uint32_t getResult() const;
    uint32_t getDistance() const;
    void setTimeout(uint32_t timeout);
    void update();
};


#endif //TM4C_HC_SR04_H
