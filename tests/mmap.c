#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "flit.h"

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_open("./test_mmap.db", &flit, FLITDB_CREATE) == FLITDB_SUCCESS);
	for (unsigned char i = 1; i <= 0x10; i++)
		for (unsigned char ii = 1; ii <= 0x10; ii++)
			assert(flitdb_insert_int(&flit, i, ii, 12345) == FLITDB_DONE);
	flitdb_close(&flit);
	assert(flitdb_open("./test_mmap.db", &flit, FLITDB_READONLY) == FLITDB_SUCCESS);
	for (unsigned char i = 1; i <= 0x10; i++)
		for (unsigned char ii = 1; ii <= 0x10; ii++)
		{
			assert(flitdb_extract(&flit, i, ii) == FLITDB_DONE);
			assert(flitdb_retrieve_int(&flit) == 12345);
		}
	flitdb_close(&flit);
	return 0;
}