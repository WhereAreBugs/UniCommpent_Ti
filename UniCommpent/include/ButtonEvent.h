//
// Created by 神奇bug在哪里 on 7/29/23.
//

#ifndef TM4C_BUTTONEVENT_H
#define TM4C_BUTTONEVENT_H
#include "commpent.h"
#include "MicroTasks.h"
class ButtonTirrgerMessage : public MicroTasks::Message{
private:
    uint8_t pin;
public:
    explicit ButtonTirrgerMessage(uint8_t pin):Message(0x10),pin(pin){};
    uint8_t getPin() const { return pin; }
};


class ButtonEvent : public Commpent, virtual public MicroTasks::Task{
private:
    uint8_t pin;
    uint8_t edge;
    MicroTasks::EventListener pinListener;
    MicroTasks::Interrupt * pinEvent = nullptr;
    Commpent * notifyTarget = nullptr;
    void (*notifyFunc)(uint8_t pin) = nullptr;
public:
    explicit ButtonEvent(uint8_t pin , uint8_t edge):MicroTasks::Task(),Commpent(1),pin(pin),
    edge(edge), pinListener(this){};
    void setup() override ;
    void IQRHandler() override{};
    unsigned long loop(MicroTasks::WakeReason reason) override;
    ~ButtonEvent() override;
    void setNotifyTarget(Commpent * target) { notifyTarget = target; }
    void setNotifyFunc(void (*func)(uint8_t pin)) { notifyFunc = func; }
};


#endif //TM4C_BUTTONEVENT_H
