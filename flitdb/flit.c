#ifndef flit_c
#define flit_c

#include <unistd.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "flit.h"

// FlitDB database operations
int flitdb_new(flitdb **handler);
void flitdb_destroy(flitdb **handler);
int flitdb_connection_setup(flitdb **handler, const char *filename, int flags);
char *flitdb_get_err_message(flitdb **handler);
unsigned char flitdb_read_at(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position);
void flitdb_clear_values(flitdb **handler);
unsigned char flitdb_insert_at(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position);
unsigned char flitdb_insert_value_int(flitdb **handler, int set_value);
unsigned char flitdb_insert_value_float(flitdb **handler, float set_value);
unsigned char flitdb_insert_value_char(flitdb **handler, char *set_value);
unsigned char flitdb_insert_value_bool(flitdb **handler, bool set_value);
void flitdb_insert_reset(flitdb **handler);
int flitdb_retrieve_value_int(flitdb **handler);
long double flitdb_retrieve_value_double(flitdb **handler);
float flitdb_retrieve_value_float(flitdb **handler);
char *flitdb_retrieve_value_char(flitdb **handler);
bool flitdb_retrieve_value_bool(flitdb **handler);
unsigned char flitdb_retrieve_value_type(flitdb **handler);
void flitdb_error_state(flitdb **handler, unsigned char error_id);
#if FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_TINY
union flitdb_read_mmap_response flitdb_read_mmap(unsigned int position, unsigned char size, void *mmapped_char);
#endif

#define FLITDB_MAX_BUFFER_SIZE 1024
#define FLITDB_MAX_CHAR_LENGTH (0xFFFF - sizeof(short))
#define FLITDB_MAX_ERR_SIZE 100
#define FLITDB_SEGMENT_SIZE 3
#define FLITDB_PARTITION_SIZE 4
#define FLITDB_PARTITION_AND_SEGMENT (FLITDB_SEGMENT_SIZE + FLITDB_PARTITION_SIZE)

#define FLITDB_READ_INT 1
#define FLITDB_READ_FLOAT 2
#define FLITDB_READ_CHAR 3
#define FLITDB_READ_BOOL_TRUE 4
#define FLITDB_READ_BOOL_FALSE 5

#ifndef FLITDB_SIZING_MODE
#error No sizing mode type was defined to FLITDB_SIZING_MODE
#elif FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_TINY
#if FLITDB_MMAP_ALLOWED
#define FLITDB_MMAP_OK
#endif
#define FLITDB_COLUMN_POSITION_MAX 0x000F
#define FLITDB_ROW_POSITION_MAX 0x000F
typedef unsigned int flitdb_sizing_max;
typedef unsigned short flitdb_size_selection_type;
#elif FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_SMALL
#define FLITDB_COLUMN_POSITION_MAX 0x00FF
#define FLITDB_ROW_POSITION_MAX 0x00FF
typedef unsigned int flitdb_size_selection_type;
typedef unsigned int flitdb_sizing_max;
#elif FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_BIG
#define FLITDB_COLUMN_POSITION_MAX 0xFFFF
#define FLITDB_ROW_POSITION_MAX 0xFFFF
#define FLITDB_ALLOW_UNSAFE
typedef unsigned long long flitdb_size_selection_type;
typedef unsigned long long flitdb_sizing_max;
#else
#error An invalid sizing mode was attributed to FLITDB_SIZING_MODE
#endif

unsigned int flitdb_version_check()
{
	return FLITDB_VERSION;
}

int flitdb_open(const char *filename, flitdb **handler, int flags)
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

int flitdb_extract(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position)
{
	return flitdb_read_at(handler, column_position, row_position);
}

int flitdb_retrieved_type(flitdb **handler)
{
	return flitdb_retrieve_value_type(handler);
}

int flitdb_insert_int(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, int value)
{
	unsigned char insert_success = flitdb_insert_value_int(handler, value);
	if (insert_success != FLITDB_DONE)
		return insert_success;
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, int value)
{
	return flitdb_insert_int(handler, column_position, row_position, value);
}

#endif

int flitdb_insert_float(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, float value)
{
	unsigned char insert_success = flitdb_insert_value_float(handler, value);
	if (insert_success != FLITDB_DONE)
		return insert_success;
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, float value)
{
	return flitdb_insert_float(handler, column_position, row_position, value);
}

#endif

int flitdb_insert_char(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, char *value)
{
	unsigned int value_length = strlen(value);
	if (value_length > FLITDB_MAX_CHAR_LENGTH)
	{
		flitdb_error_state(handler, 11);
		return FLITDB_ERROR;
	}
	unsigned char insert_success = flitdb_insert_value_char(handler, value);
	if (insert_success != FLITDB_DONE)
		return insert_success;
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, char *value)
{
	return flitdb_insert_char(handler, column_position, row_position, value);
}

#endif

int flitdb_insert_const_char(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, const char *value)
{
	unsigned int value_length = strlen(value);
	if (value_length > FLITDB_MAX_CHAR_LENGTH)
	{
		flitdb_error_state(handler, 11);
		return FLITDB_ERROR;
	}
	char tmp_value[(value_length + 1)];
	tmp_value[value_length] = '\0';
	for (unsigned short i = value_length; i > 0; i--)
		tmp_value[(i - 1)] = value[(i - 1)];
	return flitdb_insert_char(handler, column_position, row_position, tmp_value);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, const char *value)
{
	return flitdb_insert_const_char(handler, column_position, row_position, value);
}

#endif

int flitdb_insert_bool(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, bool value)
{
	int insert_success = flitdb_insert_value_bool(handler, (value == true));
	if (insert_success != FLITDB_DONE)
		return insert_success;
	return flitdb_insert_at(handler, column_position, row_position);
}

#ifdef __cplusplus

int flitdb_insert(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, bool value)
{
	return flitdb_insert_bool(handler, column_position, row_position, value);
}

#endif

int flitdb_delete(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position)
{
	flitdb_insert_reset(handler);
	return flitdb_insert_at(handler, column_position, row_position);
}

int flitdb_retrieve_int(flitdb **handler)
{
	return flitdb_retrieve_value_int(handler);
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

typedef struct flitdb
{
	char buffer[FLITDB_MAX_BUFFER_SIZE];
	bool configured;
	FILE *file_descriptor;
	char err_message[FLITDB_MAX_ERR_SIZE];
	union value
	{
		int int_value;
		float float_value;
		char char_value[(FLITDB_MAX_CHAR_LENGTH + 1)];
		bool bool_value;
	} value;
	unsigned char value_type;
	bool value_retrieved;
	flitdb_sizing_max size;
	bool read_only;
#ifdef FLITDB_ALLOW_UNSAFE
	bool unsafe;
#endif
} flitdb;

void flitdb_error_state(flitdb **handler, unsigned char error_id)
{
	const char *errors[] = { "\0",
							 "The minimum buffer size has encroached beyond suitable definitions\0",
							 "The maximum buffer size has encroached beyond suitable definitions\0",
							 "The database handler has already been attributed to handle another database\0",
							 "No database exists to be exclusively read\0",
							 "Failed to open the database\0",
							 "Exclusive rights to access the database could not be obtained\0",
							 "The database attempted to access has a larger size than what this object can read\0",
							 "The database handler has not been attributed to handle a database\0",
							 "The database was opened in readonly mode\0",
							 "Data insertion avoided due to unexpected tennant\0",
							 "Data insertion avoided due to the length of a string being too large\0",
#if FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_TINY
							 "The requested range was outside of the database's range (sizing mode parameter is: tiny)\0",
#elif FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_SMALL
							 "The requested range was outside of the database's range (sizing mode parameter is: small)\0",
#elif FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_BIG
							 "The requested range was outside of the database's range\0",
#endif
							 "The database contracted a malformed structure declaration\0",
							 "An error occurred in attempting to read data from the database\0",
							 "An error occurred in attempting to write data to the database\0",
							 "An error occurred in attempting to write data to an updating skip offset notation in the database\0",
							 "Failed database truncation occurred\0",
							 "An error occurred in attempting to retrieve data from the database\0",
							 "Data retrieval avoided due to the length of a string being too large\0",
							 "The database yielded an invalid datatype\0",
							 "The requested range must have a valid starting range of at least 1\0",
							 "The database declares ranges that exceed the current sizing mode parameter set\0",
							 "Unable to enable unsafe mode due to compilation sizing mode parameter set\0",
							 "An unknown error occurred\0",
	};
	if (error_id > 24)
		error_id = 24;
	strncpy((*handler)->err_message, errors[error_id], FLITDB_MAX_ERR_SIZE);
}

#ifdef FLITDB_MMAP_OK
union flitdb_read_mmap_response
{
	int integer;
	short short_integer;
	float floating_point;
	unsigned char bytes[4];
};

union flitdb_read_mmap_response flitdb_read_mmap(unsigned int position, unsigned char size, void *mmapped_char)
{
	union flitdb_read_mmap_response value;
	memset(value.bytes, 0, 4);
	unsigned char byte_position = 0;
	for (unsigned int i = position; i < (size + position); i++)
		value.bytes[byte_position++] = (char)(((char *)mmapped_char)[i]);
	return value;
}
#endif

void flitdb_clear_values(flitdb **handler)
{
	// Resets all the data associated with the current insert/retrieval
	(*handler)->value.int_value = 0;
	(*handler)->value_type = FLITDB_NULL;
	(*handler)->value_retrieved = false;
	memset((*handler)->buffer, 0, sizeof((*handler)->buffer));
}

int flitdb_new(flitdb **handler)
{
	*handler = &*(flitdb *)malloc(sizeof(flitdb)); // Attempts to request for memory
	if (*handler == NULL)						   // No memory was allocated
		return FLITDB_ERROR;
	(*handler)->configured = (FLITDB_MAX_BUFFER_SIZE < 50 || FLITDB_MAX_BUFFER_SIZE > 1024); // Buffer size is outside of operable bounds
	(*handler)->size = 0;
	(*handler)->read_only = false;
#ifdef FLITDB_ALLOW_UNSAFE
	(*handler)->unsafe = false;
#endif
	(*handler)->value_type = 0;
	(*handler)->value_retrieved = false;
	(*handler)->file_descriptor = NULL;
	if (FLITDB_MAX_BUFFER_SIZE < 50) // Buffer size is outside of operable bounds
	{
		flitdb_error_state(handler, 1);
		return FLITDB_ERROR;
	}
	else if (FLITDB_MAX_BUFFER_SIZE > 1024) // Buffer size is outside of operable bounds
	{
		flitdb_error_state(handler, 2);
		return FLITDB_ERROR;
	}
	else
		flitdb_error_state(handler, 0); // Clear error state
	flitdb_clear_values(handler);
	return FLITDB_SUCCESS;
}

void flitdb_destroy(flitdb **handler)
{
	if ((*handler)->configured)
	{
		(*handler)->configured = false;
		if ((*handler)->file_descriptor != NULL)
		{
			flock(fileno((*handler)->file_descriptor), LOCK_UN); // Removes lock on the database file of operation
			fclose((*handler)->file_descriptor);				 // Closes conenction to the database file of operation
		}
	}
}

const flitdb_sizing_max flitdb_max_size()
{
	// To calculate the maximum file size of what the database file can safely be read and written to
	flitdb_sizing_max insertion_area[2] = {0, 0};
	insertion_area[0] = FLITDB_COLUMN_POSITION_MAX;
	insertion_area[0] *= FLITDB_ROW_POSITION_MAX;
	insertion_area[0] *= (FLITDB_MAX_CHAR_LENGTH - 1);
	if (FLITDB_ROW_POSITION_MAX > 1)
	{
		insertion_area[1] = FLITDB_COLUMN_POSITION_MAX;
		insertion_area[1] *= FLITDB_ROW_POSITION_MAX;
		insertion_area[1] *= FLITDB_SEGMENT_SIZE;
	}
	return (insertion_area[0] + insertion_area[1] + (FLITDB_COLUMN_POSITION_MAX * FLITDB_PARTITION_AND_SEGMENT));
}

int flitdb_connection_setup(flitdb **handler, const char *filename, int flags)
{
	if ((*handler)->configured)
	{
		if (FLITDB_MAX_BUFFER_SIZE >= 50 && FLITDB_MAX_BUFFER_SIZE <= 1024) // If buffer size is within operable bounds
			flitdb_error_state(handler, 3);
		return FLITDB_ERROR;
	}
	(*handler)->size = 0;
	if ((flags & FLITDB_READONLY) == FLITDB_READONLY)
		(*handler)->read_only = true;
	bool file_exists = true;
	if (access(filename, F_OK) != 0) // If file exists
	{
		file_exists = false;
		if ((*handler)->read_only)
		{
			if ((flags & FLITDB_CREATE) == 0)
			{
				flitdb_error_state(handler, 4);
				return FLITDB_NOT_FOUND;
			}
		}
	}
	(*handler)->file_descriptor = fopen(filename, (((flags & FLITDB_READONLY) == FLITDB_READONLY) ? "r" : ((((flags & FLITDB_CREATE) == FLITDB_CREATE) && !file_exists) ? "w+" : "r+"))); // Opens a connection to a database file
	if ((*handler)->file_descriptor == NULL)
	{
		flitdb_error_state(handler, 5); // Failed to create connection to database file
		return FLITDB_PERM;
	}
	else
	{
		fseek((*handler)->file_descriptor, 0L, SEEK_END);	   // Move pointer to end of database file
		(*handler)->size = ftell((*handler)->file_descriptor); // Get position of pointer
		fseek((*handler)->file_descriptor, 0L, SEEK_SET);	   // Move pointer to start of database file
	}
	(*handler)->configured = true;											// Successfully configured correctly
	if (flock(fileno((*handler)->file_descriptor), LOCK_EX | LOCK_NB) != 0) // Attempts to lock the current database file
	{
		flitdb_error_state(handler, 6);
		return FLITDB_BUSY;
	}
	if ((flags & FLITDB_UNSAFE) == FLITDB_UNSAFE) // Checks if unsafe operations have been attributed to this handler
#ifdef FLITDB_ALLOW_UNSAFE
		(*handler)->unsafe = true; // Unsafe mode is enabled
#else
	{
		flitdb_error_state(handler, 23);
		return FLITDB_ERROR;
	}
#endif
	else if ((*handler)->size > flitdb_max_size()) // Checks if the database file is bigger than the maximum size for safe operations
	{
		flitdb_error_state(handler, 7);
		return FLITDB_RANGE;
	}
	return FLITDB_SUCCESS;
}

char *flitdb_get_err_message(flitdb **handler)
{
	if (*handler == NULL)
		return (char *)"This handler failed to be setup\0"; // Default error message if handler has not been setup
	return (*handler)->err_message;							// Returns an error message (if set)
}

unsigned char flitdb_insert_value_int(flitdb **handler, int set_value)
{
	if (!(*handler)->configured)
	{
		flitdb_error_state(handler, 8); // Handler not configured
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		flitdb_error_state(handler, 9); // Handler is in readonly mode
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		flitdb_error_state(handler, 10); // Handler already has value inserted
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);			 // Clears the current insert values
	(*handler)->value_type = FLITDB_INTEGER; // Sets the current insert value type to int
	(*handler)->value.int_value = set_value; // Sets the current insert value
	return FLITDB_DONE;
}

unsigned char flitdb_insert_value_float(flitdb **handler, float set_value)
{
	if (!(*handler)->configured)
	{
		flitdb_error_state(handler, 8); // Handler not configured
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		flitdb_error_state(handler, 9); // Handler is in readonly mode
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		flitdb_error_state(handler, 10); // Handler already has value inserted
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);			   // Clears the current insert values
	(*handler)->value_type = FLITDB_FLOAT;	   // Sets the current insert value type to float
	(*handler)->value.float_value = set_value; // Sets the current insert value
	return FLITDB_DONE;
}

unsigned char flitdb_insert_value_char(flitdb **handler, char *set_value)
{
	if (!(*handler)->configured)
	{
		flitdb_error_state(handler, 8); // Handler not configured
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		flitdb_error_state(handler, 9); // Handler is in readonly mode
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		flitdb_error_state(handler, 10); // Handler already has value inserted
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);															// Clears the current insert values
	(*handler)->value_type = FLITDB_CHAR;													// Sets the current insert value type to char array
	strncpy((*handler)->value.char_value, set_value, sizeof((*handler)->value.char_value)); // Sets the current insert value
	return FLITDB_DONE;
}

unsigned char flitdb_insert_value_bool(flitdb **handler, bool set_value)
{
	if (!(*handler)->configured)
	{
		flitdb_error_state(handler, 8); // Handler not configured
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		flitdb_error_state(handler, 9); // Handler is in readonly mode
		return FLITDB_READONLY;
	}
	if ((*handler)->value_type != FLITDB_NULL && !(*handler)->value_retrieved)
	{
		flitdb_error_state(handler, 10); // Handler already has value inserted
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);			  // Clears the current insert values
	(*handler)->value_type = FLITDB_BOOL;	  // Sets the current insert value type to boolean
	(*handler)->value.bool_value = set_value; // Sets the current insert value
	return FLITDB_DONE;
}

void flitdb_insert_reset(flitdb **handler)
{
	flitdb_clear_values(handler); // Clears the current insert values
	return;
}

int flitdb_retrieve_value_int(flitdb **handler)
{
	if ((*handler)->value_type == FLITDB_INTEGER)
		return (*handler)->value.int_value; // Returns the value stored in the int register
	return 0;
}

float flitdb_retrieve_value_float(flitdb **handler)
{
	if ((*handler)->value_type == FLITDB_FLOAT)
		return (*handler)->value.float_value; // Returns the value stored in the float register
	return 0;
}

char *flitdb_retrieve_value_char(flitdb **handler)
{
	if ((*handler)->value_type == FLITDB_CHAR)
		return (*handler)->value.char_value; // Returns the value stored in the char register
	return NULL;
}

bool flitdb_retrieve_value_bool(flitdb **handler)
{
	if ((*handler)->value_type == FLITDB_BOOL)
		return (*handler)->value.bool_value; // Returns the value stored in the bool register
	return false;
}

unsigned char flitdb_retrieve_value_type(flitdb **handler)
{
	return (*handler)->value_type; // Returns the value type
}

unsigned char flitdb_read_at(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position)
{
	if (!(*handler)->configured)
	{
		flitdb_error_state(handler, 8); // Handler not configured
		return FLITDB_ERROR;
	}
	flitdb_clear_values(handler);
#ifdef FLITDB_ALLOW_UNSAFE
	if (!(*handler)->unsafe)
	{
#endif
		if (column_position == 0 || (column_position - 1) > FLITDB_COLUMN_POSITION_MAX || row_position == 0 || (row_position - 1) > FLITDB_ROW_POSITION_MAX)
		{
			flitdb_error_state(handler, 12); // Outside of supported range
			flitdb_clear_values(handler);
			return FLITDB_RANGE;
		}
#ifdef FLITDB_ALLOW_UNSAFE
	}
	else
#endif
		if (column_position == 0 || row_position == 0)
	{
		flitdb_error_state(handler, 21); // Outside of supported range
		flitdb_clear_values(handler);
		return FLITDB_RANGE;
	}
	else if ((row_position - 1) > FLITDB_ROW_POSITION_MAX)
	{
		flitdb_error_state(handler, 12); // Outside of supported range
		flitdb_clear_values(handler);
		return FLITDB_RANGE;
	}
	row_position -= 1;
	bool store_response = false;
	flitdb_sizing_max offset = 0;
	flitdb_size_selection_type skip_offset = 0;
	unsigned char read_length = FLITDB_PARTITION_AND_SEGMENT;
	unsigned short row_count = 0;
#ifdef FLITDB_MMAP_OK
	void *mmapped_file = (void *)-1;
	if ((*handler)->read_only)
		mmapped_file = mmap(NULL, (*handler)->size, PROT_READ, MAP_PRIVATE, fileno((*handler)->file_descriptor), 0); // Attempt to allocate memory to map to file
	printf("MMAP!\n");
#endif
	for (;;)
	{
		if ((offset + read_length) > (*handler)->size) // Reached end of database file
		{
			if ((offset + read_length) > ((*handler)->size + read_length))
			{
				flitdb_error_state(handler, 13); // Exceeded sizing expectations
				return FLITDB_CORRUPT;
			}
			break; // Is end of database file
		}
#ifdef FLITDB_MMAP_OK
		if (mmapped_file == (void *)-1)
		{
#endif
			fseek((*handler)->file_descriptor, offset, SEEK_SET); // Go to position
			offset += read_length;
#ifdef FLITDB_MMAP_OK
		}
#endif
		if (read_length == FLITDB_PARTITION_AND_SEGMENT) //  If is new partition
		{
			unsigned short skip_amount;
#ifdef FLITDB_MMAP_OK
			if (mmapped_file != (void *)-1)
				skip_amount = (unsigned short)flitdb_read_mmap(offset, sizeof(short), mmapped_file).integer;
			else
#endif
				if (fread(&skip_amount, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short)) // Read skip amount from previous partition
			{
				flitdb_error_state(handler, 14);
				return FLITDB_ERROR;
			}
			skip_offset += skip_amount;
			if (skip_offset > FLITDB_COLUMN_POSITION_MAX)
			{
#ifdef FLITDB_ALLOW_UNSAFE
				if (!(*handler)->unsafe)
				{
#endif
#ifdef FLITDB_MMAP_OK
					if (mmapped_file != (void *)-1)
						munmap(mmapped_file, (*handler)->size); // Free up mmapped file memory
#endif
					flitdb_error_state(handler, 22);
					return FLITDB_RANGE;
#ifdef FLITDB_ALLOW_UNSAFE
				}
#endif
			}
			skip_offset += 1;
			if (skip_offset > column_position)
				return FLITDB_NULL;
#ifdef FLITDB_MMAP_OK
			if (mmapped_file != (void *)-1)
				row_count = (unsigned short)flitdb_read_mmap((offset + sizeof(short)), sizeof(short), mmapped_file).integer;
			else
#endif
				if (fread(&row_count, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short)) // Read row count for partition
			{
				flitdb_error_state(handler, 14);
				return FLITDB_ERROR;
			}
#if FLITDB_SIZING_MODE != FLITDB_SIZING_MODE_BIG
			if (row_count > FLITDB_ROW_POSITION_MAX)
			{
#ifdef FLITDB_MMAP_OK
				if (mmapped_file != (void *)-1)
					munmap(mmapped_file, (*handler)->size); // Free up mmapped file memory
#endif
				flitdb_error_state(handler, 22);
				return FLITDB_RANGE;
			}
#endif
			row_count += 1;
		}
		unsigned char set_read_length = FLITDB_PARTITION_AND_SEGMENT;
		if (skip_offset == column_position) // Is column that is to be read
		{
			unsigned short position;
#ifdef FLITDB_MMAP_OK
			if (mmapped_file != (void *)-1)
				position = (unsigned short)flitdb_read_mmap((offset + ((sizeof(short) * 2) * (read_length == FLITDB_PARTITION_AND_SEGMENT))), sizeof(short), mmapped_file).integer;
			else
#endif
				if (fread(&position, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short)) // Read row position
			{
				flitdb_error_state(handler, 14);
				return FLITDB_ERROR;
			}
			if (position == row_position) // Is row that is to be read
			{
				store_response = true;
				row_count = 0;
			}
#if FLITDB_SIZING_MODE != FLITDB_SIZING_MODE_BIG
			else if (position > FLITDB_ROW_POSITION_MAX)
			{
#ifdef FLITDB_MMAP_OK
				if (mmapped_file != (void *)-1)
					munmap(mmapped_file, (*handler)->size); // Free up mmapped file memory
#endif
				flitdb_error_state(handler, 22);
				return FLITDB_RANGE;
			}
#endif
		}
		else
#ifdef FLITDB_MMAP_OK
		if (mmapped_file == (void *)-1)
#endif
			fseek((*handler)->file_descriptor, sizeof(short), SEEK_CUR); // Ignore row position of current value
		if (row_count > 1)												 // If more segments remain
		{
			row_count -= 1;
			set_read_length = FLITDB_SEGMENT_SIZE;
		}
		unsigned short response_length;
		unsigned char data_type;
#ifdef FLITDB_MMAP_OK
		flitdb_sizing_max offset_mmap_standard_diff = offset + (sizeof(short) * 3);
		if (read_length == FLITDB_SEGMENT_SIZE)
			offset_mmap_standard_diff = (offset + sizeof(short));
		if (mmapped_file != (void *)-1)
		{
			if (read_length == FLITDB_PARTITION_AND_SEGMENT)
				data_type = (unsigned short)flitdb_read_mmap((offset + (sizeof(short) * 3)), 1, mmapped_file).integer;
			else
				data_type = (unsigned short)flitdb_read_mmap((offset + sizeof(short)), 1, mmapped_file).integer;
			offset_mmap_standard_diff += 1;
		}
		else
#endif
			if (fread(&data_type, 1, 1, (*handler)->file_descriptor) != 1) // Read data type of current value
		{
			flitdb_error_state(handler, 14);
			return FLITDB_ERROR;
		}
		(*handler)->value_retrieved = store_response;
		switch (data_type)
		{
		case FLITDB_READ_INT:
			if (store_response)
			{
				(*handler)->value_type = FLITDB_INTEGER;
#ifdef FLITDB_MMAP_OK
				if (mmapped_file != (void *)-1)
					(*handler)->value.int_value = flitdb_read_mmap(offset_mmap_standard_diff, sizeof(int), mmapped_file).integer;
				else
#endif
					if (fread(&(*handler)->value.int_value, 1, sizeof(int), (*handler)->file_descriptor) != sizeof(int)) // Read and store integer value
				{
					flitdb_error_state(handler, 18);
					return FLITDB_ERROR;
				}
#ifdef FLITDB_MMAP_OK
				if (mmapped_file != (void *)-1)
					munmap(mmapped_file, (*handler)->size); // Free up mmapped file memory
#endif
				return FLITDB_DONE;
			}
			data_type = FLITDB_INTEGER;
			response_length = sizeof(int);
			break;
		case FLITDB_READ_FLOAT:
			if (store_response)
			{
				(*handler)->value_type = FLITDB_FLOAT;
#ifdef FLITDB_MMAP_OK
				if (mmapped_file != (void *)-1)
					(*handler)->value.float_value = flitdb_read_mmap(offset_mmap_standard_diff, sizeof(float), mmapped_file).floating_point;
				else
#endif
					if (fread(&(*handler)->value.float_value, 1, sizeof(float), (*handler)->file_descriptor) != sizeof(float)) // Read and store float value
				{
					flitdb_error_state(handler, 18);
					return FLITDB_ERROR;
				}
#ifdef FLITDB_MMAP_OK
				if (mmapped_file != (void *)-1)
					munmap(mmapped_file, (*handler)->size); // Free up mmapped file memory
#endif
				return FLITDB_DONE;
			}
			data_type = FLITDB_FLOAT;
			response_length = sizeof(float);
			break;
		case FLITDB_READ_CHAR:
			data_type = FLITDB_CHAR;
#ifdef FLITDB_MMAP_OK
			if (mmapped_file != (void *)-1)
				response_length = (unsigned short)flitdb_read_mmap(offset_mmap_standard_diff, sizeof(short), mmapped_file).integer;
			else
#endif
				if (fread(&response_length, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short)) // Read length of char array
			{
				flitdb_error_state(handler, 18);
				return FLITDB_ERROR;
			}
			response_length += 1;
			if (response_length > FLITDB_MAX_CHAR_LENGTH)
			{
#ifdef FLITDB_MMAP_OK
				if (mmapped_file != (void *)-1)
					munmap(mmapped_file, (*handler)->size); // Free up mmapped file memory
#endif
				flitdb_error_state(handler, 19);
				return FLITDB_ERROR;
			}
			if (store_response)
			{
				(*handler)->value_type = data_type;
				memset((*handler)->value.char_value, 0, sizeof((*handler)->value.char_value));
#ifdef FLITDB_MMAP_OK
				if (mmapped_file != (void *)-1)
				{
					memset((*handler)->value.char_value, 0, FLITDB_MAX_CHAR_LENGTH);
					flitdb_sizing_max offset_diff = (offset_mmap_standard_diff + sizeof(short));
					for (flitdb_sizing_max i = 0; i < response_length; i++)
						(*handler)->value.char_value[i] = ((char *)mmapped_file)[(i + offset_diff)];
				}
				else
#endif
					if (fread((*handler)->value.char_value, response_length, sizeof(char), (*handler)->file_descriptor) != sizeof(char)) // Read and store char array
				{
					flitdb_error_state(handler, 18);
					return FLITDB_ERROR;
				}
#ifdef FLITDB_MMAP_OK
				if (mmapped_file != (void *)-1)
					munmap(mmapped_file, (*handler)->size); // Free up mmapped file memory
#endif
				if (strlen((*handler)->value.char_value) == 0)
				{
					flitdb_error_state(handler, 14);
					return FLITDB_ERROR;
				}
				return FLITDB_DONE;
			}
			response_length += sizeof(short);
			break;
		case FLITDB_READ_BOOL_TRUE:
		case FLITDB_READ_BOOL_FALSE:
			if (store_response)
			{
				(*handler)->value_type = FLITDB_BOOL;
				(*handler)->value.bool_value = (data_type == FLITDB_READ_BOOL_TRUE); // Store boolean value
#ifdef FLITDB_MMAP_OK
				if (mmapped_file != (void *)-1)
					munmap(mmapped_file, (*handler)->size); // Free up mmapped file memory
#endif
				return FLITDB_DONE;
			}
			data_type = FLITDB_BOOL;
			response_length = 0;
			break;
		default: // Unknown
			(*handler)->value_retrieved = false;
#ifdef FLITDB_MMAP_OK
			if (mmapped_file != (void *)-1)
				munmap(mmapped_file, (*handler)->size); // Free up mmapped file memory
#endif
			flitdb_error_state(handler, 20);
			return FLITDB_CORRUPT;
		}
#ifdef FLITDB_MMAP_OK
		if (mmapped_file != (void *)-1)
			offset += read_length;
#endif
		read_length = set_read_length;
		offset += response_length;
	}
#ifdef FLITDB_MMAP_OK
	if (mmapped_file != (void *)-1)
		munmap(mmapped_file, (*handler)->size); // Free up mmapped file memory
#endif
	return FLITDB_NULL;
}

unsigned char flitdb_insert_at(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position)
{
	if (!(*handler)->configured)
	{
		flitdb_error_state(handler, 8);
		return FLITDB_ERROR;
	}
	if ((*handler)->read_only)
	{
		flitdb_error_state(handler, 9);
		flitdb_clear_values(handler);
		return FLITDB_READONLY;
	}
#ifdef FLITDB_ALLOW_UNSAFE
	if (!(*handler)->unsafe)
	{
#endif
		if (column_position == 0 || (column_position - 1) > FLITDB_COLUMN_POSITION_MAX || row_position == 0 || (row_position - 1) > FLITDB_ROW_POSITION_MAX)
		{
			flitdb_error_state(handler, 12);
			flitdb_clear_values(handler);
			return FLITDB_RANGE;
		}
#ifdef FLITDB_ALLOW_UNSAFE
	}
	else
#endif
		if (column_position == 0 || row_position == 0)
	{
		flitdb_error_state(handler, 21);
		flitdb_clear_values(handler);
		return FLITDB_RANGE;
	}
	else if ((row_position - 1) > FLITDB_ROW_POSITION_MAX)
	{
		flitdb_error_state(handler, 12);
		flitdb_clear_values(handler);
		return FLITDB_RANGE;
	}
	row_position -= 1;
	char *input_buffer = NULL;
	unsigned short input_size = 0;
	switch ((*handler)->value_type)
	{
	case FLITDB_INTEGER:
		input_size = sizeof(int);
		free(input_buffer);
		break;
	case FLITDB_FLOAT:
		input_size = sizeof(float);
		free(input_buffer);
		break;
	case FLITDB_BOOL:
		free(input_buffer);
		break;
	case FLITDB_CHAR:
		input_size = strlen((*handler)->value.char_value);
		if (input_size == 0)
		{
			flitdb_clear_values(handler);
			if (input_buffer != NULL)
				free(input_buffer);
		}
		else
		{
			input_buffer = (char *)calloc((input_size + 1), sizeof(char));
			input_buffer[input_size] = '\0';
			strncpy(input_buffer, (*handler)->value.char_value, input_size);
		}
		break;
	}
	const unsigned short input_size_default = input_size;
	if ((*handler)->value_type == FLITDB_CHAR)
		input_size += sizeof(short);
	flitdb_sizing_max offset[6] = {0, 0, 0, 0, 0, 0};
	flitdb_size_selection_type skip_offset[2] = {0, 0};
	unsigned short skip_amount[2] = {0, 0};
	unsigned short read_length[2] = {FLITDB_PARTITION_AND_SEGMENT, FLITDB_PARTITION_AND_SEGMENT};
	unsigned short row_count[3] = {0, 0, 0};
	unsigned short current_length[2] = {0, 0};
	unsigned char current_type = 0;

	// READ STRUCTURE
	if ((*handler)->size > 0)
	{
		for (;;)
		{
			read_length[1] = read_length[0]; // Store previous size
			offset[2] = offset[0];			 // Store segment position
			if (offset[0] >= (*handler)->size)
			{
				if ((offset[0] - current_length[0]) > (*handler)->size)
				{
					flitdb_error_state(handler, 13);
					return FLITDB_CORRUPT;
				}
				else if (skip_offset[0] == column_position) // Add segment to end
				{
					offset[0] = offset[3];
					offset[1] = (*handler)->size;
					row_count[0] = row_count[1]; // Set row count to row default
					current_length[0] = 0;
				}
				else if (skip_offset[0] < column_position) // Insert partition to end
				{
					offset[0] = (*handler)->size;
					offset[1] = offset[0];
					row_count[0] = 0; // Set row count to 0
					current_length[0] = 0;
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
					flitdb_error_state(handler, 14);
					return FLITDB_ERROR;
				}
				skip_offset[1] = skip_offset[0];
				skip_offset[0] += skip_amount[0];
				if (skip_offset[0] > FLITDB_COLUMN_POSITION_MAX)
				{
#ifdef FLITDB_ALLOW_UNSAFE
					if (!(*handler)->unsafe)
					{
#endif
						flitdb_error_state(handler, 22);
						return FLITDB_RANGE;
#ifdef FLITDB_ALLOW_UNSAFE
					}
#endif
				}
				skip_offset[0] += 1;
				if (fread(&row_count[0], 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
				{
					flitdb_error_state(handler, 14);
					return FLITDB_ERROR;
				}
#if FLITDB_SIZING_MODE != FLITDB_SIZING_MODE_BIG
				if (row_count[0] > FLITDB_ROW_POSITION_MAX)
				{
					flitdb_error_state(handler, 22);
					return FLITDB_RANGE;
				}
#endif
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
			current_length[0] = 0;
			if (skip_offset[0] == column_position) // Reached target column
			{
				skip_offset[1] = skip_offset[0];
				unsigned short position;
				if (fread(&position, 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
				{
					flitdb_error_state(handler, 14);
					return FLITDB_ERROR;
				}
				if (position == row_position) // Is current row
				{
					if (fread(&current_type, 1, 1, (*handler)->file_descriptor) != 1)
					{
						flitdb_error_state(handler, 14);
						return FLITDB_ERROR;
					}
					current_length[1] = 1;
					switch (current_type)
					{
					case FLITDB_READ_INT:
						current_length[0] = sizeof(int);
						break;
					case FLITDB_READ_FLOAT:
						current_length[0] = sizeof(float);
						break;
					case FLITDB_READ_CHAR:
						if (fread(&current_length[0], 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
						{
							flitdb_error_state(handler, 14);
							return FLITDB_ERROR;
						}
						current_length[0] += (1 + sizeof(short));
						break;
					case FLITDB_READ_BOOL_TRUE:
					case FLITDB_READ_BOOL_FALSE:
						break;
					default:
						flitdb_error_state(handler, 20);
						return FLITDB_ERROR;
					}
					offset[0] = offset[3];		 // Beginning of partition
					offset[1] = offset[2];		 // Beginning of segment
					row_count[0] = row_count[1]; // Set row count to total rows
					break;
				}
#if FLITDB_SIZING_MODE != FLITDB_SIZING_MODE_BIG
				else if (position > FLITDB_ROW_POSITION_MAX)
				{
					flitdb_error_state(handler, 22);
					return FLITDB_RANGE;
				}
#endif
				else if (row_count[0] >= 1 && position > row_position) // Passed target row (doesn't exist) but additional rows exist
				{
					offset[0] = offset[3];		 // Beginning of segment
					offset[1] = offset[2];		 // Beginning of partition
					current_length[0] = 0;		 // Does not exist so set length of it to 0
					row_count[0] = row_count[1]; // Set row count to total rows
					break;
				}
				else if (row_position < position) // Passed target row (doesn't exist)
				{
					offset[0] = offset[2];		 // Beginning of segment
					current_length[0] = 0;		 // Does not exist so set length of it to 0
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
					offset[0] = offset[2];			   // Beginning of new partition
				read_length[0] = read_length[1];	   // Set length to previous
				current_length[0] = 0;				   // Does not exist so set length of it to 0
				if (skip_offset[1] == column_position) // If previous was the correct column
					row_count[0] = row_count[2];	   // Use previous row count
				else
					row_count[0] = 0; // Does not exist so row count is 0
				break;
			}
			else
				fseek((*handler)->file_descriptor, (3 - skip_read_offset), SEEK_CUR);
			if (fread(&current_type, 1, 1, (*handler)->file_descriptor) != 1)
			{
				flitdb_error_state(handler, 14);
				return FLITDB_ERROR;
			}
			switch (current_type)
			{
			case FLITDB_READ_INT:
				current_length[0] = sizeof(int);
				break;
			case FLITDB_READ_FLOAT:
				current_length[0] = sizeof(float);
				break;
			case FLITDB_READ_CHAR:
				if (fread(&current_length[0], 1, sizeof(short), (*handler)->file_descriptor) != sizeof(short))
				{
					flitdb_error_state(handler, 14);
					return FLITDB_ERROR;
				}
				current_length[0] += (1 + sizeof(short));
				break;
			case FLITDB_READ_BOOL_TRUE:
			case FLITDB_READ_BOOL_FALSE:
				break;
			default:
				flitdb_error_state(handler, 20);
				return FLITDB_ERROR;
			}
			current_type = 0;
			if (row_count[0] == 1)
				read_length[0] = FLITDB_PARTITION_AND_SEGMENT;
			row_count[0] -= 1;
			offset[0] += current_length[0];
		}
	}
	if (current_length[1] == 0 && (input_size == 0 && (*handler)->value_type == FLITDB_NULL))
		return FLITDB_DONE; // No need to write any data
	struct relinquish_excersion
	{
		unsigned short size;
		flitdb_sizing_max position;
		bool use;
	};
	struct relinquish_excersion info_skip_offset = {
		.size = 0,
		.position = 0,
		.use = false
	};
	struct relinquish_excersion info_row_count = {
		.size = (unsigned short)((((input_size == 0 && (*handler)->value_type == FLITDB_NULL) || current_length[0] == 1) && info_row_count.size != 0) ? (row_count[0] - 1) : row_count[0]),
		.position = 0,
		.use = false
	};
	struct relinquish_excersion info_row_position = {
		.size = (unsigned short)row_position,
		.position = 0,
		.use = false
	};
	struct relinquish_excersion info_input_size = {
		.size = (unsigned short)(((*handler)->value_type == FLITDB_CHAR) ? (input_size - 1) : 0),
		.position = 0,
		.use = 0
	};
	struct relinquish_excersion info_input_type = {
		.size = (*handler)->value_type,
		.position = 0,
		.use = false
	};
	struct relinquish_excersion info_input_buffer = {
		.size = 0,
		.position = 0,
		.use = 0
	};
	bool removal = false;
	if (input_size > current_length[0] || (current_length[1] == 0 && (*handler)->value_type != FLITDB_NULL))
	{
		unsigned short offset_sizing = (input_size - current_length[0]);
		unsigned char additional_offset = 0;
		if (current_length[1] == 0)
		{
			if (row_count[0] == 0)
				additional_offset = FLITDB_PARTITION_AND_SEGMENT;
			else
				additional_offset = FLITDB_SEGMENT_SIZE;
		}
		if (offset[1] < (*handler)->size)
		{
			// Move segments after offset position
			flitdb_sizing_max buffer_delimitation_point = (offset[1]);
			flitdb_sizing_max buffer_offset = (((*handler)->size - offset[1]) % FLITDB_MAX_BUFFER_SIZE);
			if (buffer_offset == 0)
				buffer_offset = FLITDB_MAX_BUFFER_SIZE;
			unsigned short buffer_size = buffer_offset;
			for (;;)
			{
				memset((*handler)->buffer, 0, sizeof((*handler)->buffer));
				fseek((*handler)->file_descriptor, ((*handler)->size - buffer_offset), SEEK_SET);
				if (fread((*handler)->buffer, buffer_size, 1, (*handler)->file_descriptor) != 1)
				{
					flitdb_error_state(handler, 14);
					return FLITDB_ERROR;
				}
				fseek((*handler)->file_descriptor, (((*handler)->size - buffer_offset) + offset_sizing + additional_offset), SEEK_SET);
				if (fwrite((*handler)->buffer, buffer_size, 1, (*handler)->file_descriptor) != 1)
				{
					flitdb_error_state(handler, 15);
					return FLITDB_ERROR;
				}
				if (((*handler)->size - buffer_offset) <= buffer_delimitation_point)
					break;
				buffer_size = FLITDB_MAX_BUFFER_SIZE;
				buffer_offset += buffer_size;
			}
			fseek((*handler)->file_descriptor, (((*handler)->size - buffer_offset) + offset_sizing + additional_offset), SEEK_SET);
		}
		(*handler)->size += offset_sizing;
		if (current_length[1] == 0)
			(*handler)->size += additional_offset;
		else
			row_count[0] -= 1;
	}
	else if (input_size < current_length[0] || (input_size < current_length[0] && (*handler)->value_type == FLITDB_CHAR) || (*handler)->value_type == FLITDB_NULL)
	{
		unsigned short offset_sizing = (current_length[0] - (current_length[0] - input_size));
		if (row_count[0] == 1)
		{
			offset[3] = offset[1]; // Store beginging of segment
			offset[1] = offset[4]; // Set to beginging of segment
		}
		flitdb_sizing_max deletion_point[2] = {
			(offset[1] + FLITDB_SEGMENT_SIZE + current_length[0]), // End point of current
			(offset[1] + FLITDB_SEGMENT_SIZE + offset_sizing),	   // New end point position
		};
		if (offset[0] == offset[1])
			deletion_point[0] += FLITDB_PARTITION_SIZE;
		if (input_size == 0 && (*handler)->value_type == FLITDB_NULL) // Removing data
		{
			if (row_count[0] > 1) // More rows exist
			{
				if (offset[0] == offset[1])
				{
					deletion_point[1] -= (((deletion_point[1] - offset[0]))); // Delete partition
					deletion_point[1] += FLITDB_PARTITION_SIZE;
				}
				else
					deletion_point[1] -= (FLITDB_SEGMENT_SIZE);
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
			else																			// No more rows exist
				deletion_point[1] -= (deletion_point[1] - (deletion_point[1] - offset[1])); // Delete segment and partition
		}
		else if (offset[0] == offset[1])				// Segment declaration is at beginning of partition
			deletion_point[1] += FLITDB_PARTITION_SIZE; // Account for partition declaration
		unsigned short buffer_size = FLITDB_MAX_BUFFER_SIZE;
		flitdb_sizing_max buffer_offset = 0;
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
				flitdb_error_state(handler, 14);
				return FLITDB_ERROR;
			}
			fseek((*handler)->file_descriptor, (deletion_point[1] + buffer_offset), SEEK_SET);
			if (fwrite((*handler)->buffer, buffer_size, 1, (*handler)->file_descriptor) != 1)
			{
				flitdb_error_state(handler, 15);
				return FLITDB_ERROR;
			}
			buffer_offset += buffer_size;
		}
		(*handler)->size -= (current_length[0] - offset_sizing);
		if (input_size == 0 && (*handler)->value_type == FLITDB_NULL)
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
			flitdb_error_state(handler, 17);
			return FLITDB_CORRUPT;
		}
		row_count[0] -= 1;
		if (row_count[0] == 0 && input_size == 0 && (*handler)->value_type == FLITDB_NULL)
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
	if ((*handler)->value_type == FLITDB_NULL)
		info_row_count.size -= 1;
	if (((input_size == 0 && (*handler)->value_type != FLITDB_NULL) ? true : (input_size != 0)) && !removal)
	{
		// MORE EXIST
		info_skip_offset.use = true;
		if (row_count[0] == 0)
		{
			if (current_length[0] != 0 || current_length[1] == 1)		   // If empty or existing
				info_skip_offset.use = false;							   // Do not update next occuring skip offset (as it is already correct)
			else if (offset[0] != 0)									   // if other partitions exists prior to insertion point
				skip_amount[0] = (column_position - (skip_offset[0] + 1)); // Calculate insertion difference for skip amount
			else
				skip_amount[0] = (column_position - 1);
		}
		info_skip_offset.size = skip_amount[0];
		info_row_position.use = true;
		info_row_position.position = (offset[1]);
		info_input_type.use = true;
		info_input_type.position = (offset[1] + 2);
		if ((*handler)->value_type == FLITDB_CHAR)
		{
			info_input_size.use = true;
			info_input_size.position = (offset[1] + 3);
			info_input_buffer.position = (offset[1] + 5);
		}
		else
			info_input_buffer.position = (offset[1] + 3);
		info_input_buffer.use = ((*handler)->value_type != FLITDB_BOOL);
		info_row_count.use = true;
		if (current_length[0] == 0 && current_length[1] == 0 && row_count[0] == 0 && (current_length[0] != input_size || (*handler)->value_type == FLITDB_BOOL)) // Partition is new, so update skip offset for next partition
		{
			offset[0] += (FLITDB_PARTITION_AND_SEGMENT + input_size);
			if (offset[0] != (*handler)->size)
			{
				if (fread(&skip_amount[1], sizeof(short), 1, (*handler)->file_descriptor) != 1)
				{
					flitdb_error_state(handler, 14);
					return FLITDB_ERROR;
				}
				if (skip_amount[1] == 1)
					skip_amount[0] = 0;
				else
					skip_amount[0] = (skip_amount[1] - (skip_amount[0] + 1));
				fseek((*handler)->file_descriptor, offset[0], SEEK_SET);
				if (fwrite(&skip_amount[0], sizeof(short), 1, (*handler)->file_descriptor) != 1)
				{
					flitdb_error_state(handler, 16);
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
				flitdb_error_state(handler, 14);
				return FLITDB_ERROR;
			}
			skip_amount[0] += (skip_amount[1] + 1);
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
			flitdb_error_state(handler, 15);
			return FLITDB_ERROR;
		}
	}
	if (info_row_count.use)
	{
		fseek((*handler)->file_descriptor, info_row_count.position, SEEK_SET);
		if (fwrite(&info_row_count.size, sizeof(short), 1, (*handler)->file_descriptor) != 1)
		{
			flitdb_error_state(handler, 15);
			return FLITDB_ERROR;
		}
	}
	if (info_row_position.use)
	{
		fseek((*handler)->file_descriptor, info_row_position.position, SEEK_SET);
		if (fwrite(&info_row_position.size, sizeof(short), 1, (*handler)->file_descriptor) != 1)
		{
			flitdb_error_state(handler, 15);
			return FLITDB_ERROR;
		}
	}
	if (info_input_size.use)
	{
		if ((*handler)->value_type == FLITDB_CHAR)
			info_input_size.size -= sizeof(short);
		fseek((*handler)->file_descriptor, info_input_size.position, SEEK_SET);
		if (fwrite(&info_input_size.size, sizeof(short), 1, (*handler)->file_descriptor) != 1)
		{
			flitdb_error_state(handler, 15);
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
		case FLITDB_FLOAT:
			input_type = 2;
			break;
		case FLITDB_CHAR:
			input_type = 3;
			break;
		case FLITDB_BOOL:
			input_type = ((*handler)->value.bool_value ? 4 : 5);
			break;
		}
		if (fwrite(&input_type, 1, 1, (*handler)->file_descriptor) != 1)
		{
			flitdb_error_state(handler, 15);
			return FLITDB_ERROR;
		}
	}
	if (info_input_buffer.use)
	{
		fseek((*handler)->file_descriptor, info_input_buffer.position, SEEK_SET);
		switch (info_input_type.size)
		{
		case FLITDB_INTEGER:
			if (fwrite(&(*handler)->value.int_value, sizeof(int), 1, (*handler)->file_descriptor) != 1)
			{
				flitdb_error_state(handler, 15);
				return FLITDB_ERROR;
			}
			break;
		case FLITDB_FLOAT:
			if (fwrite(&(*handler)->value.float_value, sizeof(float), 1, (*handler)->file_descriptor) != 1)
			{
				flitdb_error_state(handler, 15);
				return FLITDB_ERROR;
			}
			break;
		case FLITDB_CHAR:
			if (fwrite(input_buffer, input_size_default, sizeof(char), (*handler)->file_descriptor) != sizeof(char))
			{
				flitdb_error_state(handler, 15);
				return FLITDB_ERROR;
			}
		}
	}
	if ((*handler)->value_type != FLITDB_NULL)
		if (input_buffer != NULL)
			free(input_buffer);
	flitdb_clear_values(handler);
	return FLITDB_DONE;
}

#undef FLITDB_MAX_BUFFER_SIZE
#undef FLITDB_MAX_CHAR_LENGTH
#undef FLITDB_MAX_ERR_SIZE
#undef FLITDB_COLUMN_POSITION_MAX
#undef FLITDB_ROW_POSITION_MAX
#undef FLITDB_SEGMENT_SIZE
#undef FLITDB_PARTITION_SIZE
#undef FLITDB_PARTITION_AND_SEGMENT
#undef FLITDB_READ_INT
#undef FLITDB_READ_FLOAT
#undef FLITDB_READ_CHAR
#undef FLITDB_READ_BOOL_TRUE
#undef FLITDB_READ_BOOL_FALSE
#undef FLITDB_MMAP_OK
#undef FLITDB_COLUMN_POSITION_MAX
#undef FLITDB_ROW_POSITION_MAX
#undef FLITDB_ALLOW_UNSAFE

#endif