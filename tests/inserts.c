#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <flit.h>

#define MAX 999
#define MAX_INSERTIONS 5000
#define MAX_VALUE 99999

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_setup("./test.db", &flit, FLITDB_CREATE) == FLITDB_SUCCESS);
	unsigned int inserted[MAX][MAX] = {{}};
	for (size_t i = 0; i < MAX; i++)
		for (size_t ii = 0; ii < MAX; ii++)
			inserted[i][ii] = 0;
	int inserted_amount = 0;
	while (inserted_amount != MAX_INSERTIONS)
	{
		int x = rand() % MAX;
		int y = rand() % MAX;
		if (inserted[x][y] == 0)
		{
			inserted_amount++;
			while (inserted[x][y] == 0)
				inserted[x][y] = rand() % MAX_VALUE;
			assert(flitdb_insert_int(&flit, (x + 1), (y + 1), inserted[x][y]) == FLITDB_DONE);
		}
	}
	for (int x = 0; x < MAX; x++)
	{
		for (int y = 0; y < MAX; y++)
		{
			if (inserted[x][y] == 0)
				continue;
			flitdb_extract(&flit, (x + 1), (y + 1));
			assert(flitdb_retrieve_int(&flit) == inserted[x][y]);	
		}
	}
	flitdb_close(&flit);
	return 0;
}