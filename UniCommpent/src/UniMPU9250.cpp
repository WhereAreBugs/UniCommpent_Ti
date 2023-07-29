//
// Created by 神奇bug在哪里 on 7/28/23.
//

#include "UniMPU9250.h"
#include "SerialLogger.h"

UniMPU9250::UniMPU9250() : Commpent(19) {
    mpu9250 = new MPU9250();
    settings.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_125HZ;
    updateInterval = 1000/125;
}

UniMPU9250::~UniMPU9250() {
    delete mpu9250;
}

void UniMPU9250::setup() {
    mpu9250->setup(addr, settings,*taregtwire);
    if (!mpu9250->isConnected())
        logger.log(Loggr::ERROR, "[UniMPU9250] MPU9250 not connected");
    mpu9250->calibrateAccelGyro();
    logger.log(Loggr::INFO, "[UniMPU9250] Accel and Gyro calibrated");

}

void UniMPU9250::setUpdateInterval(int16_t newInterval) {
    updateInterval = newInterval;
}

void UniMPU9250::setIQREnabled(bool enabled) {
    IQREnabled = enabled;
}

void UniMPU9250::setAddr(uint8_t newAddr) {
    addr = newAddr;
}

unsigned long UniMPU9250::loop(MicroTasks::WakeReason reason) {
    if (reason == WakeReason_Scheduled)
    {
        if (!mpu9250->update())
        {
            logger.log(Loggr::ERROR, "[UniMPU9250] MPU9250 update failed");
        }
        if (dataReceiver!= nullptr)
        {
            dataReceiver->send(new MPU9250DataReady(MPU9250DataReady::MPU9250Data(
                    mpu9250->getAccX(), mpu9250->getAccY(), mpu9250->getAccZ(),
                    mpu9250->getGyroX(), mpu9250->getGyroY(), mpu9250->getGyroZ(),
                    mpu9250->getMagX(), mpu9250->getMagY(), mpu9250->getMagZ()
            ), MPU9250DataReady::pitchYawRoll(
                    mpu9250->getPitch(), mpu9250->getYaw(), mpu9250->getRoll()
            )));
        }
    }
    if (reason == WakeReason_Event)
    {
        IQRHandler();
    }
    if (IQREnabled)
        return updateInterval|MicroTasks::MicroTasksClass::WaitForEvent;
    else
        return updateInterval;
}

void UniMPU9250::IQRHandler() {

}

void UniMPU9250::cilibrateAcc() {
    mpu9250->calibrateAccelGyro();
    config.push_back(mpu9250->getAccBiasX());
    config.push_back(mpu9250->getAccBiasY());
    config.push_back(mpu9250->getAccBiasZ());
    config.push_back(mpu9250->getGyroBiasX());
    config.push_back(mpu9250->getGyroBiasY());
    config.push_back(mpu9250->getGyroBiasZ());
}

void UniMPU9250::cilibrateMag() {
    mpu9250->calibrateMag();
    config.push_back(mpu9250->getMagBiasX());
    config.push_back(mpu9250->getMagBiasY());
    config.push_back(mpu9250->getMagBiasZ());
}

bool UniMPU9250::checkSavedConfig() {
    return false;
}

void UniMPU9250::reloadConfig() {
    mpu9250->setAccBias(config.at(0),config.at(1),config.at(2));
    mpu9250->setGyroBias(config.at(3),config.at(4),config.at(5));
    mpu9250->setMagBias(config.at(6),config.at(7),config.at(8));
}
