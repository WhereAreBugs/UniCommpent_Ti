//
// Created by 神奇bug在哪里 on 7/25/23.
//

#ifndef TM4C_UNIMPU6050_H
#define TM4C_UNIMPU6050_H
#include "commpent.h"
#include "../MPU/MPU6050_6Axis_MotionApps_V6_12.h"
#include "../MPU/MPU6050.h"
#include "MicroTasksEventListener.h"

class UniMPU6050 : public Commpent,virtual public MicroTasks::Task{
private:
    uint8_t teapotPacket[14] = {'$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };
    bool dmpReady = false;  // set true if DMP init was successful
    uint8_t fifoBuffer[64]{}; // FIFO storage buffer
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
// orientation/motion vars
    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorInt16 aa;         // [x, y, z]            accel sensor measurements
    VectorInt16 gy;         // [x, y, z]            gyro sensor measurements
    VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
    VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
    VectorFloat gravity;    // [x, y, z]            gravity vector
    float euler[3]{};         // [psi, theta, phi]    Euler angle container
    float ypr[3]{};           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
    uint8_t intPin = 0;
    MPU6050 mpu;
    bool isSetup = false;
    float yaw = 0;
    float pitch = 0;
    float roll = 0;
    MicroTasks::EventListener pinListener;
public:
    void setup() override;
    unsigned long loop(MicroTasks::WakeReason reason) override;
    void IQRHandler() override;
    void setIntPin(uint8_t intPin);
    float getYaw() const;
    float getPitch() const;
    float getRoll() const;

    UniMPU6050();
};
extern UniMPU6050 MPU;

#endif //TM4C_UNIMPU6050_H
