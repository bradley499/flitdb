#ifndef flit_c
#define flit_c

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/file.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "flit.h"

const unsigned long long flitdb_max_size();

// FlitDB database operations
int flitdb_new(flitdb **handler);
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

unsigned int flitdb_version_check()
{
	return FLITDB_VERSION;
}

int flitdb_setup(const char *filename, flitdb **handler, int flags)
{
	if (flitdb_new(handler) == FLITDB_ERROR)
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
	int insert_success = flitdb_insert_value_int(handler, value);
	if (insert_success != FLITDB_DONE)
		return insert_success;
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
	int insert_success = flitdb_insert_value_double(handler, value);
	if (insert_success != FLITDB_DONE)
		return insert_success;
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
	int insert_success = flitdb_insert_value_float(handler, value);
	if (insert_success != FLITDB_DONE)
		return insert_success;
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
	int insert_success = flitdb_insert_value_char(handler, value);
	if (insert_success != FLITDB_DONE)
		return insert_success;
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
	int insert_success = flitdb_insert_value_bool(handler, (value == true));
	if (insert_success != FLITDB_DONE)
		return insert_success;
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
#define FLITDB_SEGMENT_SIZE 5
#define FLITDB_PARTITION_SIZE 4
#define FLITDB_PARTITION_AND_SEGMENT (FLITDB_SEGMENT_SIZE + FLITDB_PARTITION_SIZE)

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
	// Resets all the data associated with the current insert/retrieval
	(*handler)->value.int_value = 0;
	(*handler)->value.double_value = 0;
	(*handler)->value.float_value = 0;
	(*handler)->value.bool_value = false;
	strncpy((*handler)->value.char_value, "\0", sizeof((*handler)->value.char_value));
	(*handler)->value_type = FLITDB_NULL;
	(*handler)->value_retrieved = false;
	memset((*handler)->buffer, 0, sizeof((*handler)->buffer));
}

int flitdb_new(flitdb **handler)
{
	*handler = &*(flitdb *)malloc(sizeof(flitdb)); // Attempts to request for memory
	if (*handler == NULL) // No memory was allocated
		return FLITDB_ERROR;
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
	return FLITDB_SUCCESS;
}

void flitdb_destroy(flitdb **handler)
{
	if ((*handler)->configured)
	{
		(*handler)->configured = false;
		flock(fileno((*handler)->file_descriptor), LOCK_UN); // Removes lock on the database file of operation
		fclose((*handler)->file_descriptor); // Closes conenction to the database file of operation
	}
}

inline const unsigned long long flitdb_max_size()
{
	// To calculate the maximum file size of what the database file can safely be read and written to
	unsigned long long insertion_area[2] = {(FLITDB_COLUMN_POSITION_MAX * FLITDB_ROW_POSITION_MAX), (FLITDB_ROW_POSITION_MAX > 1 ? ((FLITDB_COLUMN_POSITION_MAX * (FLITDB_ROW_POSITION_MAX - 1)) * FLITDB_SEGMENT_SIZE) : 0)};
	return ((insertion_area[0] * 9999) + insertion_area[1] + (FLITDB_COLUMN_POSITION_MAX * FLITDB_PARTITION_AND_SEGMENT));
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
	(*handler)->file_descriptor = fopen(filename, (((flags & FLITDB_READONLY) == FLITDB_READONLY) ? "r" : ((((flags & FLITDB_CREATE) == FLITDB_CREATE) && !file_exists) ? "w+" : "r+"))); // Opens a connection to a database file
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
	(*handler)->configured = true; // Successfully configured correctly
	if (flock(fileno((*handler)->file_descriptor), LOCK_EX | LOCK_NB) != 0) // Attempts to lock the current database file
	{
		strncpy((*handler)->err_message, "Exclusive rights to access the database could not be obtained\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_BUSY;
	}
	if ((flags & FLITDB_UNSAFE) == FLITDB_UNSAFE) // Checks if unsafe operations have been attributed to this handler
		(*handler)->unsafe = true; // Unsafe mode is enabled
	else if ((*handler)->size > flitdb_max_size()) // Checks if the database file is bigger than the maximum size for safe operations
	{
		strncpy((*handler)->err_message, "The database attempted to access has a larger size than what this object can read\0", FLITDB_MAX_ERR_SIZE);
		return FLITDB_RANGE;
	}
	return FLITDB_SUCCESS;
}

char *flitdb_get_err_message(flitdb **handler)
{
	if (*handler == NULL)
		return (char *)"This handler failed to be setup\0"; // Default error message if handler has not been setup
	return (*handler)->err_message; // Returns an error message (if set)
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
	flitdb_clear_values(handler); // Clears the current insert values
	(*handler)->value_type = FLITDB_INTEGER; // Sets the current insert value type to integer
	(*handler)->value.int_value = set_value; // Sets the current insert value
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
	flitdb_clear_values(handler); // Clears the current insert values
	(*handler)->value_type = FLITDB_DOUBLE; // Sets the current insert value type to double
	(*handler)->value.double_value = set_value; // Sets the current insert value
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
	flitdb_clear_values(handler); // Clears the current insert values
	(*handler)->value_type = FLITDB_FLOAT; // Sets the current insert value type to float
	(*handler)->value.float_value = set_value; // Sets the current insert value
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
	flitdb_clear_values(handler); // Clears the current insert values
	(*handler)->value_type = FLITDB_CHAR; // Sets the current insert value type to char array
	strncpy((*handler)->value.char_value, set_value, sizeof((*handler)->value.char_value)); // Sets the current insert value
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
	flitdb_clear_values(handler); // Clears the current insert values
	(*handler)->value_type = FLITDB_BOOL; // Sets the current insert value type to boolean
	(*handler)->value.bool_value = set_value; // Sets the current insert value
	return FLITDB_DONE;
}

int flitdb_insert_reset(flitdb **handler)
{
	flitdb_clear_values(handler); // Clears the current insert values
	return FLITDB_DONE;
}

signed long long flitdb_retrieve_value_int(flitdb **handler)
{
	return (*handler)->value.int_value; // Returns the value stored in the int register
}

long double flitdb_retrieve_value_double(flitdb **handler)
{
	return (*handler)->value.double_value; // Returns the value stored in the double register
}

float flitdb_retrieve_value_float(flitdb **handler)
{
	return (*handler)->value.float_value; // Returns the value stored in the float register
}

char *flitdb_retrieve_value_char(flitdb **handler)
{
	return (*handler)->value.char_value; // Returns the value stored in the char register
}

bool flitdb_retrieve_value_bool(flitdb **handler)
{
	return (*handler)->value.bool_value; // Returns the value stored in the bool register
}

int flitdb_retrieve_value_type(flitdb **handler)
{
	return (*handler)->value_type; // Returns the value type
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
	unsigned char read_length = FLITDB_PARTITION_AND_SEGMENT;
	unsigned short row_count = 0;
	unsigned short row_position_count = 0;
	for (;;)
	{
		if ((offset + read_length) >= (*handler)->size)
		{
			if ((offset + read_length) > ((*handler)->size + read_length))
			{
				strncpy((*handler)->err_message, "The database contracted a malformed structure declaration\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_CORRUPT;
			}
			break;
		}
		fseek((*handler)->file_descriptor, offset, SEEK_SET);
		offset += read_length;
		if (read_length == FLITDB_PARTITION_AND_SEGMENT)
		{
			signed short skip_amount;
			if (fread(&skip_amount, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
			{
				strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			if (skip_amount < 0)
			{
				strncpy((*handler)->err_message, "Skip offset negation detected\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			skip_offset += (skip_amount + 1);
			if (skip_offset > column_position)
				return FLITDB_NULL;
			if (fread(&row_count, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
			{
				strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			row_count += 1;
			row_position_count = 0;
		}
		unsigned char set_read_length = FLITDB_PARTITION_AND_SEGMENT;
		if (skip_offset == column_position)
		{
			unsigned short position;
			if (fread(&position, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
			{
				strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			if (position == row_position)
			{
				store_response = true;
				row_count = 0;
			}
		}
		else
			fseek((*handler)->file_descriptor, sizeof(short), SEEK_CUR);
		row_position_count += 1;
		if (row_count > 1)
		{
			row_count -= 1;
			set_read_length = FLITDB_SEGMENT_SIZE;
		}
		unsigned short response_length;
		if (fread(&response_length, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
		if (response_length == 0)
		{
			strncpy((*handler)->err_message, "A reference to an imposed data declaration holds no length\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_CORRUPT;
		}
		response_length += 1;
		unsigned char data_type;
		if (fread(&data_type, 1, 1, (*handler)->file_descriptor) != 1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
		switch (data_type)
		{
		case 1:
			data_type = FLITDB_INTEGER;
			break;
		case 2:
			data_type = FLITDB_DOUBLE;
			break;
		case 3:
			data_type = FLITDB_FLOAT;
			break;
		case 4:
			data_type = FLITDB_CHAR;
			break;
		case 5:
			data_type = FLITDB_BOOL;
			break;
		default:
			strncpy((*handler)->err_message, "The database yielded an invalid datatype\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_CORRUPT;
		}
		if (data_type == FLITDB_BOOL && response_length != 1)
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
			memset((*handler)->buffer, 0, sizeof((*handler)->buffer));
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
	if (input_size == 0)
		if (input_buffer != NULL)
			free(input_buffer);
	const unsigned short input_size_default = input_size;
	size_t offset[6] = {0, 0, 0, 0, 0, 0};
	unsigned short skip_offset[2] = {0, 0};
	signed short skip_amount[2] = {0, 0};
	unsigned short read_length[2] = {FLITDB_PARTITION_AND_SEGMENT, FLITDB_PARTITION_AND_SEGMENT};
	signed short row_count[3] = {0, 0, 0};
	unsigned short current_length = 0;

	// READ STRUCTURE
	if ((*handler)->size > 0)
	{
		for (;;)
		{
			read_length[1] = read_length[0]; // Store previous size
			offset[2] = offset[0]; // Store segment position
			if (offset[0] >= (*handler)->size)
			{
				if ((offset[0] - current_length) > (*handler)->size)
				{
					strncpy((*handler)->err_message, "The database contracted a malformed structure declaration\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_CORRUPT;
				}
				else if (skip_offset[0] == column_position) // Add segment to end
				{
					offset[0] = offset[3];
					offset[1] = (*handler)->size;
					row_count[0] = row_count[1]; // Set row count to row default
					current_length = 0;
				}
				else if (skip_offset[0] < column_position) // Insert partition to end
				{
					offset[0] = (*handler)->size;
					offset[1] = offset[0];
					row_count[0] = 0; // Set row count to 0
					current_length = 0;
				}
				break;
			}
			fseek((*handler)->file_descriptor, offset[0], SEEK_SET);
			offset[0] += read_length[0];
			unsigned char skip_read_offset = 1;
			if (row_count[0] == 0) // Previous partition reached end
			{
				offset[1] = offset[2];
				offset[5] = offset[3];
				offset[3] = offset[2];
				offset[4] = (offset[2] + FLITDB_PARTITION_SIZE);
				skip_amount[1] = skip_amount[0]; // Set previous skip amount
				if (fread(&skip_amount[0], 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				if (skip_amount[0] < 0)
				{
					strncpy((*handler)->err_message, "Skip offset negation detected\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				skip_offset[1] = skip_offset[0];
				skip_offset[0] += (skip_amount[0] + 1);
				if (fread(&row_count[0], 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				row_count[2] = row_count[1];
				row_count[0] += 1;
				row_count[1] = row_count[0];
				if (row_count[0] > 1)
					read_length[0] = FLITDB_SEGMENT_SIZE;
			}
			else
			{
				if (skip_offset[0] != column_position)
					fseek((*handler)->file_descriptor, skip_read_offset++, SEEK_CUR);
				offset[1] += read_length[0];
			}
			if (skip_offset[0] == column_position) // Reached target column
			{
				skip_offset[1] = skip_offset[0];
				unsigned short position;
				if (fread(&position, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				if (position == row_position) // Is current row
				{
					if (fread(&current_length, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
					{
						strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
						return FLITDB_ERROR;
					}
					current_length += 1;
					offset[0] = offset[3]; // Beginning of partition
					offset[1] = offset[2]; // Beginning of segment
					row_count[0] = row_count[1]; // Set row count to total rows
					break;
				}
				else if (row_count[0] >= 1 && position > row_position) // Passed target row (doesn't exist) but additional rows exist
				{
					offset[0] = offset[3]; // Beginning of segment
					offset[1] = offset[2]; // Beginning of partition
					current_length = 0; // Does not exist so set length of it to 0
					row_count[0] = row_count[1]; // Set row count to total rows
					break;
				}
				else if (row_position < position) // Passed target row (doesn't exist)
				{
					offset[0] = offset[2]; // Beginning of segment
					current_length = 0; // Does not exist so set length of it to 0
					row_count[0] = row_count[1]; // Set row count to total rows
					break;
				}
			}
			else if (skip_offset[0] > column_position) // Passed target column (doesn't exist)
			{
				skip_offset[0] = skip_offset[1];
				skip_amount[0] = skip_amount[1];
				offset[1] = offset[2]; // Beginning of segment
				if (skip_offset[0] == column_position)
					offset[0] = offset[5]; // Beginning of previous partition
				else
					offset[0] = offset[2]; // Beginning of new partition
				read_length[0] = read_length[1]; // Set length to previous
				current_length = 0; // Does not exist so set length of it to 0
				if (skip_offset[1] == column_position) // If previous was the correct column
					row_count[0] = row_count[2]; // Use previous row count
				else
					row_count[0] = 0; // Does not exist so row count is 0
				break;
			}
			else
				fseek((*handler)->file_descriptor, (3 - skip_read_offset), SEEK_CUR);
			if (fread(&current_length, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
			{
				strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			current_length += 1;
			if (row_count[0] > 1)
				row_count[0] -= 1;
			else
			{
				row_count[0] = 0;
				read_length[0] = FLITDB_PARTITION_AND_SEGMENT;
			}
			offset[0] += current_length;
		}
	}
	if (current_length == 0 && input_size == 0)
		return FLITDB_DONE; // No need to write any data
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
	info_row_count.size = row_count[0];
	if (input_size == 0 || current_length > 0)
		info_row_count.size--;
	info_row_count.position = 0;
	info_row_count.use = false;
	struct relinquish_excersion info_row_position;
	info_row_position.size = row_position;
	info_row_position.position = 0;
	info_row_position.use = false;
	struct relinquish_excersion info_input_size;
	info_input_size.size = (input_size - 1);
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
	info_input_type.size = (*handler)->value_type;
	bool removal = false;
	if (input_size > current_length)
	{
		unsigned short offset_sizing = (input_size - current_length);
		if (current_length == 0)
		{
			if (row_count[0] == 0)
				offset_sizing += FLITDB_PARTITION_AND_SEGMENT;
			else
				offset_sizing += FLITDB_SEGMENT_SIZE;
		}
		else
			row_count[0] -= 1;
		if (offset[1] < (*handler)->size)
		{
			// Move segments after offset position
			size_t buffer_delimitation_point = (offset[1]);
			size_t buffer_offset = (((*handler)->size - offset[1]) % FLITDB_MAX_BUFFER_SIZE);
			if (buffer_offset == 0)
				buffer_offset = FLITDB_MAX_BUFFER_SIZE;
			unsigned short buffer_size = buffer_offset;
			for (;;)
			{
				memset((*handler)->buffer, 0, sizeof((*handler)->buffer));
				fseek((*handler)->file_descriptor, ((*handler)->size - buffer_offset), SEEK_SET);
				if (fread((*handler)->buffer, buffer_size, 1, (*handler)->file_descriptor) != 1)
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				fseek((*handler)->file_descriptor, (((*handler)->size - buffer_offset) + offset_sizing), SEEK_SET);
				if (fwrite((*handler)->buffer, buffer_size, 1, (*handler)->file_descriptor) != 1)
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				if (((*handler)->size - buffer_offset) <= buffer_delimitation_point)
					break;
				buffer_size = FLITDB_MAX_BUFFER_SIZE;
				buffer_offset += buffer_size;
			}
			fseek((*handler)->file_descriptor, (((*handler)->size - buffer_offset) + offset_sizing), SEEK_SET);
		}
		(*handler)->size += offset_sizing;
	}
	else if (input_size < current_length)
	{
		unsigned int offset_sizing = (current_length - (current_length - input_size));
		if (row_count[0] == 1)
		{
			offset[3] = offset[1]; // Store beginging of segment
			offset[1] = offset[4]; // Set to beginging of segment
		}
		size_t deletion_point[2] = {
			(offset[1] + FLITDB_SEGMENT_SIZE + current_length), // End point of current
			(offset[1] + FLITDB_SEGMENT_SIZE + offset_sizing), // New end point position
		};
		if (offset[0] == offset[1])
			deletion_point[0] += FLITDB_PARTITION_SIZE;
		if (input_size == 0) // Removing data
		{
			if (row_count[0] > 1) // More rows exist
			{
				if (offset[0] == offset[1])
				{
					deletion_point[1] -= (((deletion_point[1] - offset[0]))); // Delete partition
					deletion_point[1] += FLITDB_PARTITION_SIZE;
				}
				else
					deletion_point[1] -= (FLITDB_PARTITION_SIZE + 1);
			}
			else if (offset[3] == offset[0]) // Beginging of partition
			{
				if (offset[0] == offset[1]) // Segment declaration is at beginning of partition
				{
					// Account for partition decalartion
					deletion_point[0] += FLITDB_PARTITION_SIZE;
					deletion_point[1] += FLITDB_PARTITION_SIZE;
				}
				else if (row_count[0] == 1)
					deletion_point[1] -= FLITDB_PARTITION_AND_SEGMENT;
				else
					deletion_point[1] -= FLITDB_PARTITION_SIZE;
			}
			else // No more rows exist
				deletion_point[1] -= (deletion_point[1] - (deletion_point[1] - offset[1])); // Delete segment and partition
		}
		else if (offset[0] == offset[1]) // Segment declaration is at beginning of partition
			deletion_point[1] += FLITDB_PARTITION_SIZE; // Account for partition declaration
		unsigned short buffer_size = FLITDB_MAX_BUFFER_SIZE;
		size_t buffer_offset = 0;
		bool writable = (deletion_point[0] != (*handler)->size);
		while (writable)
 		{
			if ((deletion_point[0] + buffer_offset + buffer_size) >= (*handler)->size)
 			{
 				buffer_size = ((*handler)->size - (deletion_point[0] + buffer_offset));
 				writable = false;
 				if (buffer_size == 0)
 					break;
 			}
			memset((*handler)->buffer, 0, sizeof((*handler)->buffer));
 			fseek((*handler)->file_descriptor, (deletion_point[0] + buffer_offset), SEEK_SET);
 			if (fread((*handler)->buffer, buffer_size, 1, (*handler)->file_descriptor) != 1)
 			{
 				strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
 				return FLITDB_ERROR;
 			}
			fseek((*handler)->file_descriptor, (deletion_point[1] + buffer_offset), SEEK_SET);
			if (fwrite((*handler)->buffer, buffer_size, 1, (*handler)->file_descriptor) != 1)
			{
				strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			buffer_offset += buffer_size;
 		}
		(*handler)->size -= (current_length - offset_sizing);
		if (input_size == 0)
		{
			if (row_count[0] > 1)
				(*handler)->size -= FLITDB_SEGMENT_SIZE; // Segment length removed
			else
				(*handler)->size -= FLITDB_PARTITION_AND_SEGMENT; // Segment and partition length removed
		}
 		if (ftruncate(fileno((*handler)->file_descriptor), (*handler)->size) != 0)
		{
			if (input_buffer != NULL)
				free(input_buffer);
			flitdb_clear_values(handler);
			strncpy((*handler)->err_message, "Failed database truncation occurred\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_CORRUPT;
		}
		row_count[0] -= 1;
		if (row_count[0] == 0 && input_size == 0)
			removal = true;
	}
	else // Input size is equal to current size
		row_count[0] -= 1;
	if (offset[0] == offset[1])
		offset[1] += FLITDB_PARTITION_SIZE;
	// New end grouping
	info_skip_offset.position = offset[0];
	info_row_count.position = (offset[0] + 2);
	info_row_count.size = row_count[0];
	if (input_size == 0 && info_row_count.size > 0)
		info_row_count.size -= 1;
	if (input_size != 0 && !removal)
	{
		// MORE EXIST
		info_row_count.position = (offset[0] + 2);
		info_skip_offset.use = true;
		if (row_count[0] == 0)
		{
			if (current_length != 0)
				info_skip_offset.use = false; // Do not update next occuring skip offset (as it is already correct)
			else if (offset[0] != 0) // if other partitions exists prior to insertion point
				skip_amount[0] = (column_position - (skip_offset[0] + 1)); // Calculate insertion difference for skip amount
			else
				skip_amount[0] = (column_position - 1);
		}
		if (skip_amount[0] < 0)
			skip_amount[0] = 0;
		info_skip_offset.size = skip_amount[0];
		info_row_position.use = true;
		info_row_position.position = (offset[1]);
		info_input_size.use = true;
		info_input_size.position = (offset[1] + 2);
		info_input_type.use = true;
		info_input_type.position = (offset[1] + 4);
		info_input_buffer.use = true;
		info_input_buffer.position = (offset[1] + 5);
		info_row_count.use = true;
		if (current_length == 0 && row_count[0] == 0 && current_length != input_size) // Partition is new, so update skip offset for next partition 
		{
			offset[0] += (FLITDB_PARTITION_AND_SEGMENT + input_size);
			if (offset[0] != (*handler)->size)
			{
				if (fread(&skip_amount[1], sizeof(short), 1, (*handler)->file_descriptor) != 1)
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				if (skip_amount[1] == 1)
					skip_amount[0] = 0;
				else
					skip_amount[0] = (skip_amount[1] - (skip_amount[0] + 1));
				if (skip_amount[0] < 0)
				{
					strncpy((*handler)->err_message, "Skip offset negation detected\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
				fseek((*handler)->file_descriptor, offset[0], SEEK_SET);
				if (fwrite(&skip_amount[0], sizeof(short), 1, (*handler)->file_descriptor) != 1)
				{
					strncpy((*handler)->err_message, "An error occurred in attempting to write data to an updating skip offset notation in the database\0", FLITDB_MAX_ERR_SIZE);
					return FLITDB_ERROR;
				}
			}
		}
	}
	else if (offset[0] != (*handler)->size)
	{
		// Removed segment
		if (row_count[0] == 0)
		{
			info_skip_offset.use = true;
			fseek((*handler)->file_descriptor, offset[0], SEEK_SET);
			skip_amount[1] = skip_amount[0];
			if (fread(&skip_amount[0], 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
			{
				strncpy((*handler)->err_message, "An error occurred in attempting to read data from the database\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			skip_amount[0] += (skip_amount[1] + 1);
			if (skip_amount[0] < 0)
			{
				strncpy((*handler)->err_message, "Skip offset negation detected\0", FLITDB_MAX_ERR_SIZE);
				return FLITDB_ERROR;
			}
			info_skip_offset.size = skip_amount[0];
		}
		else
			info_row_count.use = true;
	}
	if (info_skip_offset.use)
	{
		fseek((*handler)->file_descriptor, info_skip_offset.position, SEEK_SET);
		if (fwrite(&info_skip_offset.size, sizeof(short), 1, (*handler)->file_descriptor) != 1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
	}
	if (info_row_count.use)
	{
		fseek((*handler)->file_descriptor, info_row_count.position, SEEK_SET);
		if (fwrite(&info_row_count.size, sizeof(short), 1, (*handler)->file_descriptor) != 1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
	}
	if (info_row_position.use)
	{
		fseek((*handler)->file_descriptor, info_row_position.position, SEEK_SET);
		if (fwrite(&info_row_position.size, sizeof(short), 1, (*handler)->file_descriptor) != 1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
	}
	if (info_input_size.use)
	{
		fseek((*handler)->file_descriptor, info_input_size.position, SEEK_SET);
		if (fwrite(&info_input_size.size, sizeof(short), 1, (*handler)->file_descriptor) != 1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
		}
	}
	if (info_input_type.use)
	{
		fseek((*handler)->file_descriptor, info_input_type.position, SEEK_SET);
		unsigned short input_type = 0;
		switch (info_input_type.size)
		{
			case FLITDB_INTEGER:
				input_type = 1;
				break;
			case FLITDB_DOUBLE:
				input_type = 2;
				break;
			case FLITDB_FLOAT:
				input_type = 3;
				break;
			case FLITDB_CHAR:
				input_type = 4;
				break;
			case FLITDB_BOOL:
				input_type = 5;
				break;
		}
		if (fwrite(&input_type, 1, 1, (*handler)->file_descriptor) != 1)
		{
			strncpy((*handler)->err_message, "An error occurred in attempting to write data to the database\0", FLITDB_MAX_ERR_SIZE);
			return FLITDB_ERROR;
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
			return -1; // Invalid
		}
	}
	unsigned short short_value = 0;
	for (unsigned short ii = 0; ii < i; ++ii)
		short_value = short_value * 10 + chars[ii] - '0'; // Calculates numeric representation
	return short_value; // Returns result
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
			if (!negative) // Checks if the value isn't already a negative
				negative = true;
			else
				return -1; // Invalid
			break;
		}
		default:
			return -1; // Invalid
		}
	}
	signed long long long_long_value = 0;
	for (unsigned short ii = 0; ii < i; ++ii)
		long_long_value = long_long_value * 10 + chars[ii] - '0'; // Calculates numeric representation
	return (((negative) ? -1 : 1) * long_long_value); // Returns result and sets negation state of result
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
			if (decimalized == -1 && i > 0) // Checks if the value isn't already a decimalized value
				decimalized = i; // Sets decimalization position
			else
				return -1; // Invalid
			break;
		}
		case '-':
		{
			if (!negative) // Checks if the value isn't already a negative
				negative = true;
			else
				return -1; // Invalid
			break;
		}
		default:
			return -1; // Invalid
		}
	}
	double double_value = 0;
	for (unsigned short ii = 0; ii < i; ++ii)
	{
		if (chars[ii] != '.' && chars[ii] != '-')
			double_value = double_value * 10 + chars[ii] - '0'; // Calculates numeric representation
	}
	i -= 1;
	if (decimalized != -1)
	{
		while (decimalized < i)
		{
			decimalized += 1;
			double_value /= 10; // Moves decimal position leftwards
		}
	}
	return (((negative) ? -1 : 1) * double_value); // Returns result and sets negation state of result
}

float flitdb_to_float(char *chars)
{
	return (float)flitdb_to_double(chars); // Returns double casted to float
}

long long unsigned int flitdb_abs(long long signed int value)
{
	return ((value > 0) ? value : -value); // Returns absolute value
}

#undef FLITDB_MAX_BUFFER_SIZE
#undef FLITDB_MAX_CHAR_LENGTH
#undef FLITDB_MAX_ERR_SIZE
#undef FLITDB_COLUMN_POSITION_MAX
#undef FLITDB_ROW_POSITION_MAX

#endif