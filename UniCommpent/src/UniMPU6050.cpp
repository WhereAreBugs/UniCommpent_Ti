//
// Created by 神奇bug在哪里 on 7/25/23.
//

#include "UniMPU6050.h"
#include "SerialLogger.h"
#include "MicroTasksMessage.h"
#include "MicroTasksTask.h"
#include "MicroTasks.h"
UniMPU6050 MPU;
MicroTasks::Interrupt * pinEvent;

void UniMPU6050::setup() {
    logger.log(Loggr::Debug,"Initializing I2C devices...");
    mpu.initialize();
    Serial.println("Testing device connections...");
    mpu.setRate(40);
    devStatus = mpu.dmpInitialize();

    if (mpu.testConnection())
    {
        logger.log(Loggr::Debug,"MPU6050 connection successful");
    }
    else
    {
        logger.log(Loggr::ERROR,"MPU6050 connection failed");
        isSetup = false;
        return;
    }
    if (devStatus == 0) {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateAccel(10);
        mpu.CalibrateGyro(10);
        // turn on the DMP, now that it's ready
        logger.log(Loggr::Debug,"Enabling DMP...");
        mpu.setDMPEnabled(true);
        // enable Arduino interrupt detection
        logger.log(Loggr::Debug,"Enabling interrupt detection (Arduino external interrupt %d ) ....",intPin);
        pinMode(intPin, INPUT);
        pinEvent = new MicroTasks::Interrupt(intPin, RISING);
        pinEvent->Register(&pinListener);
        pinEvent->Attach();
        mpuIntStatus = mpu.getIntStatus();
        logger.log(Loggr::Debug,"DMP ready! Waiting for first interrupt...");
        dmpReady = true;
        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
        isSetup = true;
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        logger.log(Loggr::ERROR,"DMP Initialization failed (code %d )",devStatus);
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
unsigned long UniMPU6050::loop(MicroTasks::WakeReason reason) {
    if (WakeReason_Event == reason && pinEvent->IsTriggered())
    {
        IQRHandler();
        if (notifyCommpent != nullptr)
        {
            notifyCommpent->send(new MpuReadyMessage(pitch,roll,yaw));
        }
        return 1000 | MicroTasks::MicroTasksClass::WaitForEvent;
    }
    if (reason == WakeReason_Scheduled) {
        if (!isSetup)  return MicroTasks::MicroTasksClass::Infinate;
        logger.log(Loggr::Debug, "[UniMPU6050] pitch: %f, roll: %f, yaw: %f", pitch, roll, yaw);

        return 1000 | MicroTasks::MicroTasksClass::WaitForEvent;
    }
    logger.log(Loggr::ERROR,"[UniMPU6050] Unexpected WakeReason: %d",reason);
    return MicroTasks::MicroTasksClass::Infinate;
}

void UniMPU6050::IQRHandler() {
    if (!dmpReady) return;
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        yaw = ypr[0] * 180 / M_PI;
        pitch = ypr[1] * 180 / M_PI;
        roll = ypr[2] * 180 / M_PI;
    }
}

void UniMPU6050::setIntPin(uint8_t newIntPin) {
    this->intPin = newIntPin;
}

UniMPU6050::UniMPU6050() : MicroTasks::Task(),Commpent(16), pinListener(this) {
    mpuIntStatus = 0;
    devStatus = 0;
    packetSize = 0;
    fifoCount = 0;

}

float UniMPU6050::getYaw() const {
    return yaw;
}

float UniMPU6050::getPitch() const {
    return pitch;

}

float UniMPU6050::getRoll() const {
    return roll;
}

void UniMPU6050::setWire(uint8_t wireNum) {
    if (wireNum>4)
    {
        logger.log(Loggr::ERROR,"[UniMPU6050] Invalid Wire Number: %d",wireNum);
        return;
    }
    Wire.setModule(wireNum);
}

void UniMPU6050::setNotifyCommpent(Commpent *commpent) {
    this->notifyCommpent = commpent;
}

#pragma clang diagnostic pop

MpuReadyMessage::MPUData MpuReadyMessage::getData() const {
    return data;
}
