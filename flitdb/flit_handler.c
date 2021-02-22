#ifndef flit_handler_c
#define flit_handler_c
#include <sys/stat.h>
#include <unistd.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define __WINDOWS__
#warning "File locking is currently not supported in Windows builds"
#define truncate _chsize
#include <io.h>
#else
#define truncate ftruncate
#include <sys/file.h>
#endif
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "flit.h"
#include "flit_misc.h"

#define flitdb_max_buffer_size 1024
#define flitdb_max_char_length 10000
#define flitdb_max_err_size 100

const unsigned int flitdb_column_position_max = 10000;
const unsigned int flitdb_row_position_max = 1000;

typedef struct flitdb
{
	char buffer[flitdb_max_buffer_size];
	bool configured;
	FILE *file_descriptor;
	char err_message[flitdb_max_err_size];
	struct value
	{
		signed long long int int_value;
		long double double_value;
		float float_value;
		char char_value[flitdb_max_char_length];
		bool bool_value;
	} value;
	unsigned char value_type;
	bool value_retrieved;
	size_t size;
	bool read_only;
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
	(*handler)->configured = (flitdb_max_buffer_size < 50 || flitdb_max_buffer_size > 1024);
	(*handler)->size = 0;
	(*handler)->read_only = false;
	(*handler)->value_type = 0;
	(*handler)->value_retrieved = false;
	(*handler)->file_descriptor = NULL;
	if (flitdb_max_buffer_size < 50)
		strncpy((*handler)->err_message, "The minimum buffer size has encroached beyond suitable definitions\0", flitdb_max_err_size);
	else if (flitdb_max_buffer_size > 1024)
		strncpy((*handler)->err_message, "The maximum buffer size has encroached beyond suitable definitions\0", flitdb_max_err_size);
	else
		strncpy((*handler)->err_message, "\0", flitdb_max_err_size);
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
	unsigned long long insertion_area[2] = {(flitdb_column_position_max * flitdb_row_position_max), (flitdb_row_position_max > 1 ? ((flitdb_column_position_max * (flitdb_row_position_max - 1)) * 8) : 0)};
	return ((insertion_area[0] * 10000) + insertion_area[1] + (flitdb_column_position_max * 15));
}

int flitdb_connection_setup(flitdb **handler, const char *filename, int flags)
{
	if ((*handler)->configured)
	{
		if (flitdb_max_buffer_size >= 50 && flitdb_max_buffer_size <= 1024)
			strncpy((*handler)->err_message, "The database handler has already been attributed to handle another database\0", flitdb_max_err_size);
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
		if ((flags & FLITDB_CREATE) != FLITDB_CREATE)
			return FLITDB_NOT_FOUND;
		if ((*handler)->read_only)
		{
			strncpy((*handler)->err_message, "No database exists to be exclusively read\0", flitdb_max_err_size);
			return FLITDB_ERROR;
		}
	}
	(*handler)->file_descriptor = fopen(filename, (((flags & FLITDB_READONLY) == FLITDB_READONLY) ? "r" : ((((flags & FLITDB_CREATE) == FLITDB_CREATE) && !file_exists) ? "w+" : "r+")));
	if ((*handler)->file_descriptor == NULL)
	{
		strncpy((*handler)->err_message, "Failed to open the database\0", flitdb_max_err_size);
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
		strncpy((*handler)->err_message, "Exclusive rights to access the database could not be obtained\0", flitdb_max_err_size);
		return FLITDB_BUSY;
	}
	#endif
	if ((*handler)->size > flitdb_max_size())
	{
		strncpy((*handler)->err_message, "The database attempted to access has a larger size than what this object can read\0", flitdb_max_err_size);
		return FLITDB_RANGE;
	}
	(*handler)->configured = true;
	return FLITDB_SUCCESS;
}

char *flitdb_get_err_message(flitdb **handler)
{
	return (*handler)->err_message;
}

int flitdb_insert_value_int(flitdb **handler, signed long long int set_value)
{
	if (!(*handler)->configured)
	{
		strncpy((*handler)->err_message, (char *)"The database handler has not been attributed to handle a database\0", flitdb_max_err_size);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", flitdb_max_err_size);
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to unexpected tennant\0", flitdb_max_err_size);
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
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", flitdb_max_err_size);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", flitdb_max_err_size);
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to unexpected tennant\0", flitdb_max_err_size);
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
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", flitdb_max_err_size);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", flitdb_max_err_size);
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to unexpected tennant\0", flitdb_max_err_size);
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
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", flitdb_max_err_size);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", flitdb_max_err_size);
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to unexpected tennant\0", flitdb_max_err_size);
		return FLITDB_ERROR;
	}
	if (strlen(set_value) >= flitdb_max_char_length)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to the length of a string being too large\0", flitdb_max_err_size);
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
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", flitdb_max_err_size);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", flitdb_max_err_size);
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		strncpy((*handler)->err_message, "Data insertion avoided due to unexpected tennant\0", flitdb_max_err_size);
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
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", flitdb_max_err_size);
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);
	if (column_position == 0 || column_position > flitdb_column_position_max || row_position == 0 || row_position > flitdb_row_position_max)
	{
		strncpy((*handler)->err_message, "The requested range was outside of the database's range\0", flitdb_max_err_size);
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
				strncpy((*handler)->err_message, "The database contracted a malformed structure declaration\0", flitdb_max_err_size);
				return FLITDB_CORRUPT;
			}
			else
				break;
		}
		strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
		fseek((*handler)->file_descriptor, offset, SEEK_SET);
		offset += read_length;
		if (fgets((*handler)->buffer, (read_length + 1), (*handler)->file_descriptor) == NULL)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", flitdb_max_err_size);
			return FLITDB_ERROR;
		}
		if (strlen((*handler)->buffer) != read_length)
		{
			strncpy((*handler)->err_message, "The database contracted a malformed structure declaration\0", flitdb_max_err_size);
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
			if (to_short(skip_amount_read) < 0)
			{
				strncpy((*handler)->err_message, "Skip offset negation detected\0", flitdb_max_err_size);
				return FLITDB_ERROR;
			}
			skip_offset += (to_short(skip_amount_read) + 1);
			if (skip_offset > column_position)
				return FLITDB_NULL;
			char row_count_read[4];
			row_count_read[3] = '\0';
			row_count_read[0] = (*handler)->buffer[4];
			row_count_read[1] = (*handler)->buffer[5];
			row_count_read[2] = (*handler)->buffer[6];
			row_count = to_short(row_count_read) + 1;
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
			unsigned short position = to_short(position_read);
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
		unsigned short response_length = (to_short(response_length_read) + 1);
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
			strncpy((*handler)->err_message, "The database yielded an invalid datatype\0", flitdb_max_err_size);
			return FLITDB_CORRUPT;
		}
		}
		if (response_length_read == 0)
		{
			strncpy((*handler)->err_message, "A reference to an imposed data declaration holds no length\0", flitdb_max_err_size);
			return FLITDB_CORRUPT;
		}
		else if (data_type == FLITDB_BOOL && response_length != 1)
		{
			strncpy((*handler)->err_message, "The database holds a boolean of a possible elongated length\0", flitdb_max_err_size);
			return FLITDB_CORRUPT;
		}
		if (store_response)
		{
			response_length++;
			(*handler)->value_type = data_type;
			(*handler)->value_retrieved = true;
			char response_value[response_length];
			char response_value_tmp[response_length];
			strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
			strncpy(response_value, "\0", sizeof(response_value));
			strncpy(response_value_tmp, "\0", sizeof(response_value_tmp));
			fseek((*handler)->file_descriptor, offset, SEEK_SET);
			if (fgets(response_value_tmp, response_length, (*handler)->file_descriptor) == NULL)
			{
				strncpy((*handler)->err_message, "Unable to allocate memory to retrieve value from the database\0", flitdb_max_err_size);
				return FLITDB_ERROR;
			}
			strncpy(response_value, response_value_tmp, sizeof(response_value));
			if (strlen(response_value) == 0)
			{
				strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", flitdb_max_err_size);
				return FLITDB_ERROR;
			}
			switch (data_type)
			{
			case FLITDB_INTEGER:
				(*handler)->value.int_value = to_long_long(response_value);
				break;
			case FLITDB_DOUBLE:
				(*handler)->value.double_value = to_double(response_value);
				break;
			case FLITDB_FLOAT:
				(*handler)->value.float_value = to_float(response_value);
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

int flitdb_insert_at(flitdb **handler, unsigned short column_position, unsigned short row_position)
{
	if (!(*handler)->configured)
	{
		strncpy((*handler)->err_message, "The database handler has not been attributed to handle a database\0", flitdb_max_err_size);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		strncpy((*handler)->err_message, "The database was opened in readonly mode\0", flitdb_max_err_size);
		flitdb_clear_values(handler);
		return FLITDB_READONLY;
	}
	if (column_position == 0 || column_position > flitdb_column_position_max || row_position == 0 || row_position > flitdb_row_position_max)
	{
		strncpy((*handler)->err_message, "The requested range was outside of the database's range\0", flitdb_max_err_size);
		flitdb_clear_values(handler);
		return FLITDB_RANGE;
	}
	row_position -= 1;
	char *input_buffer;
	unsigned short input_size = 0;
	switch ((*handler)->value_type)
	{
	case FLITDB_INTEGER:
	{
		input_buffer = (char *)calloc(64,sizeof(char));
		int ret = snprintf(input_buffer, 64, "%lld", (*handler)->value.int_value);
		if (ret <= 0)
		{
			free(input_buffer);
			strncpy((*handler)->err_message, "Value encoding of type: int; has failed\0", flitdb_max_err_size);
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
			free(input_buffer);
			strncpy((*handler)->err_message, "Value encoding of type: double; has failed\0", flitdb_max_err_size);
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
			free(input_buffer);
			strncpy((*handler)->err_message, "Value encoding of type: float; has failed\0", flitdb_max_err_size);
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
			free(input_buffer);
			strncpy((*handler)->err_message, "Length determination failed\0", flitdb_max_err_size);
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
		if (fgets((*handler)->buffer, (read_length[0] + 1), (*handler)->file_descriptor) == NULL)
		{
			if (offset[0] < (*handler)->size)
			{
				strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", flitdb_max_err_size);
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
			strncpy((*handler)->err_message, "The database contracted a malformed structure declaration\0", flitdb_max_err_size);
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
			signed short skip_amount = to_short(skip_amount_read);
			if (skip_amount < 0)
			{
				free(input_buffer);
				strncpy((*handler)->err_message, "Skip offset negation detected\0", flitdb_max_err_size);
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
			row_count[0] = to_short(row_count_read);
			row_count[1] = (row_count[0] + 1);
			if (row_count[0] < 0)
			{
				free(input_buffer);
				strncpy((*handler)->err_message, "Row count negation detected\0", flitdb_max_err_size);
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
		signed short position = to_short(position_read);
		if (position < 0)
		{
			free(input_buffer);
			strncpy((*handler)->err_message, "Position offset negation detected\0", flitdb_max_err_size);
			flitdb_clear_values(handler);
			return FLITDB_CORRUPT;
		}
		char response_length_read[5];
		response_length_read[4] = '\0';
		response_length_read[0] = (*handler)->buffer[(read_length[0] < 15) ? 3 : 10];
		response_length_read[1] = (*handler)->buffer[(read_length[0] < 15) ? 4 : 11];
		response_length_read[2] = (*handler)->buffer[(read_length[0] < 15) ? 5 : 12];
		response_length_read[3] = (*handler)->buffer[(read_length[0] < 15) ? 6 : 13];
		current_length = to_short(response_length_read);
		if (current_length <= 0)
		{
			free(input_buffer);
			strncpy((*handler)->err_message, "Length notation negation detected\0", flitdb_max_err_size);
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
			free(input_buffer);
			flitdb_clear_values(handler);
			return FLITDB_DONE;
		}
		// New grouping
		if ((*handler)->size > 0 && offset[0] != (*handler)->size)
		{
			skip_offset[0] = skip_offset[1];
			unsigned short offset_sizing = input_size + 15;
			unsigned short buffer_size = flitdb_max_buffer_size;
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
				if (fgets((*handler)->buffer, (buffer_size + 1), (*handler)->file_descriptor) == NULL)
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", flitdb_max_err_size);
					return FLITDB_ERROR;
				}
				fseek((*handler)->file_descriptor, (reference_position + offset_sizing), SEEK_SET);
				if (fputs((*handler)->buffer, (*handler)->file_descriptor) == -1)
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data from the database\0", flitdb_max_err_size);
					return FLITDB_ERROR;
				}
			}
		}
		info_skip_offset.size = (column_position - skip_offset[0] - 1);
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
			unsigned short buffer_size = flitdb_max_buffer_size;
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
				if (fgets((*handler)->buffer, (buffer_size + 1), (*handler)->file_descriptor) == NULL)
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", flitdb_max_err_size);
					return FLITDB_ERROR;
				}
				fseek((*handler)->file_descriptor, (buffer_offset - offset_sizing), SEEK_SET);
				if (fputs((*handler)->buffer, (*handler)->file_descriptor) == -1)
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data from the database\0", flitdb_max_err_size);
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
			if (truncate(fileno((*handler)->file_descriptor), (*handler)->size) != 0)
			{
				free(input_buffer);
				flitdb_clear_values(handler);
				strncpy((*handler)->err_message, "Failed database truncation occurred\0", flitdb_max_err_size);
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
			unsigned short buffer_size = flitdb_max_buffer_size;
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
				if (fgets((*handler)->buffer, (buffer_size + 1), (*handler)->file_descriptor) == NULL)
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", flitdb_max_err_size);
					return FLITDB_ERROR;
				}
				fseek((*handler)->file_descriptor, (reference_position + offset_sizing), SEEK_SET);
				if (fputs((*handler)->buffer, (*handler)->file_descriptor) == -1)
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data from the database\0", flitdb_max_err_size);
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
			info_input_type.position = (offset[0] + 8);
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
		if (fputs(skip_offset_buffer, (*handler)->file_descriptor) == -1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data from the database\0", flitdb_max_err_size);
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
		if (fputs(row_buffer, (*handler)->file_descriptor) == -1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data from the database\0", flitdb_max_err_size);
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
		if (fputs(position_buffer, (*handler)->file_descriptor) == -1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data from the database\0", flitdb_max_err_size);
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
		if (fputs(input_length_buffer, (*handler)->file_descriptor) == -1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data from the database\0", flitdb_max_err_size);
			return FLITDB_ERROR;
		}
	}
	if (info_input_type.use)
	{
		fseek((*handler)->file_descriptor, info_input_type.position, SEEK_SET);
		switch (info_input_type.size)
		{
			case FLITDB_INTEGER:
				fputs("1", (*handler)->file_descriptor);
				break;
			case FLITDB_DOUBLE:
				fputs("2", (*handler)->file_descriptor);
				break;
			case FLITDB_FLOAT:
				fputs("3", (*handler)->file_descriptor);
				break;
			case FLITDB_CHAR:
				fputs("4", (*handler)->file_descriptor);
				break;
			case FLITDB_BOOL:
				fputs("5", (*handler)->file_descriptor);
				break;
		}
	}
	if (info_input_buffer.use)
	{
		fseek((*handler)->file_descriptor, info_input_buffer.position, SEEK_SET);
		if (fputs(input_buffer, (*handler)->file_descriptor) == -1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data from the database\0", flitdb_max_err_size);
			return FLITDB_ERROR;
		}
	}
	if ((*handler)->value_type != FLITDB_NULL)
		free(input_buffer);
	if ((update_next > 0 && (update_next < (*handler)->size || update_override_cancel == 0) && update_override_cancel != 2 && update_next != (*handler)->size) || update_override_cancel == 3)
	{
		// Update skip amount for the next data group along after request.
		strncpy((*handler)->buffer, "\0", sizeof((*handler)->buffer));
		fseek((*handler)->file_descriptor, update_next, SEEK_SET);
		if (fgets((*handler)->buffer, 5, (*handler)->file_descriptor) == NULL)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", flitdb_max_err_size);
			return FLITDB_ERROR;
		}
		unsigned short skip_amount_reducer = to_short((*handler)->buffer);
		if (skip_amount_reducer == 0 && !deleted_column[0])
		{
			strncpy((*handler)->err_message, "Skip offset negation attempted on declared nullity\0", flitdb_max_err_size);
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
		if (fputs(skip_offset_buffer, (*handler)->file_descriptor) == -1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data from the database\0", flitdb_max_err_size);
			return FLITDB_ERROR;
		}
	}
	flitdb_clear_values(handler);
	return FLITDB_DONE;
}

#endif