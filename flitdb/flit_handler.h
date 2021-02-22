#ifndef flit_handler_h
#define flit_handler_h

#include <stdbool.h>

#define flitdb_object_configured

typedef struct flitdb flitdb;

const unsigned long long flitdb_max_size();

bool flitdb_new(flitdb **handler);
void flitdb_destroy(flitdb **handler);
int flitdb_connection_setup(flitdb **handler, const char *filename, int flags);
char *flitdb_get_err_message(flitdb **handler);
int flitdb_read_at(flitdb **handler, unsigned short column_position, unsigned short row_position);
void flitdb_clear_values(flitdb **handler);
int flitdb_insert_at(flitdb **handler, unsigned short column_position, unsigned short row_position);
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

#endif