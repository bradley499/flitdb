#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <flit.h>

#define MAX 70
#define MAX_INSERTIONS (MAX * MAX)
#define MAX_VALUE 99999

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_setup("./test2.db", &flit, FLITDB_CREATE) == FLITDB_SUCCESS);
	unsigned int inserted[MAX][MAX] = {{}};
	for (unsigned short x = 0; x < MAX; x++)
	{
		for (unsigned short y = 0; y < MAX; y++)
		{
			inserted[x][y] = rand() % MAX_VALUE;
			assert(flitdb_insert_int(&flit, (x + 1), (y + 1), inserted[x][y]) == FLITDB_DONE);
		}
	}
	for (unsigned short x = 0; x < MAX; x++)
	{
		for (unsigned short y = 0; y < MAX; y++)
		{
			flitdb_extract(&flit, (x + 1), (y + 1));
			assert(flitdb_retrieve_int(&flit) == inserted[x][y]);	
		}
	}
	flitdb_close(&flit);
	return 0;
}