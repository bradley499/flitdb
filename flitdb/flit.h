#ifndef flit_h
#define flit_h
#include <stdbool.h>

#define FLITDB_SUCCESS      0  // Successful operation
#define FLITDB_ERROR        1  // Unsuccessful operation
#define FLITDB_PERM         2  // Permission denied
#define FLITDB_BUSY         3  // The database file is locked
#define FLITDB_NOT_FOUND    4  // The database file is not found
#define FLITDB_CORRUPT      5  // The database file is malformed
#define FLITDB_RANGE        6  // The requested range is outside the range of the database
#define FLITDB_CREATE       7  // Create a database if not existent
#define FLITDB_READONLY     8  // Only allow the reading of the database
#define FLITDB_DONE         9  // The operation was completed successfully
#define FLITDB_NULL         10 // The operation resulted in a null lookup
#define FLITDB_INTEGER      11 // The value type of integer
#define FLITDB_DOUBLE       12 // The value type of double
#define FLITDB_FLOAT        13 // The value type of float
#define FLITDB_CHAR         14 // The value type of char
#define FLITDB_BOOL         15 // The value type of bool
#define FLITDB_UNSAFE       16 // Discard all safety protocols to allow for larger database
#define FLITDB_VERSION  0x52d9 // The current FlitDB version magic number

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef flit_lib_compile
#ifdef __cplusplus
#define flitdb_extern extern "C" __declspec(dllexport)
#else
#define flitdb_extern extern __declspec(dllexport)
#endif
#else
#ifdef FLITDB_WIN_LIB
#ifdef __cplusplus
#define flitdb_extern extern "C" __declspec(dllimport)
#else
#define flitdb_extern extern __declspec(dllimport)
#endif
#endif
#endif
#endif
#ifndef flitdb_extern
#ifdef __cplusplus
#define flitdb_extern extern "C"
#else
#define flitdb_extern extern
#endif
#endif

#ifndef flitdb_object_configured
typedef struct flitdb flitdb;
#endif

/**
 * @brief Configures the FlitDB handler to point to and operate on a specific file
 * 
 * @param filename The path for the file to operate on
 * @param handler The FlitDB handler The FlitDB handler
 * @param flags The read write operation flags to set on the database file
 * @return int 
 */
flitdb_extern int flitdb_setup(const char *filename, flitdb **handler, int flags);

/**
 * @brief Close the connection to the database, and delete the FlitDB handler object
 * 
 * @param handler The FlitDB handler The FlitDB handler
 * @return int 
 */
flitdb_extern int flitdb_close(flitdb **handler);

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
flitdb_extern int flitdb_extract(flitdb **handler, unsigned long long int column_position, unsigned short row_position);

/**
 * @brief Retrieve a numeric representation of what data type was retrieved:
 * 		  FLITDB_NULL,
 * 		  FLITDB_INTEGER,
 * 		  FLITDB_DOUBLE,
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
 * @brief Insert an integer (signed long long int) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, signed long long int value);

#endif

/**
 * @brief Insert an integer (signed long long int) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */

flitdb_extern int flitdb_insert_int(flitdb **handler, unsigned long long int column_position, unsigned short row_position, signed long long int value);

#ifdef __cplusplus

/**
 * @brief Insert an double (long double) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, long double value);

#endif

/**
 * @brief Insert an double (long double) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
flitdb_extern int flitdb_insert_double(flitdb **handler, unsigned long long int column_position, unsigned short row_position, long double value);

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
int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, float value);

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
flitdb_extern int flitdb_insert_float(flitdb **handler, unsigned long long int column_position, unsigned short row_position, float value);

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
int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, char* value);

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
flitdb_extern int flitdb_insert_char(flitdb **handler, unsigned long long int column_position, unsigned short row_position, char* value);

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
int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, const char* value);

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
flitdb_extern int flitdb_insert_const_char(flitdb **handler, unsigned long long int column_position, unsigned short row_position, const char* value);

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
int flitdb_insert(flitdb **handler, unsigned long long int column_position, unsigned short row_position, bool value);

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
flitdb_extern int flitdb_insert_bool(flitdb **handler, unsigned long long int column_position, unsigned short row_position, bool value);

/**
 * @brief Delete a value stored within the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to delete the value from
 * @param row_position The row that you wish to delete the value from
 * @return int 
 */
flitdb_extern int flitdb_delete(flitdb **handler, unsigned long long int column_position, unsigned short row_position);

/**
 * @brief Retrieve an integer (signed long long int) from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return signed long long int 
 */
flitdb_extern signed long long int flitdb_retrieve_int(flitdb **handler);

/**
 * @brief Retrieve an double (long double) from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return long double 
 */
flitdb_extern long double flitdb_retrieve_double(flitdb **handler);

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