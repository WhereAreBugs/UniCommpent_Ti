//
// Created by 神奇bug在哪里 on 7/27/23.
//

#include "Gy26.h"
#include "SerialLogger.h"

Gy26::~Gy26() {
    delete gy26Obj;
}

Gy26::Gy26():Commpent(9) {
    gy26Obj = new GY26_I2C_Compass();
}

Gy26::Gy26(uint8_t address) : Commpent(9) {
    gy26Obj = new GY26_I2C_Compass(address);
}

Gy26::Gy26(uint8_t address, TwoWire *wire) : Commpent(9) {
    gy26Obj = new GY26_I2C_Compass(address,wire);
}

void Gy26::setup() {
    if (gy26Obj == nullptr)
    {
        logger.log(Loggr::ERROR,"[GY26] gy26Obj is nullptr");
        return;
    }
    auto level = gy26Obj->getCalibrationLevel();
    logger.log(Loggr::INFO,"[GY26] Calibration Level: %d",level);
}

unsigned long Gy26::loop(MicroTasks::WakeReason reason) {
    if (reason == WakeReason_Scheduled)
    {
        yaw =  gy26Obj->getCompassAngle();
        logger.log(Loggr::Debug,"[GY26] yaw:%f",yaw);
    } else if (reason == WakeReason_Event)
    {
        IQRHandler();
    }
    return updateInterval;
}




