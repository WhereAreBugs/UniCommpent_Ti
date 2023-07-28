//
// Created by 神奇bug在哪里 on 7/26/23.
//
#include <thread>
#include "SerialLogger.h"
void std::__throw_bad_alloc() {
    logger.log(Loggr::FATAL, "std::__throw_length_error");
    while (true);
}
void std::__throw_length_error(char const*){
    logger.log(Loggr::FATAL, "std::__throw_length_error");
    while (true);
}