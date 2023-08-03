//
// Created by 神奇bug在哪里 on 8/2/23.
//

#include <hw_ints.h>

#include "tm4c123gh6pm.h"
#include "FrecTest.h"
#include "sysctl.h"
#include "timer.h"
#include "SerialLogger.h"
#include "TimerEvent.h"
#include <hw_timer.h>
#include <interrupt.h>

TimerEvent IQR;
void FrecTest::setup() {
    //这里需要启用PE4的定时器输入捕获功能
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)||!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    GPIOPinTypeTimer(GPIO_PORTB_BASE,GPIO_PIN_4);
    GPIOPinConfigure(GPIO_PB4_T1CCP0);
    GPIODirModeSet( GPIO_PORTB_BASE,  GPIO_PIN_4,
                    GPIO_DIR_MODE_HW);
    GPIOPadConfigSet( GPIO_PORTB_BASE,  GPIO_PIN_4,
                      GPIO_STRENGTH_2MA,  GPIO_PIN_TYPE_STD_WPU);
    TimerConfigure(timer, TIMER_CFG_PERIODIC | TIMER_CFG_A_CAP_COUNT_UP);
    TimerClockSourceSet(timer, TIMER_CLOCK_PIOSC);
    TimerControlEvent(timer, TIMER_A, TIMER_EVENT_POS_EDGE);
    TimerIntRegister(timer, TIMER_A, []()
    {
        logger.log(Loggr::Debug, "[FrecTest] IQR");
        IQR.trigger();
    });
    IntEnable(INT_TIMER1A);
    TimerIntEnable(timer, TIMER_CAPA_MATCH);
    IntMasterEnable();
    TimerLoadSet(timer, TIMER_A, 0xffffffff);
    TimerMatchSet(timer, TIMER_A, 0xffffffff);
    TimerEnable(timer, TIMER_A);
    IQR.Register(listener);
}

unsigned long FrecTest::loop(MicroTasks::WakeReason reason) {
    if (reason == WakeReason_Scheduled)
    {
        //这里需要读取定时器的值
        uint32_t value = TimerValueGet(timer, TIMER_A);
        //这里需要计算频率
        TimerValueClear(timer, TIMER_A);
        frec = value - 0.001*value+1;
        //清除定时器的值
        logger.log(Loggr::Debug, "[FrecTest] Timer: %lld Hz", frec);
    }
    if (reason == WakeReason_Message)
    {
        MicroTasks::Message * msg;
        if (this->receive(msg))
        {
            if (msg->id() == 0x83)
            {
                delete msg;
                TimerMatchSet(timer, TIMER_A, 0);
                return 1000;
            }
        }
    }
    if (WakeReason_Event == reason)
    {
        if (IQR.IsTriggered())
        {
            IQRHandler();
            return 1000;
        }
    }
    return 1000|MicroTasks::MicroTasksClass::WaitForEvent;
}

uint64_t FrecTest::getFrec() const {
    return frec;
}

void FrecTest::IQRHandler() {
    TimerIntClear(timer, TIMER_CAPA_MATCH);

}

void FrecTest::TimerValueClear(uint32_t timerBase, uint32_t Timer) {
    TimerDisable(timerBase, Timer);
    if (Timer == TIMER_A)
    {
        HWREG(timerBase + TIMER_O_TAV) = static_cast<uint32_t>(0);
    } else
    {
        HWREG(timerBase + TIMER_O_TBV) = static_cast<uint32_t>(0);
    }
    TimerEnable(timerBase, Timer);
}


FrecTest::~FrecTest() = default;
