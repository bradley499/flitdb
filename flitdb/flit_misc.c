#pragma once
#ifndef misc_c
#define misc_c

#include "flit_misc.h"
#include <stdbool.h>

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

unsigned short to_short(char *chars)
{
	unsigned short i = 0;
	for (i = 0; i < strlen(chars); i++)
	{
		switch (chars[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		default:
			return -1;
		}
	}
	unsigned short short_value = 0;
	for (unsigned short ii = 0; ii < i; ++ii)
		short_value = short_value * 10 + chars[ii] - '0';
	return short_value;
}

signed long long to_long_long(char *chars)
{
	unsigned short i = 0;
	bool negative = false;
	for (i = 0; i < strlen(chars); i++)
	{
		switch (chars[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		case '-':
		{
			if (!negative)
				negative = true;
			else
				return -1;
			break;
		}
		default:
			return -1;
		}
	}
	signed long long long_long_value = 0;
	for (unsigned short ii = 0; ii < i; ++ii)
		long_long_value = long_long_value * 10 + chars[ii] - '0';
	return (((negative) ? -1 : 1) * long_long_value);
}

double to_double(char *chars)
{
	unsigned short i = 0;
	signed short decimalized = -1;
	bool negative = false;
	for (i = 0; i < strlen(chars); i++)
	{
		switch (chars[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		case '.':
		{
			if (decimalized == -1 && i > 0)
				decimalized = i;
			else
				return -1;
			break;
		}
		case '-':
		{
			if (!negative)
				negative = true;
			else
				return -1;
			break;
		}
		default:
			return -1;
		}
	}
	double double_value = 0;
	for (unsigned short ii = 0; ii < i; ++ii)
	{
		if (chars[ii] != '.' && chars[ii] != '-')
			double_value = double_value * 10 + chars[ii] - '0';
	}
	i -= 1;
	if (decimalized != -1)
	{
		while (decimalized < i)
		{
			decimalized += 1;
			double_value /= 10;
		}
	}
	if (negative)
		double_value = -double_value;
	return double_value;
}

float to_float(char *chars)
{
	return (float)to_double(chars);
}

#endif