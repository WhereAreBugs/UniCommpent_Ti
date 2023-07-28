//
// Created by 神奇bug在哪里 on 7/28/23.
//

#include "ConfigSaver.h"
#include "UniCommpents.h"

void ConfigSaver::save() {
    int addr = 0x00;
    for(auto i : targetData) {
        if (i == nullptr) {
            logger.log(Loggr::ERROR,"[ConfigSaver] data is nullptr");
            continue;
        }
        for (int j = 0; j < sizeof i; ++j) {
            EEPROM.update(addr, *reinterpret_cast<uint8_t *>(i+j));
        }
    }
}

void ConfigSaver::load() {
    int addr = 0x00;
    for(auto i : targetData) {
        if (i == nullptr) {
            logger.log(Loggr::ERROR,"[ConfigSaver] data is nullptr");
            continue;
        }
        for (int j = 0; j < sizeof i; ++j) {
            *reinterpret_cast<uint8_t *>(i+j) = EEPROM.read(addr);
        }
    }
}

void ConfigSaver::setup() {
    load();
}

void ConfigSaver::addData(float *data) {
    targetData.push_back(data);
}

unsigned long ConfigSaver::loop(MicroTasks::WakeReason reason) {
    if (autoSaveTimeMs == -1)
        return MicroTasks::MicroTasksClass::Infinate | MicroTasks::MicroTasksClass::WaitForMessage;
    if (reason == WakeReason_Scheduled)
    {
        save();
    }
    if (reason == WakeReason_Message)
    {
        MicroTasks::Message *msg;
        if (this->receive(msg))
        {
            if (msg->id() == 0x03)
            {
                save();
            }
            delete msg;
        }
    }
    return autoSaveTimeMs | MicroTasks::MicroTasksClass::WaitForMessage;
}

void ConfigSaver::addData(float **data, int size) {
    for (int i = 0; i < size; ++i) {
        addData(data[i]);
    }

}

void ConfigSaver::addData(float &data) {
    addData(&data);
}

void ConfigSaver::addData(std::vector<float> &data) {
    for (auto &i : data) {
        addData(i);
    }

}

void ConfigSaver::addData(std::vector<float *> &data) {
    for (auto &i : data) {
        addData(i);
    }
}



