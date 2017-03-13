// ExternalSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ExternalSort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  MAX_LINE_SIZE 500
#define  MAX_LINES_NBR 2



int main(int argc, char *argv[])
{
	if (argc < 2){
		printf("Blêdna ma³a liczba parametrów! [filePath]");
		getchar();
		return 1;
	}
	printf("Sortuje plik ze sciezki: %s", argv[1]);
	splitFile(argv[1]);
	getchar();
	return 0;
}

int merge(char *baseFilePath, int filesNumber){
	int mergedFileIndex_1 = 0, mergedFileIndex_2 = 1;
	char tempFilePath_1[100], tempFilePath_2[100];
	for (int i = 0; i + 1 < filesNumber; i++);
	return 0;
}

int mergeToFiles(char *filePathIn_1, char *filePathIn_2, char *filePathOut){
	FILE *fp_in_1, *fp_in_2, *fp_out;
	char bufferIn_1[MAX_LINE_SIZE], bufferIn_2[MAX_LINE_SIZE], bufferOut[MAX_LINE_SIZE];
	//open files
	fopen_s(&fp_in_1, filePathIn_1, "r");
	fopen_s(&fp_in_2, filePathIn_2, "r");
	fopen_s(&fp_out, filePathOut, "w");
	//read input to buffers

}

int splitFile(char * filePath){
	FILE *fp_in, *fp_out;
	bool isEOF = false;
	int filesNbr = 0, i = 0, numberOfLines = 0;
	char newFilePath[500];
	char LinesArray[MAX_LINES_NBR][MAX_LINE_SIZE];

	fopen_s(&fp_in, filePath, "r");
	if (!fp_in)
		return 0;
	while (!feof(fp_in)){
		//read max chunk of lines
		i = 0;
		while (i < MAX_LINES_NBR && !isEOF){
			if (!fgets(LinesArray[i], MAX_LINE_SIZE - 1, fp_in))
				isEOF = true;
			i++;
		}		
		numberOfLines = i;
		// sort lines
		qsort(LinesArray, numberOfLines, MAX_LINE_SIZE - 1, compare);
		// create new temporary file name
		strncpy_s(newFilePath, filePath, strlen(filePath) - 4);
		sprintf_s(newFilePath, "%s_%d.txt", newFilePath, filesNbr);
		//open out file and write sorted lines
		fopen_s(&fp_out, newFilePath, "w");
		if (!fp_out)
			return filesNbr - 1;
		for (int i = 0; i < numberOfLines; i++)
			fputs(LinesArray[i], fp_out);
		fclose(fp_out);
		filesNbr++;
	}
	fclose(fp_in);
	return filesNbr;
}

int compare(const void * a, const void * b)
{
	const char *ia = (const char *)a;
	const char *ib = (const char *)b;
	return strcmp(ia, ib);
}

