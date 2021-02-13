#pragma once
#ifndef flit_handler_cpp
#define flit_handler_cpp

#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "flit_handler.h"
#include "misc.cpp"

flitdb::flitdb()
{
	configured = (max_buffer_size < 50 || max_buffer_size > 1024);
	value_type = 0;
	size = 0;
	read_only = false;
	if (max_buffer_size < 50)
		err_message = (char*)"The minimum buffer size has encroached beyond suitable definitions\0";
	else if (max_buffer_size > 1024)
		err_message = (char*)"The maximum buffer size has encroached beyond suitable definitions\0";
	else
		err_message = (char*)"";
	clear_values();
}

flitdb::~flitdb()
{
	flock(file_descriptor, LOCK_UN);
	close(file_descriptor);
}

constexpr unsigned long long flitdb::max_size()
{
	unsigned long long insertion_area[2] = {(column_position_max * row_position_max), (row_position_max > 1 ? ((column_position_max * (row_position_max - 1)) * 8) : 0)};
	return ((insertion_area[0] * 10000) + insertion_area[1] + (column_position_max * 15));
}

unsigned short flitdb::to_short(char* chars)
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

signed long long flitdb::to_long_long(char* chars)
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
	return (((negative) ? -1 : 1 ) * long_long_value);
}

double flitdb::to_double(char* chars)
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

float flitdb::to_float(char* chars)
{
	return (float)to_double(chars);
}

void flitdb::clear_values()
{
	value.int_value = 0;
	value.double_value = 0;
	value.float_value = 0;
	value.bool_value = false;
	strncpy(value.char_value, "\0", sizeof(value.char_value));
	value_type = 0;
	value_retrieved = false;
	strncpy(buffer, "\0", sizeof(buffer));
}

int flitdb::setup(const char *filename, int flags)
{
	if (configured)
	{
		if (max_buffer_size >= 50 && max_buffer_size <= 1024)
			err_message = (char *)"The database handler has already been attributed to handle another database\0";
		return FLITDB_ERROR;
	}
	struct stat buffer;
	if (stat(filename, &buffer) != 0)
	{
		if ((flags & FLITDB_CREATE) != FLITDB_CREATE)
			return FLITDB_NOT_FOUND;
	}
	if ((flags & FLITDB_READONLY) == FLITDB_READONLY)
		read_only = true;
	file_descriptor = open(filename, (((flags & FLITDB_READONLY) == FLITDB_READONLY) ? (((flags & FLITDB_CREATE) == FLITDB_CREATE) ? (O_CREAT | O_RDONLY) : O_RDONLY) : (((flags & FLITDB_CREATE) == FLITDB_CREATE) ? (O_CREAT | O_RDWR): O_RDWR)), 0666);
	if (file_descriptor == -1)
	{
		err_message = (char *)"Failed to open the database\0";
		return FLITDB_PERM;
	}
	if (flock(file_descriptor, LOCK_EX | LOCK_NB) != 0)
	{
		err_message = (char *)"Exclusive rights to access the database could not be obtained\0";
		return FLITDB_BUSY;
	}
	struct stat stat_buf;
	int rc = fstat(file_descriptor, &stat_buf);
	size = ((rc == 0) ? stat_buf.st_size : 0);
	if (size > max_size())
	{
		err_message = (char *)"The database attempted to access has a larger size than what this object can read\0";
		return FLITDB_RANGE;
	}
	configured = true;
	return FLITDB_SUCCESS;
}

char* flitdb::get_err_message()
{
	return err_message;
}

int flitdb::insert_value(signed long long int set_value)
{
	if (read_only)
	{
		err_message = (char *)"The database was opened in readonly mode";
		return FLITDB_READONLY;	
	}
	if (value_type != 0 && !value_retrieved)
	{
		err_message = (char *)"Data insertion avoided due to unexpected tennant";
		return FLITDB_ERROR;
	}
	clear_values();
	value_type = 1;
	value.int_value = set_value;
	return FLITDB_DONE;
}

int flitdb::insert_value(long double set_value)
{
	if (read_only)
	{
		err_message = (char *)"The database was opened in readonly mode";
		return FLITDB_READONLY;	
	}
	if (value_type != 0 && !value_retrieved)
	{
		err_message = (char *)"Data insertion avoided due to unexpected tennant";
		return FLITDB_ERROR;
	}
	clear_values();
	value_type = 2;
	value.double_value = set_value;
	return FLITDB_DONE;
}

int flitdb::insert_value(float set_value)
{
	if (read_only)
	{
		err_message = (char *)"The database was opened in readonly mode";
		return FLITDB_READONLY;	
	}
	if (value_type != 0 && !value_retrieved)
	{
		err_message = (char *)"Data insertion avoided due to unexpected tennant";
		return FLITDB_ERROR;
	}
	clear_values();
	value_type = 3;
	value.float_value = set_value;
	return FLITDB_DONE;
}

int flitdb::insert_value(char *set_value)
{
	if (read_only)
	{
		err_message = (char *)"The database was opened in readonly mode";
		return FLITDB_READONLY;	
	}
	if (value_type != 0 && !value_retrieved)
	{
		err_message = (char *)"Data insertion avoided due to unexpected tennant";
		return FLITDB_ERROR;
	}
	if (strlen(set_value) >= 10000)
	{
		err_message = (char *)"Data insertion avoided due to the length of a string being too large";
		return FLITDB_ERROR;
	}
	clear_values();
	value_type = 4;
	strncpy(value.char_value, set_value, sizeof(value.char_value));
	return FLITDB_DONE;
}

int flitdb::insert_value(bool set_value)
{
	if (read_only)
	{
		err_message = (char *)"The database was opened in readonly mode";
		return FLITDB_READONLY;	
	}
	if (value_type != 0 && !value_retrieved)
	{
		err_message = (char *)"Data insertion avoided due to unexpected tennant";
		return FLITDB_ERROR;
	}
	clear_values();
	value_type = 5;
	value.bool_value = set_value;
	return FLITDB_DONE;
}

int flitdb::insert_reset()
{
	clear_values();
	return FLITDB_DONE;
}

signed long long flitdb::retrieve_value_int()
{
	return value.int_value;
}

long double flitdb::retrieve_value_double()
{
	return value.double_value;
}

float flitdb::retrieve_value_float()
{
	return value.float_value;
}

char *flitdb::retrieve_value_char()
{
	return value.char_value;
}

bool flitdb::retrieve_value_bool()
{
	return value.bool_value;
}

int flitdb::retrieve_value_type()
{
	return value_type;
}

int flitdb::read_at(unsigned short column_position, unsigned short row_position)
{
	clear_values();
	if (column_position == 0 || column_position > column_position_max || row_position == 0 || row_position > row_position_max)
	{
		err_message = (char *)"The requested range was outside of the database's range";
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
		strncpy(buffer, "\0", sizeof(buffer));
		ssize_t read_status = pread64(file_descriptor, &buffer, read_length, offset);
		offset += read_length;
		if (read_status == -1)
		{
			err_message = (char *)"An error occurred in attempting to read data from the database\0";
			return FLITDB_ERROR;
		}
		else if (read_status < read_length && read_status > 0)
		{
			err_message = (char *)"The database contracted a malformed structure declaration\0";
			return FLITDB_CORRUPT;
		}
		else if (read_status == 0)
			break;
		if (read_length == 15)
		{
			char skip_amount_read[5];
			skip_amount_read[4] = '\0';
			skip_amount_read[0] = buffer[0];
			skip_amount_read[1] = buffer[1];
			skip_amount_read[2] = buffer[2];
			skip_amount_read[3] = buffer[3];
			if (to_short(skip_amount_read) < 0)
			{
				err_message = (char *)"Skip offset negation detected\0";
				return FLITDB_ERROR;
			}
			skip_offset += (to_short(skip_amount_read) + 1);
			if (skip_offset > column_position)
				return FLITDB_NULL;
			char row_count_read[4];
			row_count_read[3] = '\0';
			row_count_read[0] = buffer[4];
			row_count_read[1] = buffer[5];
			row_count_read[2] = buffer[6];
			row_count = to_short(row_count_read) + 1;
			row_position_count = 0;
		}
		unsigned char set_read_length = 15;
		if (skip_offset == column_position)
		{
			char position_read[4];
			position_read[3] = '\0';
			position_read[0] = buffer[(read_length < 15) ? 0 : 7];
			position_read[1] = buffer[(read_length < 15) ? 1 : 8];
			position_read[2] = buffer[(read_length < 15) ? 2 : 9];
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
		response_length_read[0] = buffer[(read_length < 15) ? 3 : 10];
		response_length_read[1] = buffer[(read_length < 15) ? 4 : 11];
		response_length_read[2] = buffer[(read_length < 15) ? 5 : 12];
		response_length_read[3] = buffer[(read_length < 15) ? 6 : 13];
		unsigned short response_length = to_short(response_length_read) + 1;
		unsigned char data_type;
		switch (buffer[(read_length < 15) ? 7 : 14])
		{
		case '1':
			data_type = 1;
			break;
		case '2':
			data_type = 2;
			break;
		case '3':
			data_type = 3;
			break;
		case '4':
			data_type = 4;
			break;
		case '5':
			data_type = 5;
			break;
		default:
			err_message = (char *)"The database yielded an invalid datatype\0";
			return FLITDB_CORRUPT;
		}
		if (response_length_read == 0)
		{
			err_message = (char *)"A reference to an imposed data declaration holds no length\0";
			return FLITDB_CORRUPT;
		}
		else if (data_type == 5 && response_length != 1)
		{
			err_message = (char *)"The database holds a boolean of a possible elongated length\0";
			return FLITDB_CORRUPT;
		}
		if (store_response)
		{
			value_type = data_type;
			value_retrieved = true;
			char *response_value = new char[(response_length + 1)];
			response_value[response_length] = '\0';
			read_status = pread64(file_descriptor, response_value, response_length, offset);
			if (read_status == -1)
			{
				err_message = (char *)"An error occurred in attempting to read data from the database\0";
				return FLITDB_ERROR;
			}
			switch (data_type)
			{
			case 1:
				value.int_value = to_long_long(response_value);
				break;
			case 2:
				value.double_value = to_double(response_value);
				break;
			case 3:
				value.float_value = to_float(response_value);
				break;
			case 4:
				strncpy(value.char_value, response_value, sizeof(value.char_value));
				break;
			case 5:
				value.bool_value = (response_value[0] == '1');
				break;
			default:
				break;
			}
			delete [] response_value;
			return FLITDB_DONE;
		}
		read_length = set_read_length;
		offset += response_length;
	}
	return FLITDB_NULL;
}

int flitdb::insert_at(unsigned short column_position, unsigned short row_position)
{
	if (read_only)
	{
		err_message = (char *)"The database was opened in readonly mode";
		clear_values();
		return FLITDB_READONLY;	
	}
	if (column_position == 0 || column_position > column_position_max || row_position == 0 || row_position > row_position_max)
	{
		err_message = (char *)"The requested range was outside of the database's range";
		clear_values();
		return FLITDB_RANGE;
	}
	row_position -= 1;
	char *input_buffer;
	unsigned short input_size = 0;
	switch (value_type)
	{
	case 1:
	{
		input_buffer = new char[64];
		int ret = snprintf(input_buffer, 64, "%lld", value.int_value);
		if (ret <= 0)
		{
			delete [] input_buffer;
			err_message = (char *)"Value encoding of type: int; has failed\0";
			clear_values();
			return FLITDB_RANGE;
		}
		input_size = strlen(input_buffer);
		break;
	}
	case 2:
	{
		input_buffer = new char[96];
		int ret = snprintf(input_buffer, 96, "%Lf", value.double_value);
		if (ret <= 0)
		{
			delete [] input_buffer;
			err_message = (char *)"Value encoding of type: double; has failed\0";
			clear_values();
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
	case 3:
	{
		input_buffer = new char[32];
		int ret = snprintf(input_buffer, 32, "%f", value.float_value);
		if (ret <= 0)
		{
			delete [] input_buffer;
			err_message = (char *)"Value encoding of type: float; has failed\0";
			clear_values();
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
	case 4:
	{
		input_size = strlen(value.char_value);
		input_buffer = new char[input_size + 1];
		input_buffer[input_size] = '\0';
		strncpy(input_buffer, value.char_value, input_size);
		break;
	}
	case 5:
	{
		input_buffer = new char[1];
		if (value.bool_value)
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
			delete [] input_buffer;
			err_message = (char *)"Length determination failed\0";
			clear_values();
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
	while (size > 0)
	{
		current_length = 0;
		strncpy(buffer, "\0", sizeof(buffer));
		ssize_t read_status = pread64(file_descriptor, &buffer, read_length[0], offset[0]);
		offset[0] += read_length[0];
		offset[1] = offset[0];
		if (read_status == -1)
		{
			delete [] input_buffer;
			err_message = (char *)"An error occurred in attempting to read data from the database\0";
			clear_values();
			return FLITDB_ERROR;
		}
		else if (read_status < read_length[0] && read_status > 0)
		{
			delete [] input_buffer;
			err_message = (char *)"The database contracted a malformed structure declaration\0";
			clear_values();
			return FLITDB_CORRUPT;
		}
		else if (read_status == 0)
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
		bool segment_detection = false;
		insertion_state[1] = insertion_state[0];
		if (read_length[0] == 15)
		{
			offset[3] = (offset[1] - read_length[0]);
			insertion_state[0] = 2;
			char skip_amount_read[5];
			skip_amount_read[4] = '\0';
			skip_amount_read[0] = buffer[0];
			skip_amount_read[1] = buffer[1];
			skip_amount_read[2] = buffer[2];
			skip_amount_read[3] = buffer[3];
			signed short skip_amount = to_short(skip_amount_read);
			if (skip_amount < 0)
			{
				delete [] input_buffer;
				err_message = (char *)"Skip offset negation detected\0";
				clear_values();
				return FLITDB_CORRUPT;
			}
			skip_offset[1] = skip_offset[0];
			skip_offset[0] += (skip_amount + 1);
			char row_count_read[4];
			row_count_read[3] = '\0';
			row_count_read[0] = buffer[4];
			row_count_read[1] = buffer[5];
			row_count_read[2] = buffer[6];
			row_count[0] = to_short(row_count_read);
			row_count[1] = (row_count[0] + 1);
			if (row_count[0] < 0)
			{
				delete [] input_buffer;
				err_message = (char *)"Row count negation detected\0";
				clear_values();
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
		position_read[0] = buffer[(read_length[0] < 15) ? 0 : 7];
		position_read[1] = buffer[(read_length[0] < 15) ? 1 : 8];
		position_read[2] = buffer[(read_length[0] < 15) ? 2 : 9];
		signed short position = to_short(position_read);
		if (position < 0)
		{
			delete [] input_buffer;
			err_message = (char *)"Position offset negation detected\0";
			clear_values();
			return FLITDB_CORRUPT;
		}
		char response_length_read[5];
		response_length_read[4] = '\0';
		response_length_read[0] = buffer[(read_length[0] < 15) ? 3 : 10];
		response_length_read[1] = buffer[(read_length[0] < 15) ? 4 : 11];
		response_length_read[2] = buffer[(read_length[0] < 15) ? 5 : 12];
		response_length_read[3] = buffer[(read_length[0] < 15) ? 6 : 13];
		current_length = to_short(response_length_read);
		if (current_length <= 0)
		{
			delete [] input_buffer;
			err_message = (char *)"Length notation negation detected\0";
			clear_values();
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
	if (offset[0] == offset[1])
	{
		offset[1] -= 1;
		if (input_size == 0)
		{
			delete [] input_buffer;
			clear_values();
			return FLITDB_DONE;
		}
		// New grouping
		if (size > 0 && offset[0] != size)
		{
			skip_offset[0] = skip_offset[1];
			unsigned short offset_sizing = input_size + 15;
			unsigned short buffer_size = max_buffer_size;
			unsigned short buffer_offset = 0;
			bool writable = true;
			while (writable)
			{
				if ((buffer_offset + buffer_size) > size)
				{
					buffer_size = (size - buffer_offset - offset[0]);
					writable = false;
				}
				buffer_offset += buffer_size;
				unsigned long reference_position = (size - buffer_offset);
				strncpy(buffer, "\0", sizeof(buffer));
				pread64(file_descriptor, buffer, buffer_size, reference_position);
				pwrite64(file_descriptor, buffer, buffer_size, (reference_position + offset_sizing));
			}
		}
		unsigned short skip_amount = (column_position - skip_offset[0] - 1);
		char skip_offset_buffer[5];
		strncpy(skip_offset_buffer, "\0", 5);
		snprintf(skip_offset_buffer, sizeof(skip_offset_buffer), "%d", skip_amount);
		if (skip_amount < 10)
		{
			skip_offset_buffer[3] = skip_offset_buffer[0];
			skip_offset_buffer[2] = '0';
			skip_offset_buffer[1] = '0';
			skip_offset_buffer[0] = '0';
		}
		else if (skip_amount < 100)
		{
			skip_offset_buffer[3] = skip_offset_buffer[1];
			skip_offset_buffer[2] = skip_offset_buffer[0];
			skip_offset_buffer[1] = '0';
			skip_offset_buffer[0] = '0';
		}
		else if (skip_amount < 1000)
		{
			skip_offset_buffer[3] = skip_offset_buffer[2];
			skip_offset_buffer[2] = skip_offset_buffer[1];
			skip_offset_buffer[1] = skip_offset_buffer[0];
			skip_offset_buffer[0] = '0';
		}
		pwrite64(file_descriptor, skip_offset_buffer, 4, offset[0]);
		pwrite64(file_descriptor, "000", 3, offset[0] + 4);
		char position_buffer[4];
		strncpy(position_buffer, "\0", 4);
		snprintf(position_buffer, sizeof(position_buffer), "%d", row_position);
		if (row_position < 10)
		{
			position_buffer[2] = position_buffer[0];
			position_buffer[1] = '0';
			position_buffer[0] = '0';
		}
		else if (row_position < 100)
		{
			position_buffer[2] = position_buffer[1];
			position_buffer[1] = position_buffer[0];
			position_buffer[0] = '0';
		}
		pwrite64(file_descriptor, position_buffer, 3, offset[0] + 7);
		char input_length_buffer[5];
		strncpy(input_length_buffer, "\0", 5);
		input_size -= 1;
		snprintf(input_length_buffer, sizeof(input_length_buffer), "%d", input_size);
		if (input_size < 10)
		{
			input_length_buffer[3] = input_length_buffer[0];
			input_length_buffer[2] = '0';
			input_length_buffer[1] = '0';
			input_length_buffer[0] = '0';
		}
		else if (input_size < 100)
		{
			input_length_buffer[3] = input_length_buffer[1];
			input_length_buffer[2] = input_length_buffer[0];
			input_length_buffer[1] = '0';
			input_length_buffer[0] = '0';
		}
		else if (input_size < 1000)
		{
			input_length_buffer[3] = input_length_buffer[2];
			input_length_buffer[2] = input_length_buffer[1];
			input_length_buffer[1] = input_length_buffer[0];
			input_length_buffer[0] = '0';
		}
		input_size += 1;
		pwrite64(file_descriptor, input_length_buffer, 4, offset[0] + 10);
		switch (value_type)
		{
			case 1:
				pwrite64(file_descriptor, "1", 1, (offset[0] + 14));
				break;
			case 2:
				pwrite64(file_descriptor, "2", 1, (offset[0] + 14));
				break;
			case 3:
				pwrite64(file_descriptor, "3", 1, (offset[0] + 14));
				break;
			case 4:
				pwrite64(file_descriptor, "4", 1, (offset[0] + 14));
				break;
			case 5:
				pwrite64(file_descriptor, "5", 1, (offset[0] + 14));
				break;
		}
		pwrite64(file_descriptor, input_buffer, input_size, offset[0] + 15);
		unsigned long long incremental_size = input_size + 15;
		if (offset[0] < size)
			update_next = (offset[0] + incremental_size);
		if (size == 0)
			update_override_cancel = 2;
		size += incremental_size;
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
					char position_buffer[4];
					strncpy(position_buffer, "\0", 4);
					snprintf(position_buffer, sizeof(position_buffer), "%d", row_count[1]);
					if (row_count[1] < 10)
					{
						position_buffer[2] = position_buffer[0];
						position_buffer[1] = '0';
						position_buffer[0] = '0';
					}
					else if (row_count[1] < 100)
					{
						position_buffer[2] = position_buffer[1];
						position_buffer[1] = position_buffer[0];
						position_buffer[0] = '0';
					}
					pwrite64(file_descriptor, position_buffer, 3, (offset[1] + 4));
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
			unsigned short buffer_size = max_buffer_size;
			unsigned short buffer_offset = offset[0];
			bool writable = true;
			while (writable)
			{
				if ((buffer_offset + buffer_size) > size)
				{
					buffer_size = ((size - buffer_offset));
					writable = false;
				}
				strncpy(buffer, "\0", sizeof(buffer));
				pread64(file_descriptor, buffer, buffer_size, buffer_offset);
				pwrite64(file_descriptor, buffer, buffer_size, (buffer_offset - offset_sizing));
				buffer_offset += buffer_size;
			}
			if (!deleted_column[0] && !deleted_column[1])
			{
				char input_length_buffer[5];
				strncpy(input_length_buffer, "\0", 5);
				input_size -= 1;
				snprintf(input_length_buffer, sizeof(input_length_buffer), "%d", input_size);
				if (input_size < 10)
				{
					input_length_buffer[3] = input_length_buffer[0];
					input_length_buffer[2] = '0';
					input_length_buffer[1] = '0';
					input_length_buffer[0] = '0';
				}
				else if (input_size < 100)
				{
					input_length_buffer[3] = input_length_buffer[1];
					input_length_buffer[2] = input_length_buffer[0];
					input_length_buffer[1] = '0';
					input_length_buffer[0] = '0';
				}
				else if (input_size < 1000)
				{
					input_length_buffer[3] = input_length_buffer[2];
					input_length_buffer[2] = input_length_buffer[1];
					input_length_buffer[1] = input_length_buffer[0];
					input_length_buffer[0] = '0';
				}
				input_size += 1;
				pwrite64(file_descriptor, input_length_buffer, 4, offset[3] - 5);
				switch (value_type)
				{
					case 1:
						pwrite64(file_descriptor, "1", 1, (offset[3] - 1));
						break;
					case 2:
						pwrite64(file_descriptor, "2", 1, (offset[3] - 1));
						break;
					case 3:
						pwrite64(file_descriptor, "3", 1, (offset[3] - 1));
						break;
					case 4:
						pwrite64(file_descriptor, "4", 1, (offset[3] - 1));
						break;
					case 5:
						pwrite64(file_descriptor, "5", 1, (offset[3] - 1));
						break;
				}
				pwrite64(file_descriptor, input_buffer, input_size, offset[3]);
			}
			size -= offset_sizing;
			if (size == offset[1])
				update_override_cancel = 4;
			if (ftruncate(file_descriptor, size) != 0)
			{
				delete [] input_buffer;
				clear_values();
				err_message = (char *)"Failed database truncation occurred\0";
				return FLITDB_CORRUPT;
			}
		}
		else if (input_size > current_length)
		{
			// Insert more
			if (current_length > 0)
			{
				offset[0] -= 8;
			}
			else
			{
				offset[0] -=8;
				char row_buffer[4];
				strncpy(row_buffer, "\0", 4);
				snprintf(row_buffer, sizeof(row_buffer), "%d", row_count[1]);
				if (row_count[1] < 10)
				{
					row_buffer[2] = row_buffer[0];
					row_buffer[1] = '0';
					row_buffer[0] = '0';
				}
				else if (row_count[1] < 100)
				{
					row_buffer[2] = row_buffer[1];
					row_buffer[1] = row_buffer[0];
					row_buffer[0] = '0';
				}
				pwrite64(file_descriptor, row_buffer, 3, (offset[3] + 4));
			}
			unsigned short offset_sizing = ((current_length > 0) ? ((input_size - current_length)) : (input_size + 8));
			unsigned short buffer_size = max_buffer_size;
			unsigned short buffer_offset = 0;
			bool writable = true;
			while (writable)
			{
				if ((buffer_offset + buffer_size) > size)
				{
					buffer_size = (size - buffer_offset - offset[0]);
					writable = false;
				}
				buffer_offset += buffer_size;
				unsigned long reference_position = (size - buffer_offset);
				strncpy(buffer, "\0", sizeof(buffer));
				pread64(file_descriptor, buffer, buffer_size, reference_position);
				pwrite64(file_descriptor, buffer, buffer_size, (reference_position + offset_sizing));
			}
			if (current_length > 0)
			{
				size += (input_size - current_length);
				offset[0] -= 7;
			}
			else
				size += (input_size + 8);
			char position_buffer[4];
			strncpy(position_buffer, "\0", 4);
			snprintf(position_buffer, sizeof(position_buffer), "%d", row_position);
			if (row_position < 10)
			{
				position_buffer[2] = position_buffer[0];
				position_buffer[1] = '0';
				position_buffer[0] = '0';
			}
			else if (row_position < 100)
			{
				position_buffer[2] = position_buffer[1];
				position_buffer[1] = position_buffer[0];
				position_buffer[0] = '0';
			}
			pwrite64(file_descriptor, position_buffer, 3, offset[0]);
			char input_length_buffer[5];
			strncpy(input_length_buffer, "\0", 5);
			input_size -= 1;
			snprintf(input_length_buffer, sizeof(input_length_buffer), "%d", input_size);
			if (input_size < 10)
			{
				input_length_buffer[3] = input_length_buffer[0];
				input_length_buffer[2] = '0';
				input_length_buffer[1] = '0';
				input_length_buffer[0] = '0';
			}
			else if (input_size < 100)
			{
				input_length_buffer[3] = input_length_buffer[1];
				input_length_buffer[2] = input_length_buffer[0];
				input_length_buffer[1] = '0';
				input_length_buffer[0] = '0';
			}
			else if (input_size < 1000)
			{
				input_length_buffer[3] = input_length_buffer[2];
				input_length_buffer[2] = input_length_buffer[1];
				input_length_buffer[1] = input_length_buffer[0];
				input_length_buffer[0] = '0';
			}
			input_size += 1;
			pwrite64(file_descriptor, input_length_buffer, 4, (offset[0] + 3));
			switch (value_type)
			{
				case 1:
					pwrite64(file_descriptor, "1", 1, (offset[0] + 7));
					break;
				case 2:
					pwrite64(file_descriptor, "2", 1, (offset[0] + 7));
					break;
				case 3:
					pwrite64(file_descriptor, "3", 1, (offset[0] + 7));
					break;
				case 4:
					pwrite64(file_descriptor, "4", 1, (offset[0] + 7));
					break;
				case 5:
					pwrite64(file_descriptor, "5", 1, (offset[0] + 7));
					break;
			}
			pwrite64(file_descriptor, input_buffer, input_size, (offset[0] + 8));
		}
		else
		{
			// Update value (no size change)
			pwrite64(file_descriptor, input_buffer, input_size, offset[0]);
			switch (value_type)
			{
				case 1:
					pwrite64(file_descriptor, "1", 1, (offset[0] - 1));
					break;
				case 2:
					pwrite64(file_descriptor, "2", 1, (offset[0] - 1));
					break;
				case 3:
					pwrite64(file_descriptor, "3", 1, (offset[0] - 1));
					break;
				case 4:
					pwrite64(file_descriptor, "4", 1, (offset[0] - 1));
					break;
				case 5:
					pwrite64(file_descriptor, "5", 1, (offset[0] - 1));
					break;
			}
		}
	}
	if (value_type != 0)
		delete [] input_buffer;
	if ((update_next > 0 && (update_next < size || update_override_cancel == 0) && update_override_cancel != 2 && update_next != size) || update_override_cancel == 3)
	{
		strncpy(buffer, "\0", sizeof(buffer));
		pread64(file_descriptor, buffer, 4, update_next);
		unsigned short skip_amount_reducer = to_short(buffer);
		if (skip_amount_reducer == 0 && !deleted_column[0])
		{
			err_message = (char *)"Skip offset negation attempted on declared nullity\0";
			clear_values();
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
		pwrite64(file_descriptor, skip_offset_buffer, 4, update_next);
	}
	// Update skip amount for the next data group along after request.
	clear_values();
	return FLITDB_DONE;
}

#endif