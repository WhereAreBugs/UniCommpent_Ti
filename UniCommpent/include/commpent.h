//
// Created by 神奇bug在哪里 on 7/11/23.
//

#ifndef TM4C_COMMPENT_H
#define TM4C_COMMPENT_H
#include <Arduino.h>
#include <vector>
#include "MicroTasksTask.h"

class Commpent : virtual public MicroTasks::Task {
private:
    long defaultWakeupInterval = 500;
    uint32_t uuid;
protected:
    std::vector<float> config;
public:
    explicit Commpent(uint32_t uuid) : MicroTasks::Task(),uuid(uuid) {};
    virtual void setup() = 0; // NOLINT(*-use-override)
    virtual unsigned long loop(MicroTasks::WakeReason reason); // NOLINT(*-use-override)
    virtual void IQRHandler() = 0;
    virtual uint32_t getUUID();
    virtual std::vector<float> &getData() {
        /**
         * @attention 此处返回的数据未进行保护！
         */
        return config;
    }
    virtual std::vector<float> getDataCopy() {
        /**
         * @attention 此处返回的数据未进行保护！
         */
        return config;
    }
    virtual void setData(std::vector<float> data) {
        config = data; //WARNING: this will copy the data NOLINT(*-unnecessary-value-param)
    }
    virtual void reloadConfig() {};
};
#endif //TM4C_COMMPENT_H
