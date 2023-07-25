//
// Created by 神奇bug在哪里 on 7/24/23.
//

#ifndef TM4C_MSP_QEI_H
#define TM4C_MSP_QEI_H
#include "commpent.h"
#include <qei.h>
class MSP_QEI : public Commpent,virtual public MicroTasks::Task {
private:
    uint32_t QEI_BASE;
    int64_t  positionValue;
    uint16_t readFrecuency = 500;
public:
    MSP_QEI(uint32_t QEI_BASE);
    void setup() override;
    unsigned long loop(MicroTasks::WakeReason reason) override;
    void IQRHandler() override;
    int64_t getPositionValue() const;
    int64_t getVelocityValue() const;

};


#endif //TM4C_MSP_QEI_H
