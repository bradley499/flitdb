#ifndef flit_h
#define flit_h
#include <stdbool.h>

enum flitdb_response_t
{
	FLITDB_SUCCESS   = 0,  // Successful operation
	FLITDB_ERROR     = 1,  // Unsuccessful operation
	FLITDB_PERM      = 2,  // Permission denied
	FLITDB_BUSY      = 3,  // The database file is locked
	FLITDB_NOT_FOUND = 4,  // The database file is not found
	FLITDB_CORRUPT   = 5,  // The database file is malformed
	FLITDB_RANGE     = 6,  // The requested range is outside the range of the database
	FLITDB_CREATE    = 7,  // Create a database if not existent
	FLITDB_READONLY  = 8,  // Only allow the reading of the database
	FLITDB_DONE      = 9,  // The operation was completed successfully
	FLITDB_NULL      = 10, // The operation resulted in a null lookup
	FLITDB_INTEGER   = 11, // The value type of int
	FLITDB_FLOAT     = 12, // The value type of float
	FLITDB_CHAR      = 13, // The value type of char
	FLITDB_BOOL      = 14, // The value type of bool
	FLITDB_UNSAFE    = 16, // Discard all safety protocols to allow for larger database
};

#define FLITDB_VERSION  0x117ee // The current FlitDB version magic number

// Database sizing options
#define FLITDB_SIZING_MODE_TINY  1 // Handle databases up to 14.74 megabytes in size
#define FLITDB_SIZING_MODE_SMALL 2 // Handle databases up to 4.26 gigabytes in size
#define FLITDB_SIZING_MODE_BIG   3 // Handle databases up to 281.47 terabytes in size

// Database memory mapping
#define FLITDB_MMAP_ALLOWED 1 // Allows the database to memory map files - if possible (1 - allowed, 0 - disallowed)

// Database sizing selection
#define FLITDB_SIZING_MODE FLITDB_SIZING_MODE_BIG // The sizing mode for this compilation

#if FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_BIG
#define FLITDB_COLUMN_POSITION_MAX 0xFFFF
#define FLITDB_ROW_POSITION_MAX 0xFFFF
#elif FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_SMALL
#define FLITDB_COLUMN_POSITION_MAX 0x00FF
#define FLITDB_ROW_POSITION_MAX 0x00FF
#elif FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_TINY
#define FLITDB_COLUMN_POSITION_MAX 0x000F
#define FLITDB_ROW_POSITION_MAX 0x000F
#endif

#ifdef __cplusplus
#define flitdb_extern extern "C"
#else
#define flitdb_extern extern
#endif

typedef struct flitdb flitdb;
#if FLITDB_SIZING_MODE == FLITDB_SIZING_MODE_BIG
typedef unsigned long long flitdb_column_row_sizing;
#else
typedef unsigned short flitdb_column_row_sizing;
#endif

/**
 * @brief Configures the FlitDB handler to point to and operate on a specific file
 * 
 * @param filename The path for the file to operate on
 * @param handler The FlitDB handler
 * @param flags The read write operation flags to set on the database file
 * @return int 
 */
flitdb_extern int flitdb_open(const char *filename, flitdb **handler, int flags);

/**
 * @brief Close the connection to the database, and delete the FlitDB handler object
 * 
 * @param handler The FlitDB handler
 * @return int 
 */
flitdb_extern int flitdb_close(flitdb **handler);

/**
 * @brief Return the current version of the FlitDB API
 * 
 * @return unsigned int
 */
flitdb_extern unsigned int flitdb_version_check();

/**
 * @brief Retrieve an error message from the FlitDB handler if once exists
 * 
 * @param handler The FlitDB handler 
 * @return char* 
 */
flitdb_extern char* flitdb_errmsg(flitdb **handler);

/**
 * @brief Extract a value stored within the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to extract the value from
 * @param row_position The row that you wish to extract the value from
 * @return int 
 */
flitdb_extern int flitdb_extract(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position);

/**
 * @brief Retrieve a numeric representation of what data type was retrieved:
 * 		  FLITDB_NULL,
 * 		  FLITDB_INTEGER,
 * 		  FLITDB_FLOAT,
 * 		  FLITDB_CHAR,
 * 		  FLITDB_BOOL
 * 
 * @param handler 
 * @return int 
 */
flitdb_extern int flitdb_retrieved_type(flitdb **handler);

#ifdef __cplusplus

/**
 * @brief Insert an integer (int) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, int value);

#endif

/**
 * @brief Insert an integer (int) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
flitdb_extern int flitdb_insert_int(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, int value);

#ifdef __cplusplus

/**
 * @brief Insert an float into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, float value);

#endif

/**
 * @brief Insert an float into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
flitdb_extern int flitdb_insert_float(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, float value);

#ifdef __cplusplus

/**
 * @brief Insert an char* into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, char* value);

#endif

/**
 * @brief Insert an char* into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
flitdb_extern int flitdb_insert_char(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, char* value);

#ifdef __cplusplus

/**
 * @brief Insert an const char* into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, const char* value);

#endif

/**
 * @brief Insert an const char* into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
flitdb_extern int flitdb_insert_const_char(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, const char* value);

#ifdef __cplusplus

/**
 * @brief Insert an boolean (bool) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, bool value);

#endif

/**
 * @brief Insert an boolean (bool) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
flitdb_extern int flitdb_insert_bool(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position, bool value);

/**
 * @brief Delete a value stored within the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to delete the value from
 * @param row_position The row that you wish to delete the value from
 * @return int 
 */
flitdb_extern int flitdb_delete(flitdb **handler, flitdb_column_row_sizing column_position, flitdb_column_row_sizing row_position);

/**
 * @brief Retrieve an integer (int) from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return int 
 */
flitdb_extern int flitdb_retrieve_int(flitdb **handler);

/**
 * @brief Retrieve an float from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return float 
 */
flitdb_extern float flitdb_retrieve_float(flitdb **handler);

/**
 * @brief Retrieve an char* from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return char * 
 */
flitdb_extern char* flitdb_retrieve_char(flitdb **handler);

/**
 * @brief Retrieve an boolean (bool) from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return bool 
 */
flitdb_extern bool flitdb_retrieve_bool(flitdb **handler);

#undef flitdb_extern

#endif