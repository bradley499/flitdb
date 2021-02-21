#include <stdio.h>
#ifndef FLITDB_LIB_DEMO
#include "flit.c"
#else
#include <flit.h>
#endif

int main()
{
	unsigned char authors_count = 3;
	const char *authors[3] = {"Bradley Marshall\0", "Matt Dear\0", "John Hawkins\0"};
	flitdb *flit;
	if (flitdb_setup("demo.db", &flit, FLITDB_CREATE) != FLITDB_SUCCESS)
	{
		printf("%s\n", flitdb_errmsg(&flit));
		return 1;
	}
	if (flitdb_insert_const_char(&flit, 1, 1, "Hello") != FLITDB_DONE)
	{
		printf("%s\n", flitdb_errmsg(&flit));
		return 1;
	}
	for (unsigned char i = authors_count; i > 0; i--)
	{
		if (flitdb_insert_const_char(&flit, 2, i, authors[(i - 1)]) != FLITDB_DONE)
		{
			printf("%s\n", flitdb_errmsg(&flit));
			return 1;
		}
	}
	switch (flitdb_extract(&flit, 1, 1))
	{
	case FLITDB_DONE:
	case FLITDB_NULL:
	{
		printf("%s, from FlitDB!\n\n", flitdb_retrieve_char(&flit));
		break;
	}
	default:
	{
		printf("%s\n", flitdb_errmsg(&flit));
		return 1;
	}
	}
	printf("Authors:\n");
	for (unsigned char i = 1; i <= authors_count; i++)
	{
		switch (flitdb_extract(&flit, 2, i))
		{
		case FLITDB_DONE:
		case FLITDB_NULL:
		{
			printf("- %s%s\n", flitdb_retrieve_char(&flit), ((i < authors_count) ? "," : "."));
			flitdb_delete(&flit, 2, i);
			break;
		}
		default:
		{
			printf("%s\n", flitdb_errmsg(&flit));
			return 1;
		}
		}
	}
	flitdb_close(&flit);
	return 0;
}