#include "stdafx.h"
#include "Utils.h"
#include <string.h>

int compareStrings(const void * a, const void * b)
{
	const char *pa = *(const char**)a;
	const char *pb = *(const char**)b;
	return strcmp(pa, pb);
}

int fillStringBuffer(char **buffer, const int elementSize, const int bufferSize, FILE *fp){
	bool isEOF = false;
	int i = 0;
	if (!fp)
		return -1;
	while (i < bufferSize && !isEOF){
		if (!fgets(buffer[i], elementSize - 1, fp))
			isEOF = true;
		else
			i++;
	}
	return i;
}

void putStringBuffer(char **buffer, const int bufferSize, FILE *fp){
	if (!fp)
		return;
	for (int i = 0; i < bufferSize; i++)
		fputs(buffer[i], fp);
}
