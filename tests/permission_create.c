#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "flit.h"

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_open("./test_create.db", &flit, FLITDB_READONLY) == FLITDB_NOT_FOUND);
	flitdb_close(&flit);
	assert(flitdb_open("./test_create.db", &flit, FLITDB_CREATE) == FLITDB_SUCCESS);
	flitdb_close(&flit);
	return 0;
}