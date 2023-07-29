//
// Created by 神奇bug在哪里 on 7/28/23.
//

#ifndef TM4C_CONFIGSAVER_H
#define TM4C_CONFIGSAVER_H
#include "commpent.h"
#include "MicroTasks.h"
#include "/Users/cat/.platformio/packages/framework-energiativa/libraries/EEPROM/EEPROM.h"
#include <vector>
#include <map>
class RequestSaveMessage : public MicroTasks::Message {
private:
    static const uint32_t ID = 0x03;
public:
    explicit RequestSaveMessage() : Message(ID) {}
};

class ConfigSaver : public Commpent, virtual public MicroTasks::Task{
private:
    std::vector<std::pair<std::pair<uint32_t,Commpent *> ,std::vector<float>*>> targetData;
    int32_t autoSaveTimeMs = -1;
public:
    ConfigSaver():MicroTasks::Task(), Commpent(13){};
    void setup() override ;
    unsigned long loop(MicroTasks::WakeReason reason) override;
    void IQRHandler() override{};
    void save();
    /**
     * @brief 设置自动保存时间,设置为-1则不自动保存
     * @param newTime 单位为ms,默认值-1
     */
    void setSaveTime(int32_t newTime) {
        autoSaveTimeMs = newTime;
    }
    void load();
public:
    ///@brief 添加需要保存的数据
    void addData(Commpent * target);
    void addData(uint32_t uuid, std::vector<float>& data);
    void addData(uint32_t uuid, std::vector<float>*  data);

};


#endif //TM4C_CONFIGSAVER_H
