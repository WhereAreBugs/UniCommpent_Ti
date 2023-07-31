//
// Created by 神奇bug在哪里 on 7/29/23.
//

#include "Gy26_Ser.h"
#include "SerialLogger.h"

Gy26_Ser::Gy26_Ser():Commpent(22) {

}

Gy26_Ser::~Gy26_Ser() {
    delete gy26Obj;
}

void Gy26_Ser::setup() {
    serial->begin(9600);
    gy26Obj = new GY26_Uart_Compass(serial);
    auto level = gy26Obj->getCalibrationLevel();
    logger.log(Loggr::INFO,"[GY26] Calibration Level: %d",level);
}

unsigned long Gy26_Ser::loop(MicroTasks::WakeReason reason) {
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
