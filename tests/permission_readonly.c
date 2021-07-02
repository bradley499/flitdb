#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "flit.h"

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_setup("./test_readonly.db", &flit, FLITDB_READONLY) == FLITDB_NOT_FOUND);
	flitdb_close(&flit);
	FILE *readonly_db;
	readonly_db = fopen("./test_readonly.db", "w"); // create empty database
	fclose(readonly_db);
	assert(flitdb_setup("./test_readonly.db", &flit, FLITDB_READONLY) == FLITDB_SUCCESS);
	assert(flitdb_insert_int(&flit, 1, 1, 12345) == FLITDB_READONLY);
	flitdb_close(&flit);
	return 0;
}