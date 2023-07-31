//
// Created by 神奇bug在哪里 on 7/24/23.
//

#ifndef TM4C_MSP_QEI_H
#define TM4C_MSP_QEI_H
#include "commpent.h"
#include <qei.h>
class SpeedFeedback : public MicroTasks::Message {
private:
    static const uint32_t ID = 0x91;
    bool left = 0;
public:
    explicit SpeedFeedback(bool newSpeed) : Message(ID) , left(newSpeed){}
    uint16_t getSpeed() const {
        return left;
    }
};
class MSP_QEI : public Commpent,virtual public MicroTasks::Task {
private:
    uint32_t QEI_BASE;
    int64_t  positionValue{};
    uint16_t readFrecuency = 500;
    Commpent * dataSpeedNotify = nullptr;
public:
    MSP_QEI(uint32_t QEI_BASE);
    void setup() override;
    unsigned long loop(MicroTasks::WakeReason reason) override;
    void IQRHandler() override;
    int64_t getPositionValue() const;
    int64_t getVelocityValue() const;
    void setDataSpeedNotify(Commpent *dataSpeedNotify);

};


#endif //TM4C_MSP_QEI_H
