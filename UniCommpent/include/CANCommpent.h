//
// Created by 神奇bug在哪里 on 7/28/23.
//

#ifndef TM4C_CANCOMMPENT_H
#define TM4C_CANCOMMPENT_H
#include "commpent.h"
#include "sysctl.h"
class CANCommpent : public Stream{
private:

protected:

public:
    ~CANCommpent();
    size_t write(uint8_t address, const uint8_t *buffer, size_t size);
    size_t write(const uint8_t *buffer, size_t size) override;
    size_t write(uint8_t) override;
    int available() override;
    int read() override;
    int peek() override;
    void flush() override;
    void begin();
};


#endif //TM4C_CANCOMMPENT_H
