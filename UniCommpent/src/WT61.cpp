//
// Created by 神奇bug在哪里 on 7/28/23.
//

#include "UniCommpents.h"
#include "WT1_IMU.h"
#include <functional>
#include "WT61_SDK.h"



void WT61_SDK::WitDeInit(void) {

}

int32_t WT61_SDK::WitWriteReg(uint32_t uiReg, uint16_t usData) {
    uint16_t usCRC;
    uint8_t ucBuff[8];
    if(uiReg >= REGSIZE)return WIT_HAL_INVAL;
    switch(s_uiProtoclo)
    {
        case WIT_PROTOCOL_NORMAL:
            if(serial == nullptr)return WIT_HAL_EMPTY;
            ucBuff[0] = 0xFF;
            ucBuff[1] = 0xAA;
            ucBuff[2] = uiReg & 0xFF;
            ucBuff[3] = usData & 0xff;
            ucBuff[4] = usData >> 8;
            serial->write(ucBuff, 5);
            break;
        case WIT_PROTOCOL_MODBUS:
            if(serial == nullptr)return WIT_HAL_EMPTY;
            ucBuff[0] = s_ucAddr;
            ucBuff[1] = FuncW;
            ucBuff[2] = uiReg >> 8;
            ucBuff[3] = uiReg & 0xFF;
            ucBuff[4] = usData >> 8;
            ucBuff[5] = usData & 0xff;
            usCRC = __CRC16(ucBuff, 6);
            ucBuff[6] = usCRC >> 8;
            ucBuff[7] = usCRC & 0xff;
            serial->write(ucBuff, 8);
            break;
        case WIT_PROTOCOL_CAN:
            if(canCommpent == nullptr)return WIT_HAL_EMPTY;
            ucBuff[0] = 0xFF;
            ucBuff[1] = 0xAA;
            ucBuff[2] = uiReg & 0xFF;
            ucBuff[3] = usData & 0xff;
            ucBuff[4] = usData >> 8;
            canCommpent->write(s_ucAddr, ucBuff, 5);
            break;
        case WIT_PROTOCOL_I2C:
            if(wireHandler == nullptr)return WIT_HAL_EMPTY;
            ucBuff[0] = usData & 0xff;
            ucBuff[1] = usData >> 8;
            wireHandler->beginTransmission(s_ucAddr);
            wireHandler->write(uiReg & 0xFF);
            wireHandler->write(ucBuff, 2);
            wireHandler->endTransmission();
            break;
        default:
            return WIT_HAL_INVAL;
    }
    return WIT_HAL_OK;
}

int32_t WT61_SDK::WitReadReg(uint32_t uiReg, uint32_t uiReadNum) {
    uint16_t usTemp, i;
    uint8_t ucBuff[8];
    if((uiReg + uiReadNum) >= REGSIZE)return WIT_HAL_INVAL;
    switch(s_uiProtoclo)
    {
        case WIT_PROTOCOL_NORMAL:
            if(uiReadNum > 4)return WIT_HAL_INVAL;
            if(serial == nullptr)return WIT_HAL_EMPTY;
            ucBuff[0] = 0xFF;
            ucBuff[1] = 0xAA;
            ucBuff[2] = 0x27;
            ucBuff[3] = uiReg & 0xff;
            ucBuff[4] = uiReg >> 8;
            serial->write(ucBuff, 5);
            break;
        case WIT_PROTOCOL_MODBUS:
            if(serial == nullptr)return WIT_HAL_EMPTY;
            usTemp = uiReadNum << 1;
            if((usTemp + 5) > WIT_DATA_BUFF_SIZE)return WIT_HAL_NOMEM;
            ucBuff[0] = s_ucAddr;
            ucBuff[1] = FuncR;
            ucBuff[2] = uiReg >> 8;
            ucBuff[3] = uiReg & 0xFF;
            ucBuff[4] = uiReadNum >> 8;
            ucBuff[5] = uiReadNum & 0xff;
            usTemp = __CRC16(ucBuff, 6);
            ucBuff[6] = usTemp >> 8;
            ucBuff[7] = usTemp & 0xff;
            serial->write(ucBuff, 8);
            break;
        case WIT_PROTOCOL_CAN:
            if(uiReadNum > 3)return WIT_HAL_INVAL;
            if(canCommpent == nullptr)return WIT_HAL_EMPTY;
            ucBuff[0] = 0xFF;
            ucBuff[1] = 0xAA;
            ucBuff[2] = 0x27;
            ucBuff[3] = uiReg & 0xff;
            ucBuff[4] = uiReg >> 8;
            canCommpent->write(s_ucAddr, ucBuff, 5);
            break;
        case WIT_PROTOCOL_I2C:
            if(wireHandler == nullptr)return WIT_HAL_EMPTY;
            usTemp = uiReadNum << 1;
            if(WIT_DATA_BUFF_SIZE < usTemp)return WIT_HAL_NOMEM;
            logger.log(Loggr::FATAL,"Trying to call a none completed function");
            break;
        default:
            return WIT_HAL_INVAL;
    }
    s_uiReadRegIndex = uiReg;

    return WIT_HAL_OK;
}

int32_t WT61_SDK::WitInit(uint32_t uiProtocol, uint8_t ucAddr) {
    if(uiProtocol > WIT_PROTOCOL_I2C)return WIT_HAL_INVAL;
    s_uiProtoclo = uiProtocol;
    s_ucAddr = ucAddr;
    s_uiWitDataCnt = 0;
    return WIT_HAL_OK;
}

void WT61_SDK::WitCanDataIn(uint8_t *ucData, uint8_t ucLen) {
    logger.log(Loggr::FATAL,"Trying to call a none completed function");
}

void WT61_SDK::WitSerialDataIn(uint8_t ucData) {
    uint16_t usCRC16, usTemp, i, usData[4];
    uint8_t ucSum;

    s_ucWitDataBuff[s_uiWitDataCnt++] = ucData;
    switch(s_uiProtoclo)
    {
        case WIT_PROTOCOL_NORMAL:
            if(s_ucWitDataBuff[0] != 0x55)
            {
                s_uiWitDataCnt--;
                memcpy(s_ucWitDataBuff, &s_ucWitDataBuff[1], s_uiWitDataCnt);
                return ;
            }
            if(s_uiWitDataCnt >= 11)
            {
                ucSum = __CaliSum(s_ucWitDataBuff, 10);
                if(ucSum != s_ucWitDataBuff[10])
                {
                    s_uiWitDataCnt--;
                    memcpy(s_ucWitDataBuff, &s_ucWitDataBuff[1], s_uiWitDataCnt);
                    return ;
                }
                usData[0] = ((uint16_t)s_ucWitDataBuff[3] << 8) | (uint16_t)s_ucWitDataBuff[2];
                usData[1] = ((uint16_t)s_ucWitDataBuff[5] << 8) | (uint16_t)s_ucWitDataBuff[4];
                usData[2] = ((uint16_t)s_ucWitDataBuff[7] << 8) | (uint16_t)s_ucWitDataBuff[6];
                usData[3] = ((uint16_t)s_ucWitDataBuff[9] << 8) | (uint16_t)s_ucWitDataBuff[8];
                CopeWitData(s_ucWitDataBuff[1], usData, 4);
                s_uiWitDataCnt = 0;
            }
            break;
        case WIT_PROTOCOL_MODBUS:
            if(s_uiWitDataCnt > 2)
            {
                if(s_ucWitDataBuff[1] != FuncR)
                {
                    s_uiWitDataCnt--;
                    memcpy(s_ucWitDataBuff, &s_ucWitDataBuff[1], s_uiWitDataCnt);
                    return ;
                }
                if(s_uiWitDataCnt < (s_ucWitDataBuff[2] + 5))return ;
                usTemp = ((uint16_t)s_ucWitDataBuff[s_uiWitDataCnt-2] << 8) | s_ucWitDataBuff[s_uiWitDataCnt-1];
                usCRC16 = __CRC16(s_ucWitDataBuff, s_uiWitDataCnt-2);
                if(usTemp != usCRC16)
                {
                    s_uiWitDataCnt--;
                    memcpy(s_ucWitDataBuff, &s_ucWitDataBuff[1], s_uiWitDataCnt);
                    return ;
                }
                usTemp = s_ucWitDataBuff[2] >> 1;
                for(i = 0; i < usTemp; i++)
                {
                    sReg[i+s_uiReadRegIndex] = ((uint16_t)s_ucWitDataBuff[(i<<1)+3] << 8) | s_ucWitDataBuff[(i<<1)+4];
                }
                //TODO: 读取数据
                s_uiWitDataCnt = 0;
            }
            break;
        case WIT_PROTOCOL_CAN:
        case WIT_PROTOCOL_I2C:
            s_uiWitDataCnt = 0;
            break;
    }
    if(s_uiWitDataCnt == WIT_DATA_BUFF_SIZE)s_uiWitDataCnt = 0;

}

void WT61_SDK::CopeWitData(uint8_t ucIndex, uint16_t *p_data, uint32_t uiLen) {
    uint32_t uiReg1 = 0, uiReg2 = 0, uiReg1Len = 0, uiReg2Len = 0;
    uint16_t *p_usReg1Val = p_data;
    uint16_t *p_usReg2Val = p_data+3;

    uiReg1Len = 4;
    switch(ucIndex)
    {
        case WIT_ACC:   uiReg1 = AX;    uiReg1Len = 3;  uiReg2 = TEMP;  uiReg2Len = 1;  break;
        case WIT_ANGLE: uiReg1 = Roll;  uiReg1Len = 3;  uiReg2 = VERSION;  uiReg2Len = 1;  break;
        case WIT_TIME:  uiReg1 = YYMM;	break;
        case WIT_GYRO:  uiReg1 = GX;  uiLen = 3;break;
        case WIT_MAGNETIC: uiReg1 = HX;  uiLen = 3;break;
        case WIT_DPORT: uiReg1 = D0Status;  break;
        case WIT_PRESS: uiReg1 = PressureL;  break;
        case WIT_GPS:   uiReg1 = LonL;  break;
        case WIT_VELOCITY: uiReg1 = GPSHeight;  break;
        case WIT_QUATER:    uiReg1 = Q0;  break;
        case WIT_GSA:   uiReg1 = SVNUM;  break;
        case WIT_REGVALUE:  uiReg1 = s_uiReadRegIndex;  break;
        default:
            return ;

    }
    if(uiLen == 3)
    {
        uiReg1Len = 3;
        uiReg2Len = 0;
    }
    if(uiReg1Len)
    {

        memcpy(&sReg[uiReg1], p_usReg1Val, uiReg1Len<<1);
        //TODO: 从数组中获取数据
    }

    if(uiReg2Len)
    {
        memcpy(&sReg[uiReg2], p_usReg2Val, uiReg2Len<<1);
        //TODO: 从数组中获取数据
    }
}

int32_t WT61_SDK::WitStartAccCali(void) {
    /*
	First place the equipment horizontally, and then perform the following operations
*/
    if(WitWriteReg(KEY, KEY_UNLOCK) != WIT_HAL_OK)	    return  WIT_HAL_ERROR;// unlock reg
    if(s_uiProtoclo == WIT_PROTOCOL_MODBUS) delay(20);
    else if(s_uiProtoclo == WIT_PROTOCOL_NORMAL) delay(1);
    else ;
    if(WitWriteReg(CALSW, CALGYROACC) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    return WIT_HAL_OK;
}

int32_t WT61_SDK::WitStopAccCali(void) {
    if(WitWriteReg(CALSW, NORMAL) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    if(s_uiProtoclo == WIT_PROTOCOL_MODBUS)	delay(20);
    else if(s_uiProtoclo == WIT_PROTOCOL_NORMAL) delay(1);
    else ;
    if(WitWriteReg(SAVE, SAVE_PARAM) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    return WIT_HAL_OK;
}

int32_t WT61_SDK::WitStartMagCali(void) {
    if(WitWriteReg(KEY, KEY_UNLOCK) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    if(s_uiProtoclo == WIT_PROTOCOL_MODBUS)	delay(20);
    else if(s_uiProtoclo == WIT_PROTOCOL_NORMAL) delay(1);
    else ;
    if(WitWriteReg(CALSW, CALMAGMM) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    return WIT_HAL_OK;
}

int32_t WT61_SDK::WitStopMagCali(void) {
    if(WitWriteReg(KEY, KEY_UNLOCK) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    if(s_uiProtoclo == WIT_PROTOCOL_MODBUS)	delay(20);
    else if(s_uiProtoclo == WIT_PROTOCOL_NORMAL) delay(1);
    else ;
    if(WitWriteReg(CALSW, NORMAL) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    return WIT_HAL_OK;
}

int32_t WT61_SDK::WitSetUartBaud(int32_t uiBaudIndex) {
    if(!CheckRange(uiBaudIndex,WIT_BAUD_4800,WIT_BAUD_230400))
    {
        return WIT_HAL_INVAL;
    }
    if(WitWriteReg(KEY, KEY_UNLOCK) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    if(s_uiProtoclo == WIT_PROTOCOL_MODBUS)	delay(20);
    else if(s_uiProtoclo == WIT_PROTOCOL_NORMAL) delay(1);
    else ;
    if(WitWriteReg(BAUD, uiBaudIndex) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    return WIT_HAL_OK;
}

char WT61_SDK::CheckRange(short sTemp, short sMin, short sMax) {
    if(WitWriteReg(KEY, KEY_UNLOCK) != WIT_HAL_OK)	    return  WIT_HAL_ERROR;// unlock reg
    if(s_uiProtoclo == WIT_PROTOCOL_MODBUS)	delay(20);
    else if(s_uiProtoclo == WIT_PROTOCOL_NORMAL) delay(1);
    else ;
    if(WitWriteReg(CALSW, CALGYROACC) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    return WIT_HAL_OK;
}

int32_t WT61_SDK::WitSetBandwidth(int32_t uiBaudWidth) {
    if(!CheckRange(uiBaudWidth,BANDWIDTH_256HZ,BANDWIDTH_5HZ))
    {
        return WIT_HAL_INVAL;
    }
    if(WitWriteReg(KEY, KEY_UNLOCK) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    if(s_uiProtoclo == WIT_PROTOCOL_MODBUS)	delay(20);
    else if(s_uiProtoclo == WIT_PROTOCOL_NORMAL) delay(1);
    else ;
    if(WitWriteReg(BANDWIDTH, uiBaudWidth) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    return WIT_HAL_OK;
}

int32_t WT61_SDK::WitSetOutputRate(int32_t uiRate) {
    if(!CheckRange(uiRate,RRATE_02HZ,RRATE_NONE))
    {
        return WIT_HAL_INVAL;
    }
    if(WitWriteReg(KEY, KEY_UNLOCK) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    if(s_uiProtoclo == WIT_PROTOCOL_MODBUS)	delay(20);
    else if(s_uiProtoclo == WIT_PROTOCOL_NORMAL) delay(1);
    else ;
    if(WitWriteReg(RRATE, uiRate) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    return WIT_HAL_OK;
}

int32_t WT61_SDK::WitSetContent(int32_t uiRsw) {
    if(!CheckRange(uiRsw,RSW_TIME,RSW_MASK))
    {
        return WIT_HAL_INVAL;
    }
    if(WitWriteReg(KEY, KEY_UNLOCK) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    if(s_uiProtoclo == WIT_PROTOCOL_MODBUS)	delay(20);
    else if(s_uiProtoclo == WIT_PROTOCOL_NORMAL) delay(1);
    else ;
    if(WitWriteReg(RSW, uiRsw) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    return WIT_HAL_OK;
}

int32_t WT61_SDK::WitSetCanBaud(int32_t uiBaudIndex) {
    if(!CheckRange(uiBaudIndex,CAN_BAUD_1000000,CAN_BAUD_3000))
    {
        return WIT_HAL_INVAL;
    }
    if(WitWriteReg(KEY, KEY_UNLOCK) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    if(s_uiProtoclo == WIT_PROTOCOL_MODBUS)	delay(20);
    else if(s_uiProtoclo == WIT_PROTOCOL_NORMAL) delay(1);
    else ;
    if(WitWriteReg(BAUD, uiBaudIndex) != WIT_HAL_OK)	return  WIT_HAL_ERROR;
    return WIT_HAL_OK;
}

void WT61_SDK::setup() {
    WitInit(WIT_PROTOCOL_NORMAL, 0xff);
    WitSetUartBaud(WIT_BAUD_115200);
    WitSetBandwidth(BANDWIDTH_256HZ);
    WitSetOutputRate(RRATE_50HZ);

}
