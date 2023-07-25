//
// Created by 神奇bug在哪里 on 7/12/23.
//

#include "Loggr.h"


void Loggr::log(Loggr::LOG_LEVEL level, const char *format, ...) {
    if (level < logLevel) return;
}

String Loggr::getLogLevelString(Loggr::LOG_LEVEL level) {
    return level == Debug ? "[DEBUG]" : level == INFO ? "[INFO]" : level == WARN ? "[WARN]" : level == ERROR ? "[ERROR]" : "[FATAL]";
}

void Loggr::setLogLevel(Loggr::LOG_LEVEL level) {
    logLevel = level;
}
void __error__(char* file, unsigned long line)
{
    Serial.print("[Assert Error]");
    Serial.print("File: ");
    Serial.print(file);
    Serial.print("Line: ");
    Serial.print(line);
    while(1);
}