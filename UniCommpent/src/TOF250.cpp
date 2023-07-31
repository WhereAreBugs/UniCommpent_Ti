//
// Created by 神奇bug在哪里 on 7/27/23.
//

#include "TOF250.h"
#include "SerialLogger.h"
#define REG_00H 0x00
#define REG_01L 0x04
void TOF250::setup() {
    if (SerialMode)
    {
        serialHandler->begin(9600);
    }
    else
    {
        if (wireHandler == nullptr)
        {
            logger.log(Loggr::ERROR, "TOF250: wireHandler is nullptr");
            return;
        }
        logger.log(Loggr::INFO, "TOF250: begin");
        wireHandler->beginTransmission(address);
        wireHandler->write(0x09);//配置发送模式
        wireHandler->write(0x01);//配置发送模式
        wireHandler->endTransmission();
        wireHandler->beginTransmission(address);
        wireHandler->write(0x08);//配置数据模式
        wireHandler->write(0x00);//设置滤波模式
        wireHandler->endTransmission();
    }
}

unsigned long TOF250::loop(MicroTasks::WakeReason reason) {
    if (reason == WakeReason_Scheduled)
    {
        if (SerialMode)
        {
            if (serialHandler->available()) {
                Get_liarDataFromSerial();
                serialHandler->flush();
            }
        }
        else {
            Get_LidarDataFromIIC();
        }
        logger.log(Loggr::Debug, "TOF250: result %d",result);
    }
    if (reason == WakeReason_Event)
    {
        IQRHandler();
    }
    return updateInterval;
}

void TOF250::Get_LidarDataFromIIC() {
        uint8_t i = 0;
        byte rx_buf[2] = {0};

        wireHandler->beginTransmission(address);
        wireHandler->write(REG_00H);               //向TOF250 00H地址开始要求获取数据
        wireHandler->endTransmission();
        wireHandler->requestFrom(address, static_cast<uint8_t>(2));      //读取2个byte长度的数据
        while ( wireHandler->available())
        {
            rx_buf[i] = wireHandler->read();         //每次接收一个byte的数据
            i++;
            if (i >= 2)
            {
                i = 0;
                result = rx_buf[0] * 256 + rx_buf[1]; //高8位数据
            }
        }
}

void TOF250::Get_liarDataFromSerial() {
        String intchars = "";
        String chars = "";
        while (serialHandler->available() > 0) {
            char inchar = serialHandler->read();
            if (isDigit(inchar))          //是数字就放在intchars数字字符串
            {
                intchars += inchar;         //数字---字符串
            } else {
                chars += inchar;            //非数字---字符串
            }
            delay(2);                       //等待延时，不同波特率延时时间不一样，需要细调
            if (serialHandler->available() == 0)    //等待一段时间后缓冲区仍然没有收到数据,认为一帧数据已经接收完成
            {
                result = intchars.toInt();        //将数字字符串转换成整数
            }
        }
}
