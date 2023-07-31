//
// Created by 神奇bug在哪里 on 7/28/23.
//

#ifndef TM4C_UNIMPU9250_H
#define TM4C_UNIMPU9250_H

#include <vector>
#include "commpent.h"
#include "MicroTasks.h"
#include "MPU9250.h"
class MPU9250DataReady : public MicroTasks::Message {
public:
    typedef struct MPU9250Data{
        float accX, accY, accZ;
        float gyroX, gyroY, gyroZ;
        float magX, magY, magZ;
        MPU9250Data(float accX, float accY, float accZ, float gyroX, float gyroY,
                    float gyroZ, float magX, float magY, float magZ) :accX(accX),
                    accY(accY), accZ(accZ), gyroX(gyroX), gyroY(gyroY), gyroZ(gyroZ),
                    magX(magX), magY(magY), magZ(magZ) {}
    } MPU9250Data;
    typedef struct pitchYawRoll{
        float pitch, yaw, roll;
        pitchYawRoll(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll) {}
    } pitchYawRoll;
public:
    MPU9250DataReady(MPU9250Data dat,pitchYawRoll pyr) : MicroTasks::Message(0x04), data(dat), pyr_data(pyr) {};
    MPU9250Data getData() { return data; }
    pitchYawRoll getPYR() { return pyr_data; }
private:
    MPU9250Data data;
    pitchYawRoll pyr_data;
};

class UniMPU9250 : public Commpent, virtual public MicroTasks::Task {
private:
    MPU9250 *mpu9250;
    MPU9250Setting settings;
    int16_t updateInterval;
    bool IQREnabled = false;
    uint8_t addr = 0x68;
    TwoWire * taregtwire = &Wire;
    Commpent * dataReceiver = nullptr;
    static bool checkSavedConfig();
public:
    UniMPU9250();
    ~UniMPU9250() override;
    void setup() override;
    void IQRHandler() override;
    void setDataRecver(Commpent * recver) { dataReceiver = recver; }
    unsigned long loop(MicroTasks::WakeReason reason) override;
    void setUpdateInterval(int16_t newInterval);
    void setIQREnabled(bool enabled);
    void setAddr(uint8_t newAddr);
    void setWire(TwoWire &wire) { taregtwire = &wire; }
    void cilibrateAcc();
    void cilibrateMag();
    ///getter
    float getAccX() { return mpu9250->getAccX(); }
    float getAccY() { return mpu9250->getAccY(); }
    float getAccZ() { return mpu9250->getAccZ(); }
    float getGyroX() { return mpu9250->getGyroX(); }
    float getGyroY() { return mpu9250->getGyroY(); }
    float getGyroZ() { return mpu9250->getGyroZ(); }
    float getMagX() { return mpu9250->getMagX(); }
    float getMagY() { return mpu9250->getMagY(); }
    float getMagZ() { return mpu9250->getMagZ(); }
    float getPitch() { return mpu9250->getPitch(); }
    float getYaw() { return mpu9250->getYaw(); }
    float getRoll() { return mpu9250->getRoll(); }
public:
    std::vector<float> getDataCopy() override;
    void  reloadConfig() override;

};


#endif //TM4C_UNIMPU9250_H
