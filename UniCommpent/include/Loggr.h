//
// Created by 神奇bug在哪里 on 7/12/23.
//

#ifndef TM4C_LOGGR_H
#define TM4C_LOGGR_H

#include "commpent.h"
class Loggr : public Commpent {
public:
    enum LOG_LEVEL {
        Debug,
        INFO,
        WARN,
        ERROR,
        FATAL
    };
    virtual void log(LOG_LEVEL level, const char *format, ...);
    void setLogLevel(LOG_LEVEL level);
    Loggr(uint32_t uuid):Commpent(uuid){};
protected:
    LOG_LEVEL logLevel = Debug;
    static String getLogLevelString(LOG_LEVEL level);
};
void __error__(char*, unsigned long);

#endif //TM4C_LOGGR_H
