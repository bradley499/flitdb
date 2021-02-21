#ifndef misc_h
#define misc_h

#include "flit_misc.c"

unsigned short to_short(char *chars);
signed long long to_long_long(char *chars);
double to_double(char *chars);
float to_float(char *chars);
char *to_char_array(double number);

#endif