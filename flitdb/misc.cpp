#pragma once
#ifndef misc_cpp
#define misc_cpp

#include "misc.h"

unsigned int strcmp(const char *word1, const char *word2)
{
	while (*word1 && *word1 == *word2)
	{
		++word1;
		++word2;
	}
	return (int)(unsigned char)(*word1) - (int)(unsigned char)(*word2);
}

unsigned long strlen(const char *text)
{
	unsigned long i = 0;
	while (text[i] != '\0')
		i++;
	return i;
}

char *strncpy(char *s1, const char *s2, unsigned long n)
{
	char *rc = s1;
	while ((n > 0) && (*s1++ = *s2++))
		--n;
	while (n-- > 1)
		*s1++ = '\0';
	return rc;
}

#endif