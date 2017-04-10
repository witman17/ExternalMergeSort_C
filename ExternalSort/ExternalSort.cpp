// ExternalSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ExternalSort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "Bucket.h"
#include "Utils.h"
#include "math.h"



int main(int argc, char *argv[])
{
	int maxLineSize = MAX_LINE_SIZE, maxLineNumber = MAX_LINES_NBR;
	//get params
	if (argc < 2){
		printf("Bledna liczba parametrów! [filePath]");
		getchar();
		return 1;
	}//optional params
	if (argc > 2)
		maxLineNumber = atoi(argv[2]);
	if (argc > 3)
		maxLineSize = atoi(argv[3]);
	// Sorting
	printf("Sortuje plik ze sciezki: %s", argv[1]);
	if (merge(argv[1], splitFile(argv[1], maxLineSize, maxLineNumber), maxLineSize, maxLineNumber)){
		printf("Sortowanie nie powiod³o siê");
		return 1;
	}
	return 0;
}

int merge(char *baseFilePath, int filesNumber, int maxLineSize, int maxLineNumber){
	int mergedFileIndex_1 = 0, mergedFileIndex_2 = 1;
	int filesLeftToMerge = filesNumber;
	char filePathNoExtention[100], filePathIn_1[100], filePathIn_2[100], filePathOut[100];
	// init base file path without extention
	strncpy_s(filePathNoExtention, baseFilePath, strlen(baseFilePath) - 4);
	// init output file name if only 1 file
	if (filesLeftToMerge == 1){
		sprintf_s(filePathOut, "%s_%d.txt", filePathNoExtention, 0);
	}
	while (filesLeftToMerge > 1){
		// build in filepaths
		sprintf_s(filePathIn_1, "%s_%d.txt", filePathNoExtention, mergedFileIndex_1);
		sprintf_s(filePathIn_2, "%s_%d.txt", filePathNoExtention, mergedFileIndex_2);
		// build out filepath
		sprintf_s(filePathOut, "%s_%d.txt", filePathNoExtention, filesNumber++);
		// merge files
		if (mergeToFiles(filePathIn_1, filePathIn_2, filePathOut, maxLineSize, maxLineNumber)){
			// failed remove temp files
			for (int i = 0; i < filesLeftToMerge; i++){
				remove(filePathIn_1);
				sprintf_s(filePathIn_1, "%s_%d.txt", filePathNoExtention, ++mergedFileIndex_1);
			}
			return 1;
		}
		filesLeftToMerge--;
		mergedFileIndex_1 += 2;
		mergedFileIndex_2 += 2;
		// remove merged files
		remove(filePathIn_1);
		remove(filePathIn_2);
	}
	// rename result file
	sprintf_s(filePathIn_1, "%s_%s.txt", filePathNoExtention, "sorted");
	if (rename(filePathOut, filePathIn_1))
		printf("Nie mo¿na zmieniæ nazwy pliku wynikowego: %s", filePathOut);
	return 0;
}

int mergeToFiles(char *filePathIn_1, char *filePathIn_2, char *filePathOut, int maxLineSize, int maxLineNumber){
	FILE *fp_in_1, *fp_in_2, *fp_out;
	int bufferStartOffset_1, bufferStartOffset_2;
	bucket bufferIn_1, bufferIn_2, bufferOut;
	// init buffers
	if (bucket_init(&bufferIn_1, maxLineSize, maxLineNumber))
		return 1; // failed
	if (bucket_init(&bufferIn_2, maxLineSize, maxLineNumber))
		return 1;
	if (bucket_init(&bufferOut, maxLineSize, maxLineNumber * 2))
		return 1;
	//open files
	fopen_s(&fp_in_1, filePathIn_1, "r");
	fopen_s(&fp_in_2, filePathIn_2, "r");
	fopen_s(&fp_out, filePathOut, "w");
	while (!feof(fp_in_1) && !feof(fp_in_2)){
		//read input to buffers
		if (bufferIn_1.elementsNumber == 0){
			bufferStartOffset_1 = 0;
			bufferIn_1.elementsNumber = fillStringBuffer(bufferIn_1.data, bufferIn_1.maxElementSize, bufferIn_1.maxCapacity, fp_in_1);
		}
		if (bufferIn_2.elementsNumber == 0){
			bufferStartOffset_2 = 0;
			bufferIn_2.elementsNumber = fillStringBuffer(bufferIn_2.data, bufferIn_2.maxElementSize, bufferIn_2.maxCapacity, fp_in_2);
		}
		// merge buffers
		while (bufferIn_1.elementsNumber > 0 && bufferIn_2.elementsNumber > 0){
			if (strcmp(bufferIn_1.data[bufferStartOffset_1], bufferIn_2.data[bufferStartOffset_2]) <= 0){
				bucket_push_back(&bufferOut, bufferIn_1.data[bufferStartOffset_1++], bufferIn_1.maxElementSize);
				bufferIn_1.elementsNumber--;
			}
			else{
				bucket_push_back(&bufferOut, bufferIn_2.data[bufferStartOffset_2++], bufferIn_2.maxElementSize);
				bufferIn_2.elementsNumber--;
			}
			// put output buffer to file
			if (bufferOut.elementsNumber >= maxLineNumber * 2){
				putStringBuffer(bufferOut.data, bufferOut.elementsNumber, fp_out);
				bucket_clear(&bufferOut);
			}
		}
	}
	// check which file ended
	while (bufferIn_1.elementsNumber > 0 && !feof(fp_in_2)){
		if (bufferIn_2.elementsNumber == 0){
			bufferStartOffset_2 = 0;
			bufferIn_2.elementsNumber = fillStringBuffer(bufferIn_2.data, bufferIn_2.maxElementSize, bufferIn_2.maxCapacity, fp_in_2);
		}
		// merge buffers
		while (bufferIn_1.elementsNumber > 0 && bufferIn_2.elementsNumber > 0){
			if (strcmp(bufferIn_1.data[bufferStartOffset_1], bufferIn_2.data[bufferStartOffset_2]) <= 0){
				bucket_push_back(&bufferOut, bufferIn_1.data[bufferStartOffset_1++], bufferIn_1.maxElementSize);
				bufferIn_1.elementsNumber--;
			}
			else{
				bucket_push_back(&bufferOut, bufferIn_2.data[bufferStartOffset_2++], bufferIn_2.maxElementSize);
				bufferIn_2.elementsNumber--;
			}
			// put output buffer to file
			if (bufferOut.elementsNumber >= maxLineNumber * 2){
				putStringBuffer(bufferOut.data, bufferOut.elementsNumber, fp_out);
				bucket_clear(&bufferOut);
			}
		}
	}
	while (bufferIn_2.elementsNumber > 0 && !feof(fp_in_1)){
		if (bufferIn_1.elementsNumber == 0){
			bufferStartOffset_1 = 0;
			bufferIn_1.elementsNumber = fillStringBuffer(bufferIn_1.data, bufferIn_1.maxElementSize, bufferIn_1.maxCapacity, fp_in_1);
		}
		// merge buffers
		while (bufferIn_1.elementsNumber > 0 && bufferIn_2.elementsNumber > 0){
			if (strcmp(bufferIn_1.data[bufferStartOffset_1], bufferIn_2.data[bufferStartOffset_2]) <= 0){
				bucket_push_back(&bufferOut, bufferIn_1.data[bufferStartOffset_1++], bufferIn_1.maxElementSize);
				bufferIn_1.elementsNumber--;
			}
			else{
				bucket_push_back(&bufferOut, bufferIn_2.data[bufferStartOffset_2++], bufferIn_2.maxElementSize);
				bufferIn_2.elementsNumber--;
			}
			// put output buffer to file
			if (bufferOut.elementsNumber >= maxLineNumber * 2){
				putStringBuffer(bufferOut.data, bufferOut.elementsNumber, fp_out);
				bucket_clear(&bufferOut);
			}
		}
	}
	//empty bufferIn_1 if data left
	while (bufferIn_1.elementsNumber > 0){
		bucket_push_back(&bufferOut, bufferIn_1.data[bufferStartOffset_1++], bufferIn_1.maxElementSize);
		bufferIn_1.elementsNumber--;
		// put output buffer to file
		if (bufferOut.elementsNumber >= maxLineNumber * 2){
			putStringBuffer(bufferOut.data, bufferOut.elementsNumber, fp_out);
			bucket_clear(&bufferOut);
		}
	}
	//empty bufferIn_2 if data left
	while (bufferIn_2.elementsNumber > 0){
		bucket_push_back(&bufferOut, bufferIn_2.data[bufferStartOffset_2++], bufferIn_2.maxElementSize);
		bufferIn_2.elementsNumber--;
		// put output buffer to file
		if (bufferOut.elementsNumber >= maxLineNumber * 2){
			putStringBuffer(bufferOut.data, bufferOut.elementsNumber, fp_out);
			bucket_clear(&bufferOut);
		}
	}
	//empty buffer out
	if (bufferOut.elementsNumber > 0){
		putStringBuffer(bufferOut.data, bufferOut.elementsNumber, fp_out);
	}
	//write rest of file_1 if left
	while (!feof(fp_in_1)){
		bufferIn_1.elementsNumber = fillStringBuffer(bufferIn_1.data, bufferIn_1.maxElementSize, bufferIn_1.maxCapacity, fp_in_1);
		if (bufferIn_1.elementsNumber > 0)
			putStringBuffer(bufferIn_1.data, bufferIn_1.elementsNumber, fp_out);
	}
	//write rest of file_2 if left
	while (!feof(fp_in_2)){
		bufferIn_2.elementsNumber = fillStringBuffer(bufferIn_2.data, bufferIn_2.maxElementSize, bufferIn_2.maxCapacity, fp_in_2);
		if (bufferIn_2.elementsNumber > 0)
			putStringBuffer(bufferIn_2.data, bufferIn_2.elementsNumber, fp_out);
	}
	// free buckets
	bucket_erase(&bufferIn_1);
	bucket_erase(&bufferIn_2);
	bucket_erase(&bufferOut);
	//close files
	fclose(fp_in_1);
	fclose(fp_in_2);
	fclose(fp_out);
	return 0;
}

int splitFile(char * filePath, int maxLineSize, int maxLineNumber){
	FILE *fp_in, *fp_out;
	bool isEOF = false;
	int filesNbr = 0, i = 0, numberOfLines = 0;
	char newFilePath[500];
	bucket linesBuffer;

	fopen_s(&fp_in, filePath, "r");
	if (!fp_in)
		return 0;
	//init buffer
	if (bucket_init(&linesBuffer, maxLineSize, maxLineNumber))
		return 0;
	while (!feof(fp_in)){
		//read max chunk of lines
		linesBuffer.elementsNumber = fillStringBuffer(linesBuffer.data, linesBuffer.maxElementSize, linesBuffer.maxCapacity, fp_in);
		if (linesBuffer.elementsNumber > 0){
			// sort lines
			//qsort(linesBuffer.data, linesBuffer.elementsNumber, sizeof(char*), compareStrings);
			parrallelInternalMergeSort(&linesBuffer);
			// create new temporary file name
			strncpy_s(newFilePath, filePath, strlen(filePath) - 4);
			sprintf_s(newFilePath, "%s_%d.txt", newFilePath, filesNbr);
			//open out file and write sorted lines
			fopen_s(&fp_out, newFilePath, "w");
			if (!fp_out)
				return filesNbr - 1;
			putStringBuffer(linesBuffer.data, linesBuffer.elementsNumber, fp_out);
			fclose(fp_out);
			filesNbr++;
		}
	}
	bucket_erase(&linesBuffer);
	fclose(fp_in);
	return filesNbr;
}

int parrallelInternalMergeSort(bucket *buffer){
	int threadsNumber = omp_get_num_procs();
	int bucketSize, outBucketIndex, mergedBucketIndex_1 = 0, mergedBucketIndex_2 = 1, maxBucketArraySize;
	if (threadsNumber > buffer->elementsNumber)
		threadsNumber = buffer->elementsNumber;
	maxBucketArraySize = threadsNumber + 1;
	outBucketIndex = threadsNumber;
	if (threadsNumber < 2)
		qsort(buffer->data, buffer->elementsNumber, sizeof(char*), compareStrings);
	else
	{
		bucket *buckets = (bucket *)malloc((maxBucketArraySize)* sizeof(bucket));
		if (!buckets)
			return 1;
		bucketSize = (buffer->elementsNumber / threadsNumber);
		bucketSize += (int)ceil((double)(buffer->elementsNumber % threadsNumber) / (double)threadsNumber);
		// initialize buckets tab
		for (int i = 0; i < threadsNumber + 1; i++)
			if (bucket_init(&buckets[i], buffer->maxElementSize, bucketSize))
				return 1;
		// fill buckets
		for (int i = 0, j = 0, k = 0; i < buffer->elementsNumber; i++, j++){
			if (j >= bucketSize && k < threadsNumber - 1){
				j = 0;
				k++;
			}
			bucket_push_back(&buckets[k], buffer->data[i], buffer->maxElementSize);
		}
		// sort buckets
		for (int i = 0; i < threadsNumber; i++)
			qsort(buckets[i].data, buckets[i].elementsNumber, sizeof(char *), compareStrings);
		//merge buckets
		while (threadsNumber > 1){
			int outBucketCapacity;
			bucket_merge(&buckets[mergedBucketIndex_1], &buckets[mergedBucketIndex_2], &buckets[outBucketIndex]);
			// erase merged buckets
			bucket_erase(&buckets[mergedBucketIndex_1]);
			bucket_erase(&buckets[mergedBucketIndex_2]);
			//set indexes
			mergedBucketIndex_1 = (mergedBucketIndex_1 + 2) % maxBucketArraySize;
			mergedBucketIndex_2 = (mergedBucketIndex_2 + 2) % maxBucketArraySize;
			if (--threadsNumber > 1)
				outBucketIndex = (outBucketIndex + 1) % maxBucketArraySize;
			//check if out bucket is not initialized
			if (buckets[outBucketIndex].maxCapacity == 0){
				outBucketCapacity = buckets[mergedBucketIndex_1].elementsNumber + buckets[mergedBucketIndex_2].elementsNumber;
				bucket_init(&buckets[outBucketIndex], buffer->maxElementSize, outBucketCapacity);
			}
		}
		// copy data
		bucket_copy(buffer, &buckets[outBucketIndex]);
		// deallocate  temporary buckets
		for (int i = 0; i < maxBucketArraySize; i++)
			bucket_erase(&buckets[i]);
		free(buckets);
	}
	return 0;
}