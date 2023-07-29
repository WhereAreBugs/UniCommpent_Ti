//
// Created by 神奇bug在哪里 on 7/27/23.
//

#ifndef TM4C_WT1_IMU_H
#define TM4C_WT1_IMU_H

#include <Wire.h>
#include "commpent.h"
#include "MicroTasks.h"
#include "WT1_IMU_REG.h"
#include "CANCommpent.h"
#include "WT61_SDK.h"
///官方库专属宏定义
#define FuncW 0x06
#define FuncR 0x03
#define WIT_HAL_OK      (0)     /**< There is no error */
#define WIT_HAL_BUSY    (-1)    /**< Busy */
#define WIT_HAL_TIMEOUT (-2)    /**< Timed out */
#define WIT_HAL_ERROR   (-3)    /**< A generic error happens */
#define WIT_HAL_NOMEM   (-4)    /**< No memory */
#define WIT_HAL_EMPTY   (-5)    /**< The resource is empty */
#define WIT_HAL_INVAL   (-6)    /**< Invalid argument */
#define WIT_DATA_BUFF_SIZE  256
#define WIT_PROTOCOL_NORMAL 0
#define WIT_PROTOCOL_MODBUS 1
#define WIT_PROTOCOL_CAN    2
#define WIT_PROTOCOL_I2C    3

class WT1_IMU : public Commpent, virtual public MicroTasks::Task, public WT61_SDK {
public:
    explicit WT1_IMU(HardwareSerial *targetSerial) :MicroTasks::Task(), Commpent(20), WT61_SDK(targetSerial) {};
    void setup() override;
    void IQRHandler() override;
    unsigned long loop(MicroTasks::WakeReason reason) override;
};


#endif //TM4C_WT1_IMU_H
