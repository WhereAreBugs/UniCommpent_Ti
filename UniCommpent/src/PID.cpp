//
// Created by 神奇bug在哪里 on 7/15/23.
//

#include "PID.h"
#include "SerialLogger.h"
#include "MicroTasks.h"
PID::PID(const PIDConfig config) : Commpent(14) {
    this->pidConfig = config;
}

float PID::update(float input) {
    float error;
    error = pidConfig.target - input;
    integral = pidConfig.ki * error + integral;
    if (integral > pidConfig.max)
    {
        integral = pidConfig.max;
    } else if (integral < pidConfig.min)
    {
        integral = pidConfig.min;
    }
    float derivative = error - lastError;
    lastError = error;
    float output = pidConfig.kp * error + pidConfig.ki * integral + pidConfig.kd * derivative;
    if (output > pidConfig.max)
    {
        output = pidConfig.max;
    } else if (output < pidConfig.min)
    {
        output = pidConfig.min;
    }
    return output;
}

void PID::setTarget(float target) {
    this->pidConfig.target = target;
}

void PID::setConfig(PIDConfig config) {
    this->pidConfig = config;
}

PIDConfig PID::getConfig() {
    return pidConfig;
}

unsigned long PID::loop(MicroTasks::WakeReason reason) {
    return MicroTasks::MicroTasksClass::Infinate;
}

void PID::setup() {
    logger.log(Loggr::INFO, "[PID] setup");
}

void PID::setPreDefinedPID(uint8_t index) {
    if (index >= config.size()/6) //size正常的话应该是6的倍数
        return;
    PIDConfig readConfig;
    readConfig.kp = config[index*6];
    readConfig.ki = config[index*6+1];
    readConfig.kd = config[index*6+2];
    readConfig.min = config[index*6+3];
    readConfig.max = config[index*6+4];
    readConfig.target = config[index*6+5];
    setConfig(readConfig);
}

int8_t PID::getPreDefinedPIDCount() {
    return config.size()/6-1;
}

void PID::saveCurrentConfigAsPreDefined(uint8_t index) {
    if (index >=  (config.size()/6))
    {
        config.emplace_back(pidConfig.kp);
        config.emplace_back(pidConfig.ki);
        config.emplace_back(pidConfig.kd);
        config.emplace_back(pidConfig.min);
        config.emplace_back(pidConfig.max);
        config.emplace_back(pidConfig.target);

        logger.log(Loggr::INFO, "[PID] savePID Current Config!");
    }
    else
    {
        config[index*6] = pidConfig.kp;
        config[index*6+1] = pidConfig.ki;
        config[index*6+2] = pidConfig.kd;
        config[index*6+3] = pidConfig.min;
        config[index*6+4] = pidConfig.max;
        config[index*6+5] = pidConfig.target;
    }
}

std::vector<float> &PID::getData() {
    if (config.size()%6 != 0) {
        logger.log(Loggr::WARN, "[PID] config data error! All data will be clear!");
        config.clear();
    }
    if (!checkIsConfigExist(pidConfig)) {
        saveCurrentConfigAsPreDefined(getPreDefinedPIDCount() + 1);
    }
    return config;
}

bool PID::checkIsConfigExist(PIDConfig &data) {
    for (uint8_t i = 0; i < config.size() / 6; ++i) {
        if (config[i*6] == data.kp && config[i*6+1] == data.ki && config[i*6+2] == data.kd && config[i*6+3] == data.min && config[i*6+4] == data.max && config[i*6+5] == data.target)
        {
            return true;
        }
    }
    return false;
}

void PID::reloadConfig() {
    logger.log(Loggr::Debug, "[PID] reloadConfig");
    for (uint8_t i = 0; i < config.size() / 6; ++i) {
        logger.log(Loggr::Debug, "[PID] %d: %f %f %f %f %f %f", i, config[i*6], config[i*6+1], config[i*6+2], config[i*6+3], config[i*6+4], config[i*6+5]);
    }
    logger.log(Loggr::INFO,"共有%d个PID参数",config.size()/6);
    setPreDefinedPID(getPreDefinedPIDCount());
}

PID::PID():Commpent(14) {
    this->pidConfig = PIDConfig();

}


PIDConfig::PIDConfig() {
    this->kp = 0;
    this->ki = 0;
    this->kd = 0;
    this->min = -200;
    this->max = 200;
    this->target = 0;
}
