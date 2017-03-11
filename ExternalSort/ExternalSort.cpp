// ExternalSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  MAX_LINE_SIZE 500
#define  MAX_LINES_NBR 1000
int compare(const void * a, const void * b)
{
	const char *ia = (const char *)a;
	const char *ib = (const char *)b;
	return strcmp(ia, ib);
}

int main(int argc, char *argv[])
{
	FILE *fp;
	int i = 0, maxLineSize, maxLinesNbr;
	char linesArray;
	char *fileName;

	if (argc != 2){
		printf("Blêdna ma³a liczba parametrów! [filePath]");
		getchar();
		return 1;
	}
	printf("Dzia³a: %s %d %d", argv[1], maxLineSize, maxLinesNbr);
	// initalise linesArray
	//file open
	
	// read linesp
	while (i < maxLinesNbr){
		linesArray[i] = (char*) malloc(sizeof(char) * maxLineSize + 1);
		if (fgets(linesArray[i], maxLineSize + 1, fp) == NULL)
			break;
		i++;
	}

	
	fclose(fp);
	getchar();
	return 0;
}

int splitFile(char * filePath){
	FILE *fp;
	int filesNbr = 0, i = 0;
	char LinesArray[MAX_LINES_NBR][MAX_LINE_SIZE];

	fopen_s(&fp, filePath, "r");
	while (fgets(LinesArray[i], MAX_LINE_SIZE, fp) && i < MAX_LINES_NBR){
		i++;
	}
	return filesNbr;
}

