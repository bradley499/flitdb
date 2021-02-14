#ifndef flit_h
#define flit_h

typedef unsigned long uint64_t;

#ifndef flitdb_class_configured
typedef class flitdb;
#endif

#define FLITDB_SUCCESS		0  // Successful operation
#define FLITDB_ERROR		1  // Unsuccessful operation
#define FLITDB_PERM			2  // Permission denied
#define FLITDB_BUSY			3  // The database file is locked
#define FLITDB_NOT_FOUND	4  // The database file is not found
#define FLITDB_CORRUPT		5  // The database file is malformed
#define FLITDB_RANGE		6  // The requested range is outside the range of the database
#define FLITDB_DONE			7  // The operation was completed successfully
#define FLITDB_NULL			8  // The operation resulted in a null lookup
#define FLITDB_CREATE		9  // Create a database if not existent
#define FLITDB_READONLY		10 // Only allow the reading of the database

/**
 * @brief Configures the FlitDB handler to point to and operate on a specific file
 * 
 * @param filename The path for the file to operate on
 * @param handler The FlitDB handler The FlitDB handler
 * @param flags The read write operation flags to set on the database file
 * @return int 
 */
int flitdb_setup(const char *filename, flitdb *&handler, int flags);

/**
 * @brief Close the connection to the database, and delete the FlitDB handler object
 * 
 * @param handler The FlitDB handler The FlitDB handler
 * @return int 
 */
int flitdb_close(flitdb *&handler);

/**
 * @brief Retrieve an error message from the FlitDB handler if once exists
 * 
 * @param handler The FlitDB handler 
 * @return char* 
 */
char* flitdb_errmsg(flitdb *handler);

/**
 * @brief Extract a value stored within the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to extract the value from
 * @param row_position The row that you wish to extract the value from
 * @return int 
 */
int flitdb_extract(flitdb *handler, uint64_t column_position, uint64_t row_position);

/**
 * @brief Retrieve a numeric representation of what data type was retrieved:
 * 		  0 = Null,
 * 		  1 = Integer,
 * 		  2 = Double,
 * 		  3 = Float,
 * 		  4 = Char *,
 * 		  5 = Boolean
 * 
 * @param handler 
 * @return int 
 */
int flitdb_retrieved_type(flitdb *handler);

/**
 * @brief Insert an integer (signed long long int) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, signed long long int value);

/**
 * @brief Insert an double (long double) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, long double value);

/**
 * @brief Insert an float into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, float value);

/**
 * @brief Insert an char* into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, char* value);

/**
 * @brief Insert an const char* into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, const char* value);

/**
 * @brief Insert an boolean (bool) into the FlitDB handler and store
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to insert the value into
 * @param row_position The row that you wish to insert the value into
 * @param value The value to insert
 * @return int 
 */
int flitdb_insert(flitdb *handler, uint64_t column_position, uint64_t row_position, bool value);

/**
 * @brief Delete a value stored within the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @param column_position The column that you wish to delete the value from
 * @param row_position The row that you wish to delete the value from
 * @return int 
 */
int flitdb_delete(flitdb *handler, uint64_t column_position, uint64_t row_position);

/**
 * @brief Retrieve an integer (signed long long int) from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return signed long long int 
 */
signed long long int flitdb_retrieve_int(flitdb *handler);

/**
 * @brief Retrieve an double (long double) from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return long double 
 */
long double flitdb_retrieve_double(flitdb *handler);

/**
 * @brief Retrieve an float from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return float 
 */
float flitdb_retrieve_float(flitdb *handler);

/**
 * @brief Retrieve an char* from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return char * 
 */
char* flitdb_retrieve_char(flitdb *handler);

/**
 * @brief Retrieve an boolean (bool) from the extracted value from the FlitDB handler
 * 
 * @param handler The FlitDB handler 
 * @return bool 
 */
bool flitdb_retrieve_bool(flitdb *handler);
#endif