#ifndef EXTERNAL_SORT_H   /* Include guard */
#define EXTERNAL_SORT_H

#include "Bucket.h"

#define  MAX_LINE_SIZE 100
#define  MAX_LINES_NBR 1000
#define  MAX_THREAD_NUMBER 100


int merge(char *baseFilePath, int filesNumber, int maxLineSize, int maxLineNumber);
int mergeToFiles(char *filePathIn_1, char *filePathIn_2, char *filePathOut, int maxLineSize, int maxLineNumber);

int splitFile(char * filePath, int maxLineSize, int maxLineNumber);
int parrallelInternalMergeSort(bucket *buffer);

#endif 