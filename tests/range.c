#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "flit.h"

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_open("./test_exceed.db", &flit, FLITDB_CREATE) == FLITDB_SUCCESS);
	assert(flitdb_insert_int(&flit, 17, 1, 1) == FLITDB_RANGE);
	assert(flitdb_insert_int(&flit, 1, 17, 1) == FLITDB_RANGE);
	assert(flitdb_insert_int(&flit, 1, 0, 1) == FLITDB_RANGE);
	assert(flitdb_insert_int(&flit, 0, 1, 1) == FLITDB_RANGE);
	flitdb_close(&flit);
	return 0;
}