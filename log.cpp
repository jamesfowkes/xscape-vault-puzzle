/* System Includes */

#include <stdio.h>
#include <stdarg.h>

/* Arduino Includes */

#include "Arduino.h"

/* Local Includes */

#include "log.h"

#ifndef ARDUINO
#include <iostream>
#define platform_print(l) std::cout << l
#define platform_println(l) std::cout << l << std::endl
#else
#define platform_print(l) Serial.print(l)
#define platform_println(l) Serial.println(l)
#endif

void p(char const * const fmt, ...)
{
    char buf[LOG_BUFFER_SIZE];
    va_list args;
    va_start (args, fmt); vsnprintf(buf, LOG_BUFFER_SIZE, fmt, args); va_end (args);
    platform_print(buf);
}

void pln(char const * const fmt, ...)
{
    char buf[LOG_BUFFER_SIZE];
    va_list args;
    va_start (args, fmt); vsnprintf(buf, LOG_BUFFER_SIZE, fmt, args); va_end (args);
    platform_println(buf);
}
