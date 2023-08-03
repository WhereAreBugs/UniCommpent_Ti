//
// Created by 神奇bug在哪里 on 8/2/23.
//

#ifndef TM4C_TIMEREVENT_H
#define TM4C_TIMEREVENT_H
#include "MicroTasks.h"
class TimerEvent : public MicroTasks::Event {
public:
    void trigger();
};


#endif //TM4C_TIMEREVENT_H
