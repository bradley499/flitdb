#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "flit.h"

#define MAX 99
#define MAX_VALUE 99999

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_open("./test.db", &flit, FLITDB_CREATE) == FLITDB_SUCCESS);
	typedef struct value
	{
		unsigned short column;
		unsigned short row;
		unsigned char type;
		union result
		{
			int integer_value;
			float float_value;
			bool bool_value;
		} result;
	} value;
	value checkpoints[60];
	unsigned short checkpoints_used[2] = {0, 0};
	for (unsigned char repeat = 0; repeat < 2; repeat++)
	{
		unsigned char data_type = (rand() % 5);
		unsigned short checkpoint_counter = 0;
		for (unsigned int x = 0; x < MAX; x++)
		{
			for (unsigned int y = 0; y < MAX; y++)
			{
				checkpoint_counter++;
				data_type += (rand() % 4);
				data_type %= 5;
				union value
				{
					int integer_value;
					float float_value;
					bool bool_value;
				} value;
				switch (data_type)
				{
				case 0:
					value.integer_value = (rand() % MAX_VALUE);
					assert(flitdb_insert_int(&flit, (x + 1), (y + 1), value.integer_value) == FLITDB_DONE);
					break;
				case 1:
					value.float_value = ((float)rand() / (float)(rand() / MAX_VALUE));
					assert(flitdb_insert_float(&flit, (x + 1), (y + 1), value.float_value) == FLITDB_DONE);
					break;
				case 2:
					assert(flitdb_insert_char(&flit, (x + 1), (y + 1), "test") == FLITDB_DONE);
					break;
				case 3:
					value.bool_value = (rand() % 2);
					assert(flitdb_insert_bool(&flit, (x + 1), (y + 1), value.bool_value) == FLITDB_DONE);
					break;
				case 4:
					assert(flitdb_delete(&flit, (x + 1), (y + 1)) == FLITDB_DONE);
					break;
				default:
					break;
				}
				if (checkpoint_counter == 163)
				{
					checkpoint_counter = 0;
					checkpoints[checkpoints_used[repeat]].column = (x + 1);
					checkpoints[checkpoints_used[repeat]].row = (y + 1);
					checkpoints[checkpoints_used[repeat]].type = data_type;
					switch (data_type)
					{
					case 0:
						checkpoints[checkpoints_used[repeat]].result.integer_value = value.integer_value;
						break;
					case 1:
						checkpoints[checkpoints_used[repeat]].result.float_value = value.float_value;
						break;
					case 3:
						checkpoints[checkpoints_used[repeat]].result.bool_value = value.bool_value;
						break;
					default:
						break;
					}
					checkpoints_used[repeat]++;
					for (unsigned char i = 0; i < checkpoints_used[0]; i++)
					{
						unsigned int response = flitdb_extract(&flit, checkpoints[i].column, checkpoints[i].row);
						switch (checkpoints[i].type)
						{
						case 0:
						{
							assert(flitdb_retrieve_int(&flit) == checkpoints[i].result.integer_value);
							break;
						}
						case 1:
						{
							assert(flitdb_retrieve_float(&flit) == checkpoints[i].result.float_value);
							break;
						}
						case 2:
						{
							const char *char_value = flitdb_retrieve_char(&flit);
							assert(strcmp(char_value, "test") == 0);
							break;
						}
						case 3:
						{
							assert(flitdb_retrieve_bool(&flit) == checkpoints[i].result.bool_value);
							break;
						}
						case 4:
						{
							assert(response == FLITDB_NULL);
							break;
						}
						}
					}
				}
			}
		}
	}
	checkpoints[0].result.integer_value = 12345;
	checkpoints[1].result.integer_value = 54321;
	assert(flitdb_insert_int(&flit, 1, 100, checkpoints[0].result.integer_value) == FLITDB_DONE);
	assert(flitdb_insert_int(&flit, 2, 2, checkpoints[1].result.integer_value) == FLITDB_DONE);
	checkpoints[0].column = 1;
	checkpoints[0].row = 100;
	checkpoints[0].type = 0;
	checkpoints[1].column = 2;
	checkpoints[1].row = 2;
	checkpoints[1].type = 0;
	int int_validation = 12345;
	float float_validation = 123.456;
	char *char_validation = "test";
	bool bool_validation = true;
	for (unsigned char i = 0; i < 5; i++)
	{
		for (unsigned char ii = 0; ii < 5; ii++)
		{
			switch (i)
			{
			case 0:
			{
				assert(flitdb_insert_int(&flit, 2, 1, int_validation) == FLITDB_DONE);
				assert(flitdb_extract(&flit, 2, 1) == FLITDB_DONE);
				assert(flitdb_retrieve_int(&flit) == int_validation);
				break;
			}
			case 1:
			{
				assert(flitdb_insert_float(&flit, 2, 1, float_validation) == FLITDB_DONE);
				assert(flitdb_extract(&flit, 2, 1) == FLITDB_DONE);
				assert(flitdb_retrieve_float(&flit) == float_validation);
				break;
			}
			case 2:
			{
				assert(flitdb_insert_char(&flit, 2, 1, char_validation) == FLITDB_DONE);
				assert(flitdb_extract(&flit, 2, 1) == FLITDB_DONE);
				assert(strcmp(flitdb_retrieve_char(&flit), char_validation) == 0);
				break;
			}
			case 3:
			{
				assert(flitdb_insert_bool(&flit, 2, 1, bool_validation) == FLITDB_DONE);
				assert(flitdb_extract(&flit, 2, 1) == FLITDB_DONE);
				assert(flitdb_retrieve_bool(&flit) == bool_validation);
				break;
			}
			case 4:
			{
				assert(flitdb_delete(&flit, 2, 1) == FLITDB_DONE);
				assert(flitdb_extract(&flit, 2, 1) == FLITDB_NULL);
				break;
			}
			}
			switch (ii)
			{
			case 0:
			{
				assert(flitdb_insert_int(&flit, 2, 1, int_validation) == FLITDB_DONE);
				assert(flitdb_extract(&flit, 2, 1) == FLITDB_DONE);
				assert(flitdb_retrieve_int(&flit) == int_validation);
				break;
			}
			case 1:
			{
				assert(flitdb_insert_float(&flit, 2, 1, float_validation) == FLITDB_DONE);
				assert(flitdb_extract(&flit, 2, 1) == FLITDB_DONE);
				assert(flitdb_retrieve_float(&flit) == float_validation);
				break;
			}
			case 2:
			{
				assert(flitdb_insert_char(&flit, 2, 1, char_validation) == FLITDB_DONE);
				assert(flitdb_extract(&flit, 2, 1) == FLITDB_DONE);
				assert(strcmp(flitdb_retrieve_char(&flit), char_validation) == 0);
				break;
			}
			case 3:
			{
				assert(flitdb_insert_bool(&flit, 2, 1, bool_validation) == FLITDB_DONE);
				assert(flitdb_extract(&flit, 2, 1) == FLITDB_DONE);
				assert(flitdb_retrieve_bool(&flit) == bool_validation);
				break;
			}
			case 4:
			{
				assert(flitdb_delete(&flit, 2, 1) == FLITDB_DONE);
				assert(flitdb_extract(&flit, 2, 1) == FLITDB_NULL);
				break;
			}
			}
			assert(flitdb_extract(&flit, checkpoints[0].column, checkpoints[0].row) == FLITDB_DONE);
			assert(flitdb_retrieve_int(&flit) == checkpoints[0].result.integer_value);
			assert(flitdb_extract(&flit, checkpoints[1].column, checkpoints[1].row) == FLITDB_DONE);
			assert(flitdb_retrieve_int(&flit) == checkpoints[1].result.integer_value);
		}
	}
	flitdb_close(&flit);
	return 0;
}