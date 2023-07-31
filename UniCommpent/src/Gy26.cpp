//
// Created by 神奇bug在哪里 on 7/27/23.
//

#include "Gy26.h"
#include "SerialLogger.h"

Gy26_I2C::~Gy26_I2C() {
    delete gy26Obj;
}

Gy26_I2C::Gy26_I2C():Commpent(9) {
    gy26Obj = new GY26_I2C_Compass();
}

Gy26_I2C::Gy26_I2C(uint8_t address) : Commpent(9) {
    gy26Obj = new GY26_I2C_Compass(address);
}

Gy26_I2C::Gy26_I2C(uint8_t address, TwoWire *wire) : Commpent(9) {
    gy26Obj = new GY26_I2C_Compass(address,wire);
}

void Gy26_I2C::setup() {
    if (gy26Obj == nullptr)
    {
        logger.log(Loggr::ERROR,"[Gy26_I2C] gy26Obj is nullptr");
        return;
    }
    auto level = gy26Obj->getCalibrationLevel();
    logger.log(Loggr::INFO,"[Gy26_I2C] Calibration Level: %d",level);
}

unsigned long Gy26_I2C::loop(MicroTasks::WakeReason reason) {
    if (reason == WakeReason_Scheduled)
    {
        yaw =  gy26Obj->getCompassAngle();
        logger.log(Loggr::Debug,"[Gy26_I2C] yaw:%f",yaw);
    } else if (reason == WakeReason_Event)
    {
        IQRHandler();
    }
    return updateInterval;
}




