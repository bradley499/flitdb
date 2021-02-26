#ifndef flit_c
#define flit_c

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define __WINDOWS__
#warning "File locking is currently not supported in Windows builds"
#define flitdb_truncate _chsize
#include <io.h>
#else
#define flitdb_truncate ftruncate
#include <sys/file.h>
#endif
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "flit.h"

const unsigned long long flitdb_max_size();

// FlitDB database operations
bool flitdb_new(flitdb **handler);
void flitdb_destroy(flitdb **handler);
int flitdb_connection_setup(flitdb **handler, const char *filename, int flags);
char *flitdb_get_err_message(flitdb **handler);
int flitdb_read_at(flitdb **handler, unsigned short column_position, unsigned short row_position);
void flitdb_clear_values(flitdb **handler);
int flitdb_insert_at(flitdb **handler, unsigned long long int column_position, unsigned short row_position);
int flitdb_insert_value_int(flitdb **handler, signed long long set_value);
int flitdb_insert_value_double(flitdb **handler, long double set_value);
int flitdb_insert_value_float(flitdb **handler, float set_value);
int flitdb_insert_value_char(flitdb **handler, char *set_value);
int flitdb_insert_value_bool(flitdb **handler, bool set_value);
int flitdb_insert_reset(flitdb **handler);
signed long long int flitdb_retrieve_value_int(flitdb **handler);
long double flitdb_retrieve_value_double(flitdb **handler);
float flitdb_retrieve_value_float(flitdb **handler);
char *flitdb_retrieve_value_char(flitdb **handler);
bool flitdb_retrieve_value_bool(flitdb **handler);
int flitdb_retrieve_value_type(flitdb **handler);

// FlitDB miscellaneous functions 
signed short flitdb_to_short(char *chars);
signed long long flitdb_to_long_long(char *chars);
double flitdb_to_double(char *chars);
float flitdb_to_float(char *chars);
long long unsigned int flitdb_abs(long long signed int value);

int flitdb_setup(const char *filename, flitdb **handler, int flags)
{
	if (!flitdb_new(handler))
	{
		*handler = NULL;
		return FLITDB_ERROR;
	}
	return flitdb_connection_setup(handler, filename, flags);
}

int flitdb_close(flitdb **handler)
{
	flitdb_destroy(handler);
	free(*handler);
	return FLITDB_DONE;
}

char *flitdb_errmsg(flitdb **handler)
{
	return flitdb_get_err_message(handler);
}

int flitdb_extract(flitdb **handler, unsigned long long int column_position, unsigned short row_position)
{
	return flitdb_read_at(handler, column_position, row_position);
}

int flitdb_retrieved_type(flitdb **handler)
{
	return flitdb_retrieve_value_type(handler);
}

int flitdb_insert_int(flitdb **handler, unsigned long long int column_position, unsigned short row_position, signed long long int value)
{
	flitdb_insert_value_int(handler, value);
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, signed long long int value)
{
	return flitdb_insert_int(handler, column_position, row_position, value);
}

#endif

int flitdb_insert_double(flitdb **handler, unsigned long long int column_position, unsigned short row_position, long double value)
{
	flitdb_insert_value_double(handler, value);
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, long double value)
{
	return flitdb_insert_double(handler, column_position, row_position, value);
}

#endif

int flitdb_insert_float(flitdb **handler, unsigned long long int column_position, unsigned short row_position, float value)
{
	flitdb_insert_value_float(handler, value);
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, float value)
{
	return flitdb_insert_float(handler, column_position, row_position, value);
}

#endif

int flitdb_insert_char(flitdb **handler, unsigned long long int column_position, unsigned short row_position, char *value)
{
	flitdb_insert_value_char(handler, value);
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, char *value)
{
	return flitdb_insert_char(handler, column_position, row_position, value);
}

#endif

int flitdb_insert_const_char(flitdb **handler, unsigned long long int column_position, unsigned short row_position, const char *value)
{
	int value_length = strlen(value);
	char tmp_value[(value_length + 1)];
	tmp_value[value_length] = '\0';
	for (size_t i = value_length; i > 0; i--)
		tmp_value[(i - 1)] = value[(i - 1)];
	return flitdb_insert_char(handler, column_position, row_position, tmp_value);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, const char *value)
{
	return flitdb_insert_const_char(handler, column_position, row_position, value);
}

#endif

int flitdb_insert_bool(flitdb **handler, unsigned long long int column_position, unsigned short row_position, bool value)
{
	flitdb_insert_value_bool(handler, (value == true));
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, bool value)
{
	return flitdb_insert_bool(handler, column_position, row_position, value);
}

#endif

int flitdb_delete(flitdb **handler, unsigned long long int column_position, unsigned short row_position)
{
	flitdb_insert_reset(handler);
	return flitdb_insert_at(handler, column_position, row_position);
}

signed long long int flitdb_retrieve_int(flitdb **handler)
{
	return flitdb_retrieve_value_int(handler);
}

long double flitdb_retrieve_double(flitdb **handler)
{
	return flitdb_retrieve_value_double(handler);
}

float flitdb_retrieve_float(flitdb **handler)
{
	return flitdb_retrieve_value_float(handler);
}

char *flitdb_retrieve_char(flitdb **handler)
{
	return flitdb_retrieve_value_char(handler);
}

bool flitdb_retrieve_bool(flitdb **handler)
{
	return flitdb_retrieve_value_bool(handler);
}

#define FLITDB_MAX_BUFFER_SIZE 1024
#define FLITDB_MAX_CHAR_LENGTH 10000
#define FLITDB_MAX_ERR_SIZE 100
#define FLITDB_COLUMN_POSITION_MAX 10000
#define FLITDB_ROW_POSITION_MAX 1000

typedef struct flitdb
{
	char buffer[FLITDB_MAX_BUFFER_SIZE];
	bool configured;
	FILE *file_descriptor;
	char err_message[FLITDB_MAX_ERR_SIZE];
	struct value
	{
		signed long long int int_value;
		long double double_value;
		float float_value;
		char char_value[FLITDB_MAX_CHAR_LENGTH];
		bool bool_value;
	} value;
	unsigned char value_type;
	bool value_retrieved;
	size_t size;
	bool read_only;
	bool unsafe;
} flitdb;

void flitdb_clear_values(flitdb **handler)
{
	(*handler)->value.int_value = 0;
	(*handler)->value.double_value = 0;
	(*handler)->value.float_value = 0;
	(*handler)->value.bool_value = false;
	strncpy((*handler)->value.char_value, "\0", sizeof((*handler)->value.char_value));
	(*handler)->value_type = FLITDB_NULL;
	(*handler)->value_retrieved = false;
	strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
}

bool flitdb_new(flitdb **handler)
{
	*handler = &*(flitdb *)malloc(sizeof(flitdb));
	if (*handler == NULL)
		return false;
	(*handler)->configured = (FLITDB_MAX_BUFFER_SIZE < 50 || FLITDB_MAX_BUFFER_SIZE > 1024);
	(*handler)->size = 0;
	(*handler)->read_only = false;
	(*handler)->unsafe = false;
	(*handler)->value_type = 0;
	(*handler)->value_retrieved = false;
	(*handler)->file_descriptor = NULL;
	if (FLITDB_MAX_BUFFER_SIZE < 50)
		strncpy((*handler)->err_message, "The minimum buffer size has encroached beyond suitable definitions\0", FLITDB_MAX_ERR_SIZE);
	else if (FLITDB_MAX_BUFFER_SIZE > 1024)
		strncpy((*handler)->err_message, "The maximum buffer size has encroached beyond suitable definitions\0", FLITDB_MAX_ERR_SIZE);
	else
		strncpy((*handler)->err_message, "\0", FLITDB_MAX_ERR_SIZE);
	flitdb_clear_values(handler);
	return true;
}

void flitdb_destroy(flitdb **handler)
{
	if ((*handler)->configured)
	{
		(*handler)->configured = false;
		#ifndef __WINDOWS__
		flock(fileno((*handler)->file_descriptor), LOCK_UN);
		#endif
		fclose((*handler)->file_descriptor);
	}
}

const unsigned long long flitdb_max_size()
{
	unsigned long long insertion_area[2] = {(FLITDB_COLUMN_POSITION_MAX * FLITDB_ROW_POSITION_MAX), (FLITDB_ROW_POSITION_MAX > 1 ? ((FLITDB_COLUMN_POSITION_MAX * (FLITDB_ROW_POSITION_MAX - 1)) * 8) : 0)};
	return ((insertion_area[0] * 10000) + insertion_area[1] + (FLITDB_COLUMN_POSITION_MAX * 15));
}

int flitdb_connection_setup(flitdb **handler, const char *filename, int flags)
{
	if ((*handler)->configured)
	{
		if (FLITDB_MAX_BUFFER_SIZE >= 50 && FLITDB_MAX_BUFFER_SIZE <= 1024)
			strncpy((*handler)->err_message, "The database handler has already been attributed to handle another database\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	(*handler)->size = 0;
	if ((flags & FLITDB_READONLY) == FLITDB_READONLY)
		(*handler)->read_only = true;
	bool file_exists = true;
	struct stat buffer;
	if (stat(filename, &buffer) != 0)
	{
		file_exists = false;
		if ((*handler)->read_only)
		{
			if ((flags & FLITDB_CREATE) == 0)
			{
				strncpy((*handler)->err_message, "No database exists to be exclusively read\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_NOT_FOUND;
			}
		}
	}
	(*handler)->file_descriptor = fopen(filename, (((flags & FLITDB_READONLY) == FLITDB_READONLY) ? "r" : ((((flags & FLITDB_CREATE) == FLITDB_CREATE) && !file_exists) ? "w+" : "r+")));
	if ((*handler)->file_descriptor == NULL)
	{
		strncpy((*handler)->err_message, "Failed to open the database\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_PERM;
	}
	else
	{
		fseek((*handler)->file_descriptor, 0L, SEEK_END);
		(*handler)->size = ftell((*handler)->file_descriptor);
		fseek((*handler)->file_descriptor, 0L, SEEK_SET);
	}
	#ifndef __WINDOWS__
	if (flock(fileno((*handler)->file_descriptor), LOCK_EX | LOCK_NB) != 0)
	{
		strncpy((*handler)->err_message, "Exclusive rights to access the database could not be obtained\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_BUSY;
	}
	#endif
	if ((flags & FLITDB_UNSAFE) == FLITDB_UNSAFE)
		(*handler)->unsafe = true;
	else if ((*handler)->size > flitdb_max_size())
	{
		strncpy((*handler)->err_message, "The database attempted to access has a larger size than what this object can read\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_RANGE;
	}
	(*handler)->configured = true;
	return FLITDB_SUCCESS;
}

char *flitdb_get_err_message(flitdb **handler)
{
	if (*handler == NULL)
		return (char *)"This handler failed to be setup\0";
	return (*handler)->err_message;
}

int flitdb_insert_value_int(flitdb **handler, signed long long int set_value)
{
	if (!(*handler)->configured)
	{
		strncpy((*handler)->err_message, (char *)"The database handler has not been attributed to handle a database\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to unexpected tennant\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);
	(*handler)->value_type = FLITDB_INTEGER;
	(*handler)->value.int_value = set_value;
	return FLITDB_DONE;
}

int flitdb_insert_value_double(flitdb **handler, long double set_value)
{
	if (!(*handler)->configured)
	{
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to unexpected tennant\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);
	(*handler)->value_type = FLITDB_DOUBLE;
	(*handler)->value.double_value = set_value;
	return FLITDB_DONE;
}

int flitdb_insert_value_float(flitdb **handler, float set_value)
{
	if (!(*handler)->configured)
	{
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to unexpected tennant\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);
	(*handler)->value_type = FLITDB_FLOAT;
	(*handler)->value.float_value = set_value;
	return FLITDB_DONE;
}

int flitdb_insert_value_char(flitdb **handler, char *set_value)
{
	if (!(*handler)->configured)
	{
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to unexpected tennant\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	if (strlen(set_value) >= FLITDB_MAX_CHAR_LENGTH)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to the length of a string being too large\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);
	(*handler)->value_type = FLITDB_CHAR;
	strncpy((*handler)->value.char_value, set_value, sizeof((*handler)->value.char_value));
	return FLITDB_DONE;
}

int flitdb_insert_value_bool(flitdb **handler, bool set_value)
{
	if (!(*handler)->configured)
	{
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to unexpected tennant\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);
	(*handler)->value_type = FLITDB_BOOL;
	(*handler)->value.bool_value = set_value;
	return FLITDB_DONE;
}

int flitdb_insert_reset(flitdb **handler)
{
	flitdb_clear_values(handler);
	return FLITDB_DONE;
}

signed long long flitdb_retrieve_value_int(flitdb **handler)
{
	return (*handler)->value.int_value;
}

long double flitdb_retrieve_value_double(flitdb **handler)
{
	return (*handler)->value.double_value;
}

float flitdb_retrieve_value_float(flitdb **handler)
{
	return (*handler)->value.float_value;
}

char *flitdb_retrieve_value_char(flitdb **handler)
{
	return (*handler)->value.char_value;
}

bool flitdb_retrieve_value_bool(flitdb **handler)
{
	return (*handler)->value.bool_value;
}

int flitdb_retrieve_value_type(flitdb **handler)
{
	return (*handler)->value_type;
}

int flitdb_read_at(flitdb **handler, unsigned short column_position, unsigned short row_position)
{
	if (!(*handler)->configured)
	{
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);
	if (column_position == 0 || column_position > FLITDB_COLUMN_POSITION_MAX || row_position == 0 || row_position > FLITDB_ROW_POSITION_MAX)
	{
		strncpy((*handler)->err_message, "The requested range was outside of the database's range\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_RANGE;
	}
	row_position -= 1;
	bool store_response = false;
	size_t offset = 0;
	size_t skip_offset = 0;
	unsigned char read_length = 15;
	unsigned short row_count = 0;
	unsigned short row_position_count = 0;
	while (true)
	{
		if ((offset + read_length) >= (*handler)->size)
		{
			if ((offset + read_length) > ((*handler)->size + read_length))
			{
				strncpy((*handler)->err_message, "The database contracted a malformed structure declaration\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_CORRUPT;
			}
			else
				break;
		}
		strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
		fseek((*handler)->file_descriptor, offset, SEEK_SET);
		offset += read_length;
		if (fread((*handler)->buffer, read_length, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
		if (strlen((*handler)->buffer) != read_length)
		{
			strncpy((*handler)->err_message, "The database contracted a malformed structure declaration\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_CORRUPT;
		}
		if (read_length == 15)
		{
			char skip_amount_read[5];
			skip_amount_read[4] = '\0';
			skip_amount_read[0] = (*handler)->buffer[0];
			skip_amount_read[1] = (*handler)->buffer[1];
			skip_amount_read[2] = (*handler)->buffer[2];
			skip_amount_read[3] = (*handler)->buffer[3];
			if (flitdb_to_short(skip_amount_read) < 0)
			{
				strncpy((*handler)->err_message, "Skip offset negation detected\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			skip_offset += (flitdb_to_short(skip_amount_read) + 1);
			if (skip_offset > column_position)
				return FLITDB_NULL;
			char row_count_read[4];
			row_count_read[3] = '\0';
			row_count_read[0] = (*handler)->buffer[4];
			row_count_read[1] = (*handler)->buffer[5];
			row_count_read[2] = (*handler)->buffer[6];
			row_count = flitdb_to_short(row_count_read) + 1;
			row_position_count = 0;
		}
		unsigned char set_read_length = 15;
		if (skip_offset == column_position)
		{
			char position_read[4];
			position_read[3] = '\0';
			position_read[0] = (*handler)->buffer[(read_length < 15) ? 0 : 7];
			position_read[1] = (*handler)->buffer[(read_length < 15) ? 1 : 8];
			position_read[2] = (*handler)->buffer[(read_length < 15) ? 2 : 9];
			unsigned short position = flitdb_to_short(position_read);
			if (position == row_position)
			{
				store_response = true;
				row_count = 0;
			}
		}
		else if (row_position_count == row_count)
			row_count = 0;
		row_position_count += 1;
		if (row_count > 1)
		{
			row_count -= 1;
			set_read_length = 8;
		}
		char response_length_read[5];
		response_length_read[4] = '\0';
		response_length_read[0] = (*handler)->buffer[(read_length < 15) ? 3 : 10];
		response_length_read[1] = (*handler)->buffer[(read_length < 15) ? 4 : 11];
		response_length_read[2] = (*handler)->buffer[(read_length < 15) ? 5 : 12];
		response_length_read[3] = (*handler)->buffer[(read_length < 15) ? 6 : 13];
		unsigned short response_length = (flitdb_to_short(response_length_read) + 1);
		unsigned char data_type;
		switch ((*handler)->buffer[(read_length < 15) ? 7 : 14])
		{
		case '1':
			data_type = FLITDB_INTEGER;
			break;
		case '2':
			data_type = FLITDB_DOUBLE;
			break;
		case '3':
			data_type = FLITDB_FLOAT;
			break;
		case '4':
			data_type = FLITDB_CHAR;
			break;
		case '5':
			data_type = FLITDB_BOOL;
			break;
		default:
		{
			strncpy((*handler)->err_message, "The database yielded an invalid datatype\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_CORRUPT;
		}
		}
		if (response_length_read == 0)
		{
			strncpy((*handler)->err_message, "A reference to an imposed data declaration holds no length\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_CORRUPT;
		}
		else if (data_type == FLITDB_BOOL && response_length != 1)
		{
			strncpy((*handler)->err_message, "The database holds a boolean of a possible elongated length\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_CORRUPT;
		}
		if (store_response)
		{
			response_length++;
			(*handler)->value_type = data_type;
			(*handler)->value_retrieved = true;
			char response_value[response_length];
			char response_value_tmp[response_length];
			response_length--;
			strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
			strncpy(response_value, "\0", sizeof(response_value));
			strncpy(response_value_tmp, "\0", sizeof(response_value_tmp));
			fseek((*handler)->file_descriptor, offset, SEEK_SET);
			if (fread(response_value_tmp, response_length, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
			{
				strncpy((*handler)->err_message, "Unable to allocate memory to retrieve value from the database\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			strncpy(response_value, response_value_tmp, sizeof(response_value));
			if (strlen(response_value) == 0)
			{
				strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			switch (data_type)
			{
			case FLITDB_INTEGER:
				(*handler)->value.int_value = flitdb_to_long_long(response_value);
				break;
			case FLITDB_DOUBLE:
				(*handler)->value.double_value = flitdb_to_double(response_value);
				break;
			case FLITDB_FLOAT:
				(*handler)->value.float_value = flitdb_to_float(response_value);
				break;
			case FLITDB_CHAR:
				strncpy((*handler)->value.char_value, response_value, sizeof((*handler)->value.char_value));
				break;
			case FLITDB_BOOL:
				(*handler)->value.bool_value = (response_value[0] == '1');
				break;
			default:
				break;
			}
			return FLITDB_DONE;
		}
		read_length = set_read_length;
		offset += response_length;
	}
	return FLITDB_NULL;
}

int flitdb_insert_at(flitdb **handler, unsigned long long int column_position, unsigned short row_position)
{
	if (!(*handler)->configured)
	{
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", FLITDB_MAX_ERR_SIZE);
		flitdb_clear_values(handler);
		return FLITDB_READONLY;
	}
	if (!(*handler)->unsafe)
	{
		if (column_position == 0 || column_position > FLITDB_COLUMN_POSITION_MAX || row_position == 0 || row_position > FLITDB_ROW_POSITION_MAX)
		{
			strncpy((*handler)->err_message, "The requested range was outside of the database's range\0", FLITDB_MAX_ERR_SIZE);
			flitdb_clear_values(handler);
			return FLITDB_RANGE;
		}
	}
	else if (column_position == 0 || row_position == 0)
	{
		strncpy((*handler)->err_message, "The requested range must have a valid starting range of at least 1\0", FLITDB_MAX_ERR_SIZE);
		flitdb_clear_values(handler);
		return FLITDB_RANGE;
	}
	else if (row_position > FLITDB_ROW_POSITION_MAX)
	{
		strncpy((*handler)->err_message, "The row position must be less than, or equal to 1000\0", FLITDB_MAX_ERR_SIZE);
		flitdb_clear_values(handler);
		return FLITDB_RANGE;
	}
	row_position -= 1;
	char *input_buffer = NULL;
	unsigned short input_size = 0;
	switch ((*handler)->value_type)
	{
	case FLITDB_INTEGER:
	{
		input_buffer = (char *)calloc(64,sizeof(char));
		int ret = snprintf(input_buffer, 64, "%lld", (*handler)->value.int_value);
		if (ret <= 0)
		{
			if (input_buffer != NULL)
				free(input_buffer);
			strncpy((*handler)->err_message, "Value encoding of type: int; has failed\0", FLITDB_MAX_ERR_SIZE);
			flitdb_clear_values(handler);
			return FLITDB_RANGE;
		}
		input_size = strlen(input_buffer);
		break;
	}
	case FLITDB_DOUBLE:
	{
		input_buffer = (char *)calloc(96,sizeof(char));
		int ret = snprintf(input_buffer, 96, "%Lf", (*handler)->value.double_value);
		if (ret <= 0)
		{
			if (input_buffer != NULL)
				free(input_buffer);
			strncpy((*handler)->err_message, "Value encoding of type: double; has failed\0", FLITDB_MAX_ERR_SIZE);
			flitdb_clear_values(handler);
			return FLITDB_RANGE;
		}
		for (unsigned char i = 87; i > 0; --i)
		{
			if (input_buffer[i] == '0' || input_buffer[i] == '\0')
				input_buffer[i] = '\0';
			else
				break;
		}

		input_size = strlen(input_buffer);
		break;
	}
	case FLITDB_FLOAT:
	{
		input_buffer = (char *)calloc(32,sizeof(char));
		int ret = snprintf(input_buffer, 32, "%f", (*handler)->value.float_value);
		if (ret <= 0)
		{
			if (input_buffer != NULL)
				free(input_buffer);
			strncpy((*handler)->err_message, "Value encoding of type: float; has failed\0", FLITDB_MAX_ERR_SIZE);
			flitdb_clear_values(handler);
			return FLITDB_RANGE;
		}
		for (unsigned char i = 87; i > 0; --i)
		{
			if (input_buffer[i] == '0' || input_buffer[i] == '\0')
				input_buffer[i] = '\0';
			else
				break;
		}
		input_size = strlen(input_buffer);
		break;
	}
	case FLITDB_CHAR:
	{
		input_size = strlen((*handler)->value.char_value);
		input_buffer =  (char *)calloc((input_size + 1),sizeof(char));
		input_buffer[input_size] = '\0';
		strncpy(input_buffer, (*handler)->value.char_value, input_size);
		break;
	}
	case FLITDB_BOOL:
	{
		input_buffer = (char *)calloc(1,sizeof(char));
		if ((*handler)->value.bool_value)
			input_buffer[0] = '1';
		else
			input_buffer[0] = '0';
		input_size = 1;
		break;
	}
	}
	char size_buffer[4];
	strncpy(size_buffer, "\0", 4);
	{
		bool input_size_default = false;
		if (input_size > 0)
		{
			input_size_default = true;
			input_size -= 1;
		}
		int ret = snprintf(size_buffer, sizeof(size_buffer), "%d", (int)input_size);
		if (ret <= 0)
		{
			if (input_buffer != NULL)
				free(input_buffer);
			strncpy((*handler)->err_message, "Length determination failed\0", FLITDB_MAX_ERR_SIZE);
			flitdb_clear_values(handler);
			return FLITDB_ERROR;
		}
		if (input_size_default)
			input_size += 1;
	}
	if (input_size < 10)
	{
		size_buffer[3] = size_buffer[0];
		size_buffer[2] = '0';
		size_buffer[1] = '0';
		size_buffer[0] = '0';
	}
	else if (input_size < 100)
	{
		size_buffer[3] = size_buffer[1];
		size_buffer[2] = size_buffer[0];
		size_buffer[1] = '0';
		size_buffer[0] = '0';
	}
	else if (input_size < 1000)
	{
		size_buffer[3] = size_buffer[2];
		size_buffer[2] = size_buffer[1];
		size_buffer[1] = size_buffer[0];
		size_buffer[0] = '0';
	}
	const unsigned short input_size_default = input_size;
	size_t offset[4] = {0, 0, 0, 0};
	unsigned short skip_offset[2] = {0, 0};
	unsigned char read_length[2] = {15, 15};
	signed short row_count[2] = {0, 0};
	size_t current_length = 0;
	unsigned char insertion_state[2] = {2, 0};
	bool sub_reference = false;
	while ((*handler)->size > 0)
	{
		current_length = 0;
		strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
		fseek((*handler)->file_descriptor, offset[0], SEEK_SET);
		offset[0] += read_length[0];
		offset[1] = offset[0];
		if (fread((*handler)->buffer, read_length[0], sizeof(char), (*handler)->file_descriptor) != sizeof(char))
		{
			if (offset[0] < (*handler)->size)
			{
				strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			else if (offset[0] == (*handler)->size)
				break;
		}
		if (strlen((*handler)->buffer) == 0)
		{
			current_length = 0;
			if (skip_offset[0] < column_position)
				insertion_state[0] = 2;
			if (sub_reference)
				insertion_state[0] = 1;
			offset[0] -= read_length[0];
			offset[1] = offset[0];
			if (skip_offset[1] == (column_position - 1))
			{
				offset[0] = offset[3];
				offset[1] = offset[3];
			}
			current_length = 0;
			read_length[0] = read_length[1];
			break;
		}
		else if (strlen((*handler)->buffer) != read_length[0])
		{
			strncpy((*handler)->err_message, "The database contracted a malformed structure declaration\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_CORRUPT;
		}
		bool segment_detection = false;
		insertion_state[1] = insertion_state[0];
		if (read_length[0] == 15)
		{
			offset[3] = (offset[1] - read_length[0]);
			insertion_state[0] = 2;
			char skip_amount_read[5];
			skip_amount_read[4] = '\0';
			skip_amount_read[0] = (*handler)->buffer[0];
			skip_amount_read[1] = (*handler)->buffer[1];
			skip_amount_read[2] = (*handler)->buffer[2];
			skip_amount_read[3] = (*handler)->buffer[3];
			signed short skip_amount = flitdb_to_short(skip_amount_read);
			if (skip_amount < 0)
			{
				if (input_buffer != NULL)
					free(input_buffer);
				strncpy((*handler)->err_message, "Skip offset negation detected\0", FLITDB_MAX_ERR_SIZE);
				flitdb_clear_values(handler);
				return FLITDB_CORRUPT;
			}
			skip_offset[1] = skip_offset[0];
			skip_offset[0] += (skip_amount + 1);
			char row_count_read[4];
			row_count_read[3] = '\0';
			row_count_read[0] = (*handler)->buffer[4];
			row_count_read[1] = (*handler)->buffer[5];
			row_count_read[2] = (*handler)->buffer[6];
			row_count[0] = flitdb_to_short(row_count_read);
			row_count[1] = (row_count[0] + 1);
			if (row_count[0] < 0)
			{
				if (input_buffer != NULL)
					free(input_buffer);
				strncpy((*handler)->err_message, "Row count negation detected\0", FLITDB_MAX_ERR_SIZE);
				flitdb_clear_values(handler);
				return FLITDB_CORRUPT;
			}
			if (row_count[0] != 0)
			{
				segment_detection = true;
				insertion_state[0] = 1;
			}
		}
		else
		{
			row_count[0] -= 1;
			segment_detection = false;
			if (row_count[0] == 0)
				insertion_state[0] = 2;
			else
				segment_detection = true;
		}
		char position_read[4];
		position_read[3] = '\0';
		position_read[0] = (*handler)->buffer[(read_length[0] < 15) ? 0 : 7];
		position_read[1] = (*handler)->buffer[(read_length[0] < 15) ? 1 : 8];
		position_read[2] = (*handler)->buffer[(read_length[0] < 15) ? 2 : 9];
		signed short position = flitdb_to_short(position_read);
		if (position < 0)
		{
			if (input_buffer != NULL)
				free(input_buffer);
			strncpy((*handler)->err_message, "Position offset negation detected\0", FLITDB_MAX_ERR_SIZE);
			flitdb_clear_values(handler);
			return FLITDB_CORRUPT;
		}
		char response_length_read[5];
		response_length_read[4] = '\0';
		response_length_read[0] = (*handler)->buffer[(read_length[0] < 15) ? 3 : 10];
		response_length_read[1] = (*handler)->buffer[(read_length[0] < 15) ? 4 : 11];
		response_length_read[2] = (*handler)->buffer[(read_length[0] < 15) ? 5 : 12];
		response_length_read[3] = (*handler)->buffer[(read_length[0] < 15) ? 6 : 13];
		current_length = flitdb_to_short(response_length_read);
		if (current_length < 0)
		{
			if (input_buffer != NULL)
				free(input_buffer);
			strncpy((*handler)->err_message, "Length notation negation detected\0", FLITDB_MAX_ERR_SIZE);
			flitdb_clear_values(handler);
			return FLITDB_CORRUPT;
		}
		current_length += 1;
		if (skip_offset[0] == column_position)
		{
			if (position == row_position)
			{
				offset[1] = offset[3];
				insertion_state[0] = insertion_state[1];
				break;
			}
			else if (position > row_position)
			{
				current_length = 0;
				read_length[0] = 8;
				offset[0] = offset[1];
				offset[1] = offset[2];
				break;
			}
			else if (row_count[0] == 0)
			{
				if (read_length[0] == 15)
					offset[2] = offset[1];
				else
					offset[2] = offset[3];
				read_length[0] = 8;
				insertion_state[0] = 1;
				offset[0] += current_length;
				offset[1] = offset[3];
				current_length = 0;
				break;
			}
		}
		else if (skip_offset[0] > column_position)
		{
			if (skip_offset[1] == (column_position - 1))
			{
				offset[0] = offset[3];
				offset[1] = offset[3];
			}
			else
			{
				offset[0] = offset[3];
				offset[1] = offset[3];
			}
			current_length = 0;
			read_length[0] = read_length[1];
			break;
		}
		offset[0] += current_length;
		read_length[1] = read_length[0];
		if (read_length[0] == 15)
			offset[2] = offset[3];
		if (segment_detection)
			read_length[0] = 8;
		else
			read_length[0] = 15;
	}
	unsigned short update_next = 0;
	unsigned char update_override_cancel = 0;
	struct relinquish_excersion
	{
		unsigned short size;
		size_t position;
		bool use;
	};
	struct relinquish_excersion info_skip_offset;
	info_skip_offset.size = 0;
	info_skip_offset.position = 0;
	info_skip_offset.use = false;
	struct relinquish_excersion info_row_count;
	info_row_count.size = 0;
	info_row_count.position = 0;
	info_row_count.use = false;
	struct relinquish_excersion info_row_position;
	info_row_position.size = 0;
	info_row_position.position = 0;
	info_row_position.use = false;
	struct relinquish_excersion info_input_size;
	info_input_size.size = 0;
	info_input_size.position = 0;
	info_input_size.use = false;
	struct relinquish_excersion info_input_type;
	info_input_type.size = 0;
	info_input_type.position = 0;
	info_input_type.use = false;
	struct relinquish_excersion info_input_buffer;
	info_input_buffer.size = 0;
	info_input_buffer.position = 0;
	info_input_buffer.use = false;
	if (offset[0] == offset[1])
	{
		offset[1] -= 1;
		if (input_size == 0)
		{
			if (input_buffer != NULL)
				free(input_buffer);
			flitdb_clear_values(handler);
			return FLITDB_DONE;
		}
		// New grouping
		if ((*handler)->size > 0 && offset[0] != (*handler)->size)
			skip_offset[0] = skip_offset[1];
		unsigned long long int skip_offset_sizing = flitdb_abs((column_position - skip_offset[0] - 1));
		if (info_skip_offset.size >= (FLITDB_COLUMN_POSITION_MAX - 1))
		{
			strncpy((*handler)->err_message, "Unsafe measures exceeded acceptable writable boundaries\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_RANGE;
		}
		info_skip_offset.size = skip_offset_sizing;
		if ((*handler)->size > 0 && offset[0] != (*handler)->size)
		{
			unsigned short offset_sizing = input_size + 15;
			unsigned short buffer_size = FLITDB_MAX_BUFFER_SIZE;
			unsigned short buffer_offset = 0;
			bool writable = true;
			while (writable)
			{
				if ((buffer_offset + buffer_size) > (*handler)->size)
				{
					buffer_size = ((*handler)->size - buffer_offset - offset[0]);
					writable = false;
				}
				buffer_offset += buffer_size;
				unsigned long reference_position = ((*handler)->size - buffer_offset);
				strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
				fseek((*handler)->file_descriptor, reference_position, SEEK_SET);
				if (fread((*handler)->buffer, buffer_size, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				fseek((*handler)->file_descriptor, (reference_position + offset_sizing), SEEK_SET);
				if (fwrite((*handler)->buffer, buffer_size, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
			}
		}
		info_skip_offset.position = offset[0];
		info_skip_offset.use = true;
		info_row_count.size = 0;
		info_row_count.position = (offset[0] + 4);
		info_row_count.use = true;
		info_row_position.size = row_position;
		info_row_position.position = offset[0] + 7;
		info_row_position.use = true;
		info_input_size.size = (input_size - 1);
		info_input_size.position = (offset[0] + 10);
		info_input_size.use = true;
		info_input_type.size = (*handler)->value_type;
		info_input_type.position = (offset[0] + 14);
		info_input_type.use = true;
		info_input_buffer.size = input_size;
		info_input_buffer.position = (offset[0] + 15);
		info_input_buffer.use = true;
		unsigned long long incremental_size = input_size + 15;
		if (offset[0] < (*handler)->size)
			update_next = (offset[0] + incremental_size);
		if ((*handler)->size == 0)
			update_override_cancel = 2;
		(*handler)->size += incremental_size;
		current_length = 0;
		input_size = 0;
	}
	bool deleted_column[2] = {false, false};
	if (update_next == 0 && update_override_cancel == 0)
	{
		if (current_length == 0 && input_size == 0)
		{
			// Skip
		}
		else if (input_size < current_length)
		{
			// Insert less
			unsigned short offset_sizing = (current_length - input_size);
			offset[3] = offset[0];
			if (input_size == 0)
			{
				// Delete
				offset[0] += current_length;
				skip_offset[0] = (skip_offset[1] + 1);
				offset_sizing = (((insertion_state[0] == 2) ? 15 : 8) + current_length);
				if (row_count[1] > 1)
				{
					if (insertion_state[0] == 2)
						offset_sizing -= 7;
					row_count[1] -= 2;
					info_row_count.size = row_count[1];
					info_row_count.position = (offset[1] + 4);
					info_row_count.use = true;
					deleted_column[1] = true;
				}
				else
				{
					update_next = (offset[0] - current_length - 15);
					deleted_column[0] = true;
					update_override_cancel = 3;
				}
			}
			else
			{
				offset[1] = offset[0];
				offset[0] += current_length;
			}
			unsigned short buffer_size = FLITDB_MAX_BUFFER_SIZE;
			unsigned short buffer_offset = offset[0];
			bool writable = true;
			while (writable)
			{
				if ((buffer_offset + buffer_size) > (*handler)->size)
				{
					buffer_size = (((*handler)->size - buffer_offset));
					writable = false;
				}
				strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
				fseek((*handler)->file_descriptor, buffer_offset, SEEK_SET);
				if (fread((*handler)->buffer, buffer_size, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				fseek((*handler)->file_descriptor, (buffer_offset - offset_sizing), SEEK_SET);
				if (fwrite((*handler)->buffer, buffer_size, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				buffer_offset += buffer_size;
			}
			if (!deleted_column[0] && !deleted_column[1])
			{
				info_input_size.size = (input_size - 1);
				info_input_size.position = (offset[3] - 5);
				info_input_size.use = true;
				info_input_type.size = (*handler)->value_type;
				info_input_type.position = ((offset[3] - 1));
				info_input_type.use = true;
				info_input_buffer.size = input_size;
				info_input_buffer.position = offset[3];
				info_input_buffer.use = true;
			}
			(*handler)->size -= offset_sizing;
			if ((*handler)->size == offset[1])
				update_override_cancel = 4;
			if (flitdb_truncate(fileno((*handler)->file_descriptor), (*handler)->size) != 0)
			{
				if (input_buffer != NULL)
					free(input_buffer);
				flitdb_clear_values(handler);
				strncpy((*handler)->err_message, "Failed database truncation occurred\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_CORRUPT;
			}
		}
		else if (input_size > current_length)
		{
			// Insert more
			if (current_length == 0)
			{
				offset[0] -= 8;
				info_row_count.size = row_count[1];
				info_row_count.position = (offset[3] + 4);
				info_row_count.use = true;
			}
			unsigned short offset_sizing = ((current_length > 0) ? ((input_size - current_length)) : (input_size + 8));
			unsigned short buffer_size = FLITDB_MAX_BUFFER_SIZE;
			unsigned short buffer_offset = 0;
			bool writable = true;
			while (writable)
			{
				if ((buffer_offset + buffer_size) > (*handler)->size)
				{
					buffer_size = ((*handler)->size - buffer_offset - offset[0]);
					writable = false;
				}
				buffer_offset += buffer_size;
				unsigned long reference_position = ((*handler)->size - buffer_offset);
				strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
				fseek((*handler)->file_descriptor, reference_position, SEEK_SET);
				if (fread((*handler)->buffer, buffer_size, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				fseek((*handler)->file_descriptor, (reference_position + offset_sizing), SEEK_SET);
				if (fwrite((*handler)->buffer, buffer_size, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
			}
			if (current_length > 0)
			{
				(*handler)->size += (input_size - current_length);
				offset[0] -= 8;
			}
			else
				(*handler)->size += (input_size + 8);
			info_row_position.size = row_position;
			info_row_position.position = offset[0];
			info_row_position.use = true;
			info_input_size.size = (input_size - 1);
			info_input_size.position = (offset[0] + 3);
			info_input_size.use = true;
			info_input_type.size = (*handler)->value_type;
			info_input_type.position = (offset[0] + 7);
			info_input_type.use = true;
			info_input_buffer.size = input_size;
			info_input_buffer.position = (offset[0] + 8);
			info_input_buffer.use = true;
		}
		else
		{
			// Update value (no size change)
			info_input_type.size = (*handler)->value_type;
			info_input_type.position = (offset[0] - 1);
			info_input_type.use = true;
			info_input_buffer.size = input_size;
			info_input_buffer.position = offset[0];
			info_input_buffer.use = true;
		}
	}
	if (info_skip_offset.use)
	{
		char skip_offset_buffer[5];
		strncpy(skip_offset_buffer, "\0", 5);
		snprintf(skip_offset_buffer, sizeof(skip_offset_buffer), "%d", info_skip_offset.size);
		if (info_skip_offset.size < 10)
		{
			skip_offset_buffer[3] = skip_offset_buffer[0];
			skip_offset_buffer[2] = '0';
			skip_offset_buffer[1] = '0';
			skip_offset_buffer[0] = '0';
		}
		else if (info_skip_offset.size < 100)
		{
			skip_offset_buffer[3] = skip_offset_buffer[1];
			skip_offset_buffer[2] = skip_offset_buffer[0];
			skip_offset_buffer[1] = '0';
			skip_offset_buffer[0] = '0';
		}
		else if (info_skip_offset.size < 1000)
		{
			skip_offset_buffer[3] = skip_offset_buffer[2];
			skip_offset_buffer[2] = skip_offset_buffer[1];
			skip_offset_buffer[1] = skip_offset_buffer[0];
			skip_offset_buffer[0] = '0';
		}
		fseek((*handler)->file_descriptor, info_skip_offset.position, SEEK_SET);
		if (fwrite(skip_offset_buffer, 4, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
	}
	if (info_row_count.use)
	{
		char row_buffer[4];
		strncpy(row_buffer, "\0", 4);
		snprintf(row_buffer, sizeof(row_buffer), "%d", info_row_count.size);
		if (info_row_count.size < 10)
		{
			row_buffer[2] = row_buffer[0];
			row_buffer[1] = '0';
			row_buffer[0] = '0';
		}
		else if (info_row_count.size < 100)
		{
			row_buffer[2] = row_buffer[1];
			row_buffer[1] = row_buffer[0];
			row_buffer[0] = '0';
		}
		fseek((*handler)->file_descriptor, info_row_count.position, SEEK_SET);
		if (fwrite(row_buffer, 3, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
	}
	if (info_row_position.use)
	{
		char position_buffer[4];
		strncpy(position_buffer, "\0", 4);
		snprintf(position_buffer, sizeof(position_buffer), "%d", info_row_position.size);
		if (info_row_position.size < 10)
		{
			position_buffer[2] = position_buffer[0];
			position_buffer[1] = '0';
			position_buffer[0] = '0';
		}
		else if (info_row_position.size < 100)
		{
			position_buffer[2] = position_buffer[1];
			position_buffer[1] = position_buffer[0];
			position_buffer[0] = '0';
		}
		fseek((*handler)->file_descriptor, info_row_position.position, SEEK_SET);
		if (fwrite(position_buffer, 3, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
	}
	if (info_input_size.use)
	{
		char input_length_buffer[5];
		strncpy(input_length_buffer, "\0", 5);
		snprintf(input_length_buffer, sizeof(input_length_buffer), "%d", info_input_size.size);
		if (info_input_size.size < 10)
		{
			input_length_buffer[3] = input_length_buffer[0];
			input_length_buffer[2] = '0';
			input_length_buffer[1] = '0';
			input_length_buffer[0] = '0';
		}
		else if (info_input_size.size < 100)
		{
			input_length_buffer[3] = input_length_buffer[1];
			input_length_buffer[2] = input_length_buffer[0];
			input_length_buffer[1] = '0';
			input_length_buffer[0] = '0';
		}
		else if (info_input_size.size < 1000)
		{
			input_length_buffer[3] = input_length_buffer[2];
			input_length_buffer[2] = input_length_buffer[1];
			input_length_buffer[1] = input_length_buffer[0];
			input_length_buffer[0] = '0';
		}
		fseek((*handler)->file_descriptor, info_input_size.position, SEEK_SET);
		if (fwrite(input_length_buffer, 4, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
	}
	if (info_input_type.use)
	{
		fseek((*handler)->file_descriptor, info_input_type.position, SEEK_SET);
		switch (info_input_type.size)
		{
			case FLITDB_INTEGER:
				if (fwrite("1", 1, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				break;
			case FLITDB_DOUBLE:
				if (fwrite("2", 1, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				break;
			case FLITDB_FLOAT:
				if (fwrite("3", 1, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				break;
			case FLITDB_CHAR:
				if (fwrite("4", 1, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				break;
			case FLITDB_BOOL:
				if (fwrite("5", 1, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				break;
		}
	}
	if (info_input_buffer.use)
	{
		fseek((*handler)->file_descriptor, info_input_buffer.position, SEEK_SET);
		if (fwrite(input_buffer, input_size_default, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
	}
	if ((*handler)->value_type != FLITDB_NULL)
		if (input_buffer != NULL)
			free(input_buffer);
	if ((update_next > 0 && (update_next < (*handler)->size || update_override_cancel == 0) && update_override_cancel != 2 && update_next != (*handler)->size) || update_override_cancel == 3)
	{
		// Update skip amount for the next data group along after request.
		strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
		fseek((*handler)->file_descriptor, update_next, SEEK_SET);
		if (fread((*handler)->buffer, 5, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
		unsigned short skip_amount_reducer = flitdb_to_short((*handler)->buffer);
		if (skip_amount_reducer == 0 && !deleted_column[0])
		{
			strncpy((*handler)->err_message, "Skip offset negation attempted on declared nullity\0", FLITDB_MAX_ERR_SIZE);
			flitdb_clear_values(handler);
			return FLITDB_CORRUPT;
		}
		if (deleted_column[0])
			skip_amount_reducer += skip_offset[0];
		else
			skip_amount_reducer -= 1;
		char skip_offset_buffer[5];
		strncpy(skip_offset_buffer, "\0", 5);
		snprintf(skip_offset_buffer, sizeof(skip_offset_buffer), "%d", skip_amount_reducer);
		if (skip_amount_reducer < 10)
		{
			skip_offset_buffer[3] = skip_offset_buffer[0];
			skip_offset_buffer[2] = '0';
			skip_offset_buffer[1] = '0';
			skip_offset_buffer[0] = '0';
		}
		else if (skip_amount_reducer < 100)
		{
			skip_offset_buffer[3] = skip_offset_buffer[1];
			skip_offset_buffer[2] = skip_offset_buffer[0];
			skip_offset_buffer[1] = '0';
			skip_offset_buffer[0] = '0';
		}
		else if (skip_amount_reducer < 1000)
		{
			skip_offset_buffer[3] = skip_offset_buffer[2];
			skip_offset_buffer[2] = skip_offset_buffer[1];
			skip_offset_buffer[1] = skip_offset_buffer[0];
			skip_offset_buffer[0] = '0';
		}
		fseek((*handler)->file_descriptor, update_next, SEEK_SET);
		if (fwrite(skip_offset_buffer, 4, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
	}
	flitdb_clear_values(handler);
	return FLITDB_DONE;
}

signed short flitdb_to_short(char *chars)
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

signed long long flitdb_to_long_long(char *chars)
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

double flitdb_to_double(char *chars)
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

float flitdb_to_float(char *chars)
{
	return (float)flitdb_to_double(chars);
}

long long unsigned int flitdb_abs(long long signed int value)
{
	return ((value > 0) ? value : -value);
}

#undef flitdb_truncate
#undef FLITDB_MAX_BUFFER_SIZE
#undef FLITDB_MAX_CHAR_LENGTH
#undef FLITDB_MAX_ERR_SIZE
#undef FLITDB_COLUMN_POSITION_MAX
#undef FLITDB_ROW_POSITION_MAX
#undef __WINDOWS__

#endif