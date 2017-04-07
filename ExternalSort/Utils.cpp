#include "stdafx.h"
#include "Utils.h"
#include <string.h>

int compareStrings(const void * a, const void * b)
{
	const char *ia = (const char *)a;
	const char *ib = (const char *)b;
	return strcmp(ia, ib);
}

void init_CharArrayASCII(char* charArray, int size){
	int i = 0, j = 0;
	while (i < size){
		charArray[i++] = j++;
		if (j > 128)
			j = 0;
	}
}
