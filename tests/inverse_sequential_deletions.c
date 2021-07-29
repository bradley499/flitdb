#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "flit.h"

#define MAX 99
#define MAX_INSERTIONS (MAX * MAX)
#define MAX_VALUE 99999

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_open("./test2.db", &flit, FLITDB_CREATE) == FLITDB_SUCCESS);
	for (unsigned short x = MAX; ; --x)
	{
		for (unsigned short y = MAX; ; --y)
		{
			assert(flitdb_delete(&flit, (x + 1), (y + 1)) == FLITDB_DONE);
			if (y == 0)
				break;
		}
		if (x == 0)
			break;
	}
	flitdb_close(&flit);
	return 0;
}