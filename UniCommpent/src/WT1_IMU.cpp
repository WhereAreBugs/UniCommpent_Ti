//
// Created by 神奇bug在哪里 on 7/27/23.
//

#include <functional>
#include "WT1_IMU.h"
#include "UniCommpents.h"


void WT1_IMU::setup() {
    serial->begin(115200);
    WT61_SDK::setup();
    serial->setTimeout(10);
}

unsigned long WT1_IMU::loop(MicroTasks::WakeReason reason) {
    if (reason == WakeReason_Message)
    {
        MicroTasks::Message* msg;
        if(this->receive(msg))
        {

            //TODO: 处理消息
            delete msg;
        }
    }
    if (reason == WakeReason_Scheduled)
    {
        if (serial->available())
        {
            IQRHandler();
        }
    }
    return 10|MicroTasks::MicroTasksClass::WaitForMessage;
}

void WT1_IMU::IQRHandler() {
    auto data = serial->readString();
    for (int i = 0; i < data.length(); ++i) {
        WitSerialDataIn(static_cast<uint8_t >(data.charAt(i)));
    }
}

