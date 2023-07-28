//
// Created by 神奇bug在哪里 on 7/26/23.
//

#include "SerialChecker.h"
#include "SerialLogger.h"

void SerialChecker::setup() {
    serial->setTimeout(10);
    serial->setBufferSize(128,128);// Tx、Rx
    serial->begin(115200);
}

unsigned long SerialChecker::loop(MicroTasks::WakeReason reason) {
    if (reason == WakeReason_Scheduled) {
        if (serial->getWriteError()) {
            serial->clearWriteError();
            return 10|MicroTasks::MicroTasksClass::WaitForMessage;
        }
        if (serial->available()) {
            IQRHandler();
        }
    }
    if (reason == WakeReason_Message)
    {

    }
    return 10|MicroTasks::MicroTasksClass::WaitForMessage;
}

void SerialChecker::onData(void (*newFunc)(String)) {
        dataHandler = newFunc;
}

void SerialChecker::IQRHandler() {
        logger.log(Loggr::Debug, "[SerialChecker] IQRHandler");
        if (dataHandler != nullptr) {
            auto command = serial->readStringUntil(terminator);
//            serial->flush();
            dataHandler(command);
        } else {
            logger.log(Loggr::ERROR, "[SerialChecker] dataHandler is nullptr");
        }
}
