//
// Created by 神奇bug在哪里 on 7/26/23.
//

#ifndef TM4C_SERIALCHECKER_H
#define TM4C_SERIALCHECKER_H
#include "Commpent.h"
#include "MicroTasks.h"
class SerialChecker : public Commpent,virtual public MicroTasks::Task{
private:
    HardwareSerial *serial;
    void (* dataHandler)(String) = nullptr;
    char terminator = '\n';

public:
    explicit SerialChecker(HardwareSerial *targetSerial) : serial(targetSerial) {};
    void setup() override;
    void IQRHandler() override;
    void onData(void (*newFunc)(String));
    void setTerminator(char newTerminator) { terminator = newTerminator; }
    unsigned long loop(MicroTasks::WakeReason reason) override;
};


#endif //TM4C_SERIALCHECKER_H
