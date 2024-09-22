#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <stdarg.h>

void logging_init();

void logging_log(const char*msg, ...);


#endif
