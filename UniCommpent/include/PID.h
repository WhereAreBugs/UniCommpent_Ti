//
// Created by 神奇bug在哪里 on 7/15/23.
//

#ifndef SEARCHCAR_PID_H
#define SEARCHCAR_PID_H
#include "commpent.h"
struct PIDConfig{
    PIDConfig(float kp, float ki, float kd, float min, float target, float max)
    : kp(kp), ki(ki), kd(kd), target(target), max(max), min(min){};
    PIDConfig();
    float kp;
    float ki;
    float kd;
    float target;
    float max;
    float min;
};

class PID : public Commpent {
private:
    PIDConfig pidConfig;
    float lastError{};
    float integral{0};
    bool checkIsConfigExist(PIDConfig &data);

public:
    explicit PID(PIDConfig config);
    void setup() override;
    float update(float input);
    void IQRHandler() override{};
    unsigned long loop(MicroTasks::WakeReason reason) override;
    void setTarget(float target);
    void setConfig(PIDConfig config);
    PIDConfig getConfig();
    int8_t getPreDefinedPIDCount();
    std::vector<float> & getData() override;
    void saveCurrentConfigAsPreDefined(uint8_t index);
    void setPreDefinedPID(uint8_t index);
    void reloadConfig() override;
};


#endif //SEARCHCAR_PID_H
