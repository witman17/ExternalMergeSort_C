#include "stdafx.h"
#include "Utils.h"
#include <string.h>

int compareStrings(const void * a, const void * b)
{
	const char *ia = (const char *)a;
	const char *ib = (const char *)b;
	return strcmp(ia, ib);
}
