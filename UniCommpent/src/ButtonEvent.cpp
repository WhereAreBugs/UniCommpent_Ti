//
// Created by 神奇bug在哪里 on 7/29/23.
//

#include "ButtonEvent.h"
#include "UniCommpents.h"

ButtonEvent::~ButtonEvent() {
    delete pinEvent;
}

void ButtonEvent::setup() {
    pinMode(pin, INPUT);
    pinEvent = new MicroTasks::Interrupt(pin, edge);
    if (pinEvent == nullptr) {
        logger.log(Loggr::ERROR, "[ButtonEvent] new MicroTasks::Interrupt failed");
        return;
    }
    pinEvent->Register(&pinListener);
    pinEvent->Attach();
}

unsigned long ButtonEvent::loop(MicroTasks::WakeReason reason) {
    if (WakeReason_Event == reason)
    {
        if (pinEvent->IsTriggered())
        {
            logger.log(Loggr::Debug, "[ButtonEvent] Button pressed!");
            if (notifyTarget != nullptr)
            {
                notifyTarget->send(new ButtonTirrgerMessage(pin));
                logger.log(Loggr::Debug, "[ButtonEvent] MessageSend");
            }
            if (notifyFunc != nullptr)
            {
                notifyFunc(pin);
                logger.log(Loggr::Debug, "[ButtonEvent] func is called");
            }
        }
    }
    return MicroTasks::MicroTasksClass::Infinate|MicroTasks::MicroTasksClass::WaitForEvent;
}
