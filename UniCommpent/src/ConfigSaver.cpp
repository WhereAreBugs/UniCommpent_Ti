//
// Created by 神奇bug在哪里 on 7/28/23.
//

#include "ConfigSaver.h"
#include "UniCommpents.h"

void ConfigSaver::save() {
   uint8_t addrCount = 0;
    for(auto &i:targetData)
    {
         EEPROM.write(addrCount++,i.first.first); //写入uuid
         auto waitSaveData = i.first.second->getDataCopy();
         EEPROM.write(addrCount++,waitSaveData.size()); //写入数据的个数
         for(auto &j:waitSaveData)
         {
             //每个数据4个字节，分四次写入
                EEPROM.write(addrCount++,((uint8_t*)&j)[0]);
                EEPROM.write(addrCount++,((uint8_t*)&j)[1]);
                EEPROM.write(addrCount++,((uint8_t*)&j)[2]);
                EEPROM.write(addrCount++,((uint8_t*)&j)[3]);
                logger.log(Loggr::INFO,"[ConfigSaver] Save data:%f",j);
         }
         //写入结束标志
            EEPROM.write(addrCount++,0xFF);
    }
    logger.log(Loggr::INFO,"[ConfigSaver] Save success!");
}

void ConfigSaver::load() {
    //读取数据
    uint8_t addrCount = 0;
    uint32_t uuid;
    uint8_t dataCount;
    while(1)
    {
        uuid = EEPROM.read(addrCount++);
        if(uuid == 0xFF)
            break;
        dataCount = EEPROM.read(addrCount++);
        std::vector<float> *data = new std::vector<float>(dataCount);
        for(int i = 0;i < dataCount;i++)
        {
            float temp;
            ((uint8_t*)&temp)[0] = EEPROM.read(addrCount++);
            ((uint8_t*)&temp)[1] = EEPROM.read(addrCount++);
            ((uint8_t*)&temp)[2] = EEPROM.read(addrCount++);
            ((uint8_t*)&temp)[3] = EEPROM.read(addrCount++);
            data->at(i) = temp;
            logger.log(Loggr::INFO,"[ConfigSaver] Load data:%f",temp);
        }
        for(auto &i:targetData)
        {
            if(i.first.first == uuid)
            {
                *i.second = *data;
                break;
            }
        }
        delete data;
    }
    for(auto &i:targetData)
    {
        i.first.second->reloadConfig();
    }
    logger.log(Loggr::INFO,"[ConfigSaver] Load success!");
}

void ConfigSaver::setup() {

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

void ConfigSaver::addData(Commpent *target) {
    targetData.emplace_back(std::pair<uint32_t,Commpent *>(target->getUUID(),target),&target->getData());
}


void std::__throw_out_of_range(char const*)
{
    logger.log(Loggr::ERROR,"[ConfigSaver] Out of range!");
    while (true);
}