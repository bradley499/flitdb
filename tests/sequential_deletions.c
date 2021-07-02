#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "flit.h"

#define MAX 70
#define MAX_INSERTIONS (MAX * MAX)

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_setup("./test2.db", &flit, FLITDB_CREATE) == FLITDB_SUCCESS);
	for (unsigned short x = 0; x < MAX; x++)
	{
		for (unsigned short y = 0; y < MAX; y++)
		{
			assert(flitdb_delete(&flit, (x + 1), (y + 1)) == FLITDB_DONE);
		}
	}
	flitdb_close(&flit);
	return 0;
}