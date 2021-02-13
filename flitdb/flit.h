#pragma once
#ifndef flit_h
#define flit_h

typedef unsigned short uint16_t;
typedef signed long int64_t;

#ifndef flitdb_class_configured
typedef class flitdb;
#endif

#define FLITDB_SUCCESS 		0  // Successful operation
#define FLITDB_ERROR 		1  // Unsuccessful operation
#define FLITDB_PERM 		2  // Permission denied
#define FLITDB_BUSY 		3  // The database file is locked
#define FLITDB_NOT_FOUND 	4  // The database file is not found
#define FLITDB_CORRUPT 		5  // The database file is malformed
#define FLITDB_RANGE 		6  // The requested range is outside the range of the database
#define FLITDB_DONE 		7  // The operation was completed successfully
#define FLITDB_NULL			8  // The operation resulted in a null lookup
#define FLITDB_CREATE		9  // Create a database if not existent
#define FLITDB_READONLY		10 // Only allow the reading of the database

int flitdb_setup(const char *filename, flitdb *handler, int flags);
char* flitdb_errmsg(flitdb *handler);
int flitdb_extract(flitdb *handler, int64_t column_position, int64_t row_position);
int flitdb_type(flitdb *handler, int64_t column_position, int64_t row_position);
int flitdb_insert(flitdb *handler, int64_t column_position, int64_t row_position, signed long long int value);
int flitdb_insert(flitdb *handler, int64_t column_position, int64_t row_position, long double value);
int flitdb_insert(flitdb *handler, int64_t column_position, int64_t row_position, float value);
int flitdb_insert(flitdb *handler, int64_t column_position, int64_t row_position, char* value);
int flitdb_insert(flitdb *handler, int64_t column_position, int64_t row_position, const char* value);
int flitdb_insert(flitdb *handler, int64_t column_position, int64_t row_position, bool value);
int flitdb_delete(flitdb *handler, int64_t column_position, int64_t row_position);
signed long long int flitdb_retrieve_int(flitdb *handler);
double flitdb_retrieve_double(flitdb *handler);
float flitdb_retrieve_float(flitdb *handler);
char* flitdb_retrieve_char(flitdb *handler);
bool flitdb_retrieve_bool(flitdb *handler);
#endif