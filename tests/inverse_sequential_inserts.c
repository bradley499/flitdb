#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "flit.h"

#define MAX 70
#define MAX_INSERTIONS (MAX * MAX)
#define MAX_VALUE 99999

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_open("./test2.db", &flit, FLITDB_CREATE) == FLITDB_SUCCESS);
	int (*inserted)[MAX];
	inserted = malloc((MAX + 1) * (MAX + 1) * sizeof inserted[0][0]);
	for (unsigned short x = MAX; x > 0; x--)
	{
		for (unsigned short y = MAX; y > 0; y--)
		{
			inserted[x][y] = rand() % MAX_VALUE;
			assert(flitdb_insert_int(&flit, (x + 1), (y + 1), inserted[x][y]) == FLITDB_DONE);
		}
	}
	for (unsigned short x = MAX; x > 0; x--)
	{
		for (unsigned short y = MAX; y > 0; y--)
		{
			flitdb_extract(&flit, (x + 1), (y + 1));
			assert(flitdb_retrieve_int(&flit) == inserted[x][y]);	
		}
	}
	flitdb_close(&flit);
	free(inserted);
	return 0;
}