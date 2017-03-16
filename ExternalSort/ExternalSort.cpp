// ExternalSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ExternalSort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char *argv[])
{
	if (argc < 2){
		printf("Blêdna ma³a liczba parametrów! [filePath]");
		getchar();
		return 1;
	}
	printf("Sortuje plik ze sciezki: %s", argv[1]);
	//splitFile(argv[1]);
	//TODO debug
	mergeToFiles("C:\\Users\\Witold\Documents\\Visual Studio 2013\\Projects\\ExternalSort\\Debug\\test_0.txt",
				 "C:\\Users\\Witold\Documents\\Visual Studio 2013\\Projects\\ExternalSort\\Debug\\test_1.txt",
		         "C:\\Users\\Witold\Documents\\Visual Studio 2013\\Projects\\ExternalSort\\Debug\\test_10.txt");
	getchar();
	return 0;
}

int merge(char *baseFilePath, int filesNumber){
	int mergedFileIndex_1 = 0, mergedFileIndex_2 = 1;
	char tempFilePath_1[100], tempFilePath_2[100];
	for (int i = 0; i + 1 < filesNumber; i++);
	return 0;
}

void mergeToFiles(char *filePathIn_1, char *filePathIn_2, char *filePathOut){
	FILE *fp_in_1, *fp_in_2, *fp_out;
	char bufferIn_1[MAX_LINES_NBR][MAX_LINE_SIZE];
	char bufferIn_2[MAX_LINES_NBR][MAX_LINE_SIZE];
	char bufferOut[MAX_LINES_NBR][MAX_LINE_SIZE];
	int  bufferIn_1_Size = 0, bufferIn_2_Size = 0;
	int indexIn_1 = 0, indexIn_2 = 0, indexOut = 0;
	//open files
	fopen_s(&fp_in_1, filePathIn_1, "r");
	fopen_s(&fp_in_2, filePathIn_2, "r");
	fopen_s(&fp_out, filePathOut, "w");
	while (!feof(fp_in_1) && !feof(fp_in_2)){
		//read input to buffers
		if (indexIn_1 >= bufferIn_1_Size){
			bufferIn_1_Size = fillStringBuffer(bufferIn_1, MAX_LINES_NBR, fp_in_1);
			indexIn_1 = 0;
		}
		if (indexIn_2 >= bufferIn_2_Size){
			bufferIn_2_Size = fillStringBuffer(bufferIn_2, MAX_LINES_NBR, fp_in_2);
			indexIn_2 = 0;
		}
		// merge buffers
		while (indexIn_1 < bufferIn_1_Size && indexIn_2 < bufferIn_2_Size){
			if (strcmp(bufferIn_1[indexIn_1], bufferIn_2[indexIn_2]) <= 0){
				strcpy_s(bufferOut[indexOut], MAX_LINE_SIZE, bufferIn_1[indexIn_1]);
				indexIn_1++;
			}
			else{
				strcpy_s(bufferOut[indexOut], MAX_LINE_SIZE, bufferIn_2[indexIn_2]);
				indexIn_2++;
			}
			indexOut++;
			// if out buffer is full, put it to output file
			if (indexOut >= MAX_LINES_NBR){
				putStringBuffer(bufferOut, MAX_LINE_SIZE, fp_out);
				indexOut = 0;
			}
		}
	}
	//empty in buffer if any data left
	while (indexIn_1 < bufferIn_1_Size){
		strcpy_s(bufferOut[indexOut], MAX_LINE_SIZE, bufferIn_1[indexIn_1]);
		indexIn_1++;
		indexOut++;
		if (indexOut >= MAX_LINES_NBR){
			putStringBuffer(bufferOut, MAX_LINE_SIZE, fp_out);
			indexOut = 0;
		}
	}
	//empty in buffer if any data left
	while (indexIn_2 < bufferIn_2_Size){
		strcpy_s(bufferOut[indexOut], MAX_LINE_SIZE, bufferIn_2[indexIn_2]);
		indexIn_2++;
		indexOut++;
		if (indexOut >= MAX_LINES_NBR){
			putStringBuffer(bufferOut, MAX_LINE_SIZE, fp_out);
			indexOut = 0;
		}
	}
	//empty out buffer
	if (indexOut > 0)
		putStringBuffer(bufferOut, indexOut, fp_out);
	//write rest of file_1 if left
	while (!feof(fp_in_1)){
			bufferIn_1_Size = fillStringBuffer(bufferIn_1, MAX_LINES_NBR, fp_in_1);
			putStringBuffer(bufferIn_1, bufferIn_1_Size, fp_out);
	}
	//write rest of file_2 if left
	while (!feof(fp_in_2)){
		bufferIn_2_Size = fillStringBuffer(bufferIn_2, MAX_LINES_NBR, fp_in_2);
		putStringBuffer(bufferIn_2, bufferIn_2_Size, fp_out);
	}
	fclose(fp_in_1);
	fclose(fp_in_2);
	fclose(fp_out);
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
		numberOfLines = fillStringBuffer(LinesArray, MAX_LINES_NBR, fp_in);
		// sort lines
		qsort(LinesArray, numberOfLines, MAX_LINE_SIZE - 1, compare);
		// create new temporary file name
		strncpy_s(newFilePath, filePath, strlen(filePath) - 4);
		sprintf_s(newFilePath, "%s_%d.txt", newFilePath, filesNbr);
		//open out file and write sorted lines
		fopen_s(&fp_out, newFilePath, "w");
		if (!fp_out)
			return filesNbr - 1;
		putStringBuffer(LinesArray, numberOfLines, fp_out);
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

int fillStringBuffer(char buffer[][MAX_LINE_SIZE], const int bufferSize, FILE *fp){
	bool isEOF = false;
	int i = 0;

	if (!fp)
		return -1;

	while (i < bufferSize && !isEOF){
		if (!fgets(buffer[i], MAX_LINE_SIZE - 1, fp))
			isEOF = true;
		i++;
	}
	return i;
}

void putStringBuffer(char buffer[][MAX_LINE_SIZE], const int bufferSize, FILE *fp){
	if (!fp)
		return;
	for (int i = 0; i < bufferSize; i++)
		fputs(buffer[i], fp);
}