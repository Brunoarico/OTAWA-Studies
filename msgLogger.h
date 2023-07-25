#ifndef MSGLOGGER_H
#define MSGLOGGER_H
#include <elm/io.h>
#include <iostream>



class msgLogger {
   public:
    static void echoError(elm::String msg);
    static void echoInfo(elm::String msg);
    static void echoWarning(elm::String msg);
};

#endif  // MSGLOGGER_H
