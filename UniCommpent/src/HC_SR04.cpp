//
// Created by 神奇bug在哪里 on 7/12/23.
//

#include "HC_SR04.h"
#include "MicroTasks.h"
#include "MicroTasksInterrupt.h"
#include "SerialLogger.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
using namespace MicroTasks;

HC_SR04::HC_SR04(uint8_t trigPin, uint8_t echoPin) : Task(), GPIO(echoPin, INPUT), pinListener(this)  {
    trig = new GPIO(trigPin, OUTPUT);
}
Interrupt pinEvent(PB_0, FALLING);
void HC_SR04::setup() {
    GPIO::setup();
    trig->setup();
    pinEvent.Register(&pinListener);
    pinEvent.Attach();
    logger.log(Loggr::Debug, "[HC_SR04] 初始化完成");
}

unsigned long HC_SR04::loop(WakeReason reason) {
    if (WakeReason_Event == reason && pinEvent.IsTriggered())
    {
        IQRHandler();
    }
    if (WakeReason_Scheduled == reason)
    {
        if (isWorking) {
            result = 0;
            isWorking = false;
            logger.log(Loggr::ERROR, "[HC_SR04] 等待回应超时!");
        } else
        {
            update();
        }
    }
    // return when we next want to be called
    return timeout | MicroTasksClass::WaitForEvent;
}

void HC_SR04::IQRHandler() {
    //记录当前时间
    endTime = micros();
    result = endTime - startTime;

    //计算距离
    distance = result / 83;
    logger.log(Loggr::Debug, "[HC_SR04] 距离: %d cm", distance);
    isWorking = false;
}

uint32_t HC_SR04::getResult() const {
    return result;
}

uint32_t HC_SR04::getDistance() const {
    return distance;
}

void HC_SR04::update() {
    if (isWorking)
    { return;}
    isWorking = true;
    trig->setState(HIGH);
    delayMicroseconds(10);
    trig->setState(LOW);
    startTime = micros();
}

void HC_SR04::setTrig(GPIO *trig) {
    this->trig = trig;

}

void HC_SR04::setEcho(GPIO *echo) {
    this->echo = echo;

}

void HC_SR04::setTrig(uint8_t trig) {
    this->trig->setPin(trig);
}

void HC_SR04::setEcho(const uint8_t *echo) {
    this->echo->setPin(*echo);
}

void HC_SR04::setTimeout(uint32_t timeout) {
    this->timeout = timeout;
}

#pragma clang diagnostic pop