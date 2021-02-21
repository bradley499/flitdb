#pragma once
#ifndef misc_h
#define misc_h

unsigned int strcmp(const char *word1, const char *word2);
unsigned long strlen(const char *text);
char *strncpy(char *s1, const char *s2, unsigned long n);
unsigned short to_short(char *chars);
signed long long to_long_long(char *chars);
double to_double(char *chars);
float to_float(char *chars);
char *to_char_array(double number);

#endif