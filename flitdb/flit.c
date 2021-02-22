#ifndef flit_c
#define flit_c

#include <stdlib.h>
#include <string.h>
#include "flit_handler.h"
#include "flit.h"
#include "flit_misc.h"

#ifdef __cplusplus
#define flitdb_extern extern "C"
#else
#define flitdb_extern extern
#endif

flitdb_extern int flitdb_setup(const char *filename, flitdb **handler, int flags)
{
	if (!flitdb_new(handler))
	{
		*handler = NULL;
		return FLITDB_ERROR;
	}
	return flitdb_connection_setup(handler, filename, flags);
}

flitdb_extern int flitdb_close(flitdb **handler)
{
	flitdb_destroy(handler);
	free(*handler);
	return FLITDB_DONE;
}

flitdb_extern char *flitdb_errmsg(flitdb **handler)
{
	return flitdb_get_err_message(handler);
}

flitdb_extern int flitdb_extract(flitdb **handler, unsigned short column_position, unsigned short row_position)
{
	return flitdb_read_at(handler, column_position, row_position);
}

int flitdb_retrieved_type(flitdb **handler)
{
	return flitdb_retrieve_value_type(handler);
}

flitdb_extern int flitdb_insert_int(flitdb **handler, unsigned short column_position, unsigned short row_position, signed long long int value)
{
	flitdb_insert_value_int(handler, value);
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned short column_position, unsigned short row_position, signed long long int value)
{
	return flitdb_insert_int(handler, column_position, row_position, value);
}

#endif

flitdb_extern int flitdb_insert_double(flitdb **handler, unsigned short column_position, unsigned short row_position, long double value)
{
	flitdb_insert_value_double(handler, value);
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned short column_position, unsigned short row_position, long double value)
{
	return flitdb_insert_double(handler, column_position, row_position, value);
}

#endif

flitdb_extern int flitdb_insert_float(flitdb **handler, unsigned short column_position, unsigned short row_position, float value)
{
	flitdb_insert_value_float(handler, value);
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned short column_position, unsigned short row_position, float value)
{
	return flitdb_insert_float(handler, column_position, row_position, value);
}

#endif

flitdb_extern int flitdb_insert_char(flitdb **handler, unsigned short column_position, unsigned short row_position, char *value)
{
	flitdb_insert_value_char(handler, value);
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned short column_position, unsigned short row_position, char *value)
{
	return flitdb_insert_char(handler, column_position, row_position, value);
}

#endif

flitdb_extern int flitdb_insert_const_char(flitdb **handler, unsigned short column_position, unsigned short row_position, const char *value)
{
	int value_length = strlen(value);
	char tmp_value[(value_length + 1)];
	tmp_value[value_length] = '\0';
	for (size_t i = value_length; i > 0; i--)
		tmp_value[(i - 1)] = value[(i - 1)];
	return flitdb_insert_char(handler, column_position, row_position, tmp_value);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned short column_position, unsigned short row_position, const char *value)
{
	return flitdb_insert_const_char(handler, column_position, row_position, value);
}

#endif

flitdb_extern int flitdb_insert_bool(flitdb **handler, unsigned short column_position, unsigned short row_position, bool value)
{
	flitdb_insert_value_bool(handler, value);
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned short column_position, unsigned short row_position, bool value)
{
	return flitdb_insert_bool(handler, column_position, row_position, value);
}

#endif

flitdb_extern int flitdb_delete(flitdb **handler, unsigned short column_position, unsigned short row_position)
{
	flitdb_insert_reset(handler);
	return flitdb_insert_at(handler, column_position, row_position);
}

flitdb_extern signed long long int flitdb_retrieve_int(flitdb **handler)
{
	return flitdb_retrieve_value_int(handler);
}

flitdb_extern long double flitdb_retrieve_double(flitdb **handler)
{
	return flitdb_retrieve_value_double(handler);
}

flitdb_extern float flitdb_retrieve_float(flitdb **handler)
{
	return flitdb_retrieve_value_float(handler);
}

flitdb_extern char *flitdb_retrieve_char(flitdb **handler)
{
	return flitdb_retrieve_value_char(handler);
}

flitdb_extern bool flitdb_retrieve_bool(flitdb **handler)
{
	return flitdb_retrieve_value_bool(handler);
}

#endif