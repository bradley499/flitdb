#pragma once
#ifndef flit_handler_h
#define flit_handler_h

#define flitdb_class_configured

class flitdb
{
private:
	const static unsigned short max_buffer_size = 1024;
	const static unsigned int column_position_max = 10000;
	const static unsigned int row_position_max = 1000;
	char buffer[max_buffer_size];
	bool configured = false;
	int file_descriptor;
	char* err_message;
	struct value
	{
		signed long long int int_value;
		long double double_value;
		float float_value;
		char char_value[10000];
		bool bool_value;
	} value;
	unsigned char value_type;
	size_t size;
	bool read_only;
	void clear_values();
	constexpr unsigned long long max_size();
	unsigned short to_short(char* chars);
	signed long long to_long_long(char* chars);
	double to_double(char* chars);
	float to_float(char* chars);
	char* to_char_array(double number);
public:
	flitdb();
	~flitdb();
	int setup(const char *filename, int flags);
	char* get_err_message();
	int read_at(unsigned short column_position, unsigned short row_position);
	int insert_at(unsigned short column_position, unsigned short row_position);
	int insert_value(signed long long set_value);
	int insert_value(long double set_value);
	int insert_value(float set_value);
	int insert_value(char* set_value);
	int insert_value(bool set_value);
	signed long long int retrieve_value_int();
	double retrieve_value_double();
	float retrieve_value_float();
	char* retrieve_value_char();
	bool retrieve_value_bool();
};

#include "flit.h"


#endif