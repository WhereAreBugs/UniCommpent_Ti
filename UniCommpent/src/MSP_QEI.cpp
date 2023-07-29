//
// Created by 神奇bug在哪里 on 7/24/23.
//

#include <sysctl.h>

#include <pin_map.h>
#include "MSP_QEI.h"
#include "SerialLogger.h"
#include "MicroTasks.h"
#include "MicroTasksTask.h"
void QEI0_handler();
void QEI1_handler();
void MSP_QEI::setup() {
    if (QEI_BASE == QEI0_BASE) {
        SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        while ((!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)) || (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))) {}
        pinMode(PD_6, INPUT_PULLUP);
        pinMode(PF_1, INPUT_PULLUP);
        // 配置 PB4 和 PB5 为 QEI 功能
        GPIOPinConfigure(0x00031806); //PD6_PHA0
        GPIOPinConfigure(0x00050406); //PF1_PHB0
        GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6);
        GPIOPinTypeQEI(GPIO_PORTF_BASE, GPIO_PIN_1);
        // 配置 QEI0
        QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_RESET_IDX | QEI_CONFIG_QUADRATURE, INT32_MAX);
        QEIEnable(QEI0_BASE);
        QEIPositionSet(QEI0_BASE, INT32_MAX/2);
        QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, 500000);
        QEIVelocityEnable(QEI0_BASE);
        QEIIntRegister(QEI0_BASE, []() { QEI0_handler(); });
        QEIIntEnable(QEI0_BASE, QEI_INTINDEX);

        logger.log(Loggr::Debug, "QEI0 Setup");
    }
    else
    {
        SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)){}
        pinMode(PC_5, INPUT_PULLUP);
        pinMode(PC_6, INPUT_PULLUP);
        GPIOPinConfigure(0x00021406); //PC5_PHA1
        GPIOPinConfigure(0x00021806); //PC6_PHB1
        GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5|GPIO_PIN_6);
        QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_RESET_IDX | QEI_CONFIG_QUADRATURE | QEI_CONFIG_SWAP, INT32_MAX);
        QEIEnable(QEI1_BASE);
        QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, 500000);
        QEIVelocityEnable(QEI1_BASE);
        QEIIntRegister(QEI1_BASE, []() { QEI1_handler(); });
        QEIIntEnable(QEI1_BASE, QEI_INTINDEX);
        QEIPositionSet(QEI1_BASE, INT32_MAX/2);
        logger.log(Loggr::Debug, "QEI1 Setup");
    }

}

int64_t MSP_QEI::getPositionValue() const {
    auto result = static_cast<int64_t>(QEIPositionGet(QEI_BASE));
    return result-(INT32_MAX/2);
}

void MSP_QEI::IQRHandler() {
    logger.log(Loggr::LOG_LEVEL::Debug, "QEI IQR");
    /***
     * @brief 如果后续需要则在这里添加中断处理代码
     */
}

unsigned long MSP_QEI::loop(MicroTasks::WakeReason reason) {
    auto position = getPositionValue();
    auto velocity = getVelocityValue();
    if (this->QEI_BASE == QEI0_BASE)
        logger.log(Loggr::Debug, "[QEI0]current value: %lld,%lld",position,velocity);
    else
        logger.log(Loggr::Debug, "[QEI1]current value: %lld,%lld",position,velocity);
    return readFrecuency;
}

int64_t MSP_QEI::getVelocityValue() const {
    if (QEI_BASE == QEI0_BASE)
        return QEIDirectionGet(QEI0_BASE)*static_cast<int64_t>(QEIVelocityGet(QEI0_BASE));
    else
        return QEIDirectionGet(QEI1_BASE)*static_cast<int64_t>(QEIVelocityGet(QEI1_BASE));
}

MSP_QEI::MSP_QEI(uint32_t QEI_BASE) : Commpent(10) {
    if (QEI_BASE != QEI0_BASE && QEI_BASE != QEI1_BASE) {
        logger.log(Loggr::ERROR, "QEI_BASE Select ERROR");
    }
    this->QEI_BASE = QEI_BASE;
}

void QEI1_handler() {
    logger.log(Loggr::Debug,"QEI1 IQR: ERROR:%d, DIR:%d, TIMER:%d, INDEX:%d", QEIIntStatus(QEI1_BASE, true)&QEI_INTERROR,
               QEIIntStatus(QEI1_BASE, true)&QEI_INTDIR, QEIIntStatus(QEI1_BASE, true)&QEI_INTTIMER,
               QEIIntStatus(QEI1_BASE, true)&QEI_INTINDEX);
    QEIIntClear(QEI1_BASE, QEIIntStatus(QEI1_BASE, true));
}


void QEI0_handler() {
    logger.log(Loggr::Debug, "QEI0 IQR: ERROR:%d, DIR:%d, TIMER:%d, INDEX:%d", QEIIntStatus(QEI0_BASE, true)&QEI_INTERROR,
               QEIIntStatus(QEI0_BASE, true)&QEI_INTDIR, QEIIntStatus(QEI0_BASE, true)&QEI_INTTIMER,
               QEIIntStatus(QEI0_BASE, true)&QEI_INTINDEX);
    QEIIntClear(QEI0_BASE, QEIIntStatus(QEI0_BASE, true));
}
