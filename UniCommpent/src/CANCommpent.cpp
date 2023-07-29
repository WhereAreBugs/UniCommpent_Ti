//
// Created by 神奇bug在哪里 on 7/28/23.
//

#include "CANCommpent.h"
#include "UniCommpents.h"

size_t CANCommpent::write(uint8_t address, const uint8_t *buffer, size_t size) {
    logger.log(Loggr::FATAL,"[CAN] trying to call a none completed function");
    return -1;
}
