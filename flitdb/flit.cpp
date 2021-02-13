#pragma once
#ifndef flit_cpp
#define flit_cpp

#include "flit.h"

int flitdb_setup(const char *filename, flitdb *handler, int flags)
{
	return handler->setup(filename, flags);
}

char *flitdb_errmsg(flitdb *handler)
{
	return handler->get_err_message();
}

int flitdb_extract(flitdb *handler, uint64_t column_position, uint64_t row_position)
{
	return handler->read_at(column_position, row_position);
}

int flitdb_retrieved_type(flitdb *handler)
{
	return handler->retrieve_value_type();
}

int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, signed long long int value)
{
	handler->insert_value(value);
	return handler->insert_at(column_position, row_position);
}

int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, long double value)
{
	handler->insert_value(value);
	return handler->insert_at(column_position, row_position);
}

int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, float value)
{
	handler->insert_value(value);
	return handler->insert_at(column_position, row_position);
}

int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, char *value)
{
	handler->insert_value(value);
	return handler->insert_at(column_position, row_position);
}

int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, const char *value)
{
	int value_length = strlen(value);
	char tmp_value[(value_length + 1)];
	tmp_value[value_length] = '\0';
	for (size_t i = value_length; i > 0; i--)
		tmp_value[(i - 1)] = value[(i - 1)];
	handler->insert_value(tmp_value);
	return handler->insert_at(column_position, row_position);
}

int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, bool value)
{
	handler->insert_value(value);
	return handler->insert_at(column_position, row_position);
}

int flitdb_delete(flitdb *handler, uint64_t column_position, uint64_t row_position)
{
	handler->insert_reset();
	return handler->insert_at(column_position, row_position);
}

signed long long int flitdb_retrieve_int(flitdb *handler)
{
	return handler->retrieve_value_int();
}

long double flitdb_retrieve_double(flitdb *handler)
{
	return handler->retrieve_value_double();
}

float flitdb_retrieve_float(flitdb *handler)
{
	return handler->retrieve_value_float();
}

char *flitdb_retrieve_char(flitdb *handler)
{
	return handler->retrieve_value_char();
}

bool flitdb_retrieve_bool(flitdb *handler)
{
	return handler->retrieve_value_bool();
}

#endif