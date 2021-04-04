#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <flit.h>

#define MAX 999
#define MAX_DELETIONS 5000

int main(int argc, char const *argv[])
{
	flitdb *flit;
	assert(flitdb_setup("./test.db", &flit, FLITDB_CREATE) == FLITDB_SUCCESS);
	bool deleted[MAX][MAX] = {{false}};
	int deleted_amount = 0;
	while (deleted_amount != MAX_DELETIONS)
	{
		int x = rand() % MAX;
		int y = rand() % MAX;
		if (!deleted[x][y])
		{
			assert(flitdb_delete(&flit, (x + 1), (y + 1)) == FLITDB_DONE);
			deleted_amount++;
			deleted[x][y] = true;
		}
	}
	flitdb_close(&flit);
	return 0;
}
