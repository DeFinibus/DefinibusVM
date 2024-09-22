
#include "logging.h"
#include <stdio.h>
#include <stdlib.h>
//TODO only stdout logging impelented using printf
void logging_init()
{

}

void logging_log(const char*format, ...)
{
  va_list argp;
  va_start(argp, format);
  while (*format != '\0') {
    if (*format == '%') {
      format++;
      if (*format == '%') {
        putchar('%');
      } else if (*format == 'c') {
        char char_to_print = va_arg(argp, int);
        putchar(char_to_print);
      } else if (*format == 'd') {
        int number = va_arg(argp, int);
        char str[100];
        snprintf(str,100,"%d",number);
        int idx = 0;
        while(str[idx] != '\0') {
            putchar(str[idx]);
            idx++;
        }
      }
    } else {
      putchar(*format);
    }
    format++;
  }
  va_end(argp);
}
