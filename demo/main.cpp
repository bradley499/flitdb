#include <iostream>
#ifndef FLITDB_LIB_DEMO
#include "flit.cpp"
#else
#include <flit.h>
#endif

int main()
{
	unsigned char authors_count = 3;
	const char *authors[authors_count] = {"Bradley Marshall\0", "Matt Dear\0", "John Hawkins\0"};
	flitdb *flit;
	if (flitdb_setup("demo.db", flit, FLITDB_CREATE) != FLITDB_SUCCESS)
	{
		std::cout << flitdb_errmsg(flit) << std::endl;
		return 1;
	}
	if (flitdb_insert(flit, 1, 1, "Hello") != FLITDB_DONE)
	{
		std::cout << flitdb_errmsg(flit) << std::endl;
		return 1;
	}
	for (unsigned char i = authors_count; i > 0; i--)
	{
		if (flitdb_insert(flit, 2, i, authors[(i - 1)]) != FLITDB_DONE)
		{
			std::cout << flitdb_errmsg(flit) << std::endl;
			return 1;
		}
	}
	switch (flitdb_extract(flit, 1, 1))
	{
	case FLITDB_DONE:
	case FLITDB_NULL:
	{
		std::cout << flitdb_retrieve_char(flit) << ", from FlitDB!" << std::endl;
		break;
	}
	default:
	{
		std::cout << flitdb_errmsg(flit) << std::endl;
		return 1;
	}
	}
	std::cout << std::endl
			  << "Authors:" << std::endl;
	for (unsigned char i = 1; i <= authors_count; i++)
	{
		switch (flitdb_extract(flit, 2, i))
		{
		case FLITDB_DONE:
		case FLITDB_NULL:
		{
			std::cout << "- " << flitdb_retrieve_char(flit) << ((i < authors_count) ? "," : ".") << std::endl;
			flitdb_delete(flit, 2, i);
			break;
		}
		default:
		{
			std::cout << flitdb_errmsg(flit) << std::endl;
			return 1;
		}
		}
	}
	flitdb_close(flit);
	return 0;
}