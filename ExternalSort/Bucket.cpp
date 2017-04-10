#include "stdafx.h"
#include "stdlib.h"
#include "Bucket.h"
#include <string.h>

int bucket_init(bucket *b, size_t elementSize, size_t capacity){
	b->data = (char **) malloc(capacity * sizeof(char *));
	if (!b->data)
		return 1;
	for (unsigned i = 0; i < capacity; i++)
		b->data[i] = (char *) malloc(elementSize * sizeof(char));
	b->elementsNumber = 0;
	b->maxCapacity = capacity;
	b->maxElementSize = elementSize;
	return 0;
}

int bucket_resize(bucket *b, size_t newCapacity){
	// no resize needed
	if (b->maxCapacity == newCapacity)
		return 0;

	char **newData = (char **)realloc(b->data, newCapacity * sizeof(char *));
	if (!newData){
		return 1;
	}
	b->data = newData;
	for (unsigned i = b->maxCapacity; i < newCapacity; i++)
		b->data[i] = (char *)malloc(b->maxElementSize * sizeof(char));
	if (!b->data[b->maxCapacity])
		return 1;
	b->maxCapacity = newCapacity;
	return 0;
}

char* bucket_get(bucket *b, const unsigned index){
	if (!b || index >= b->elementsNumber)
		return NULL;
	return b->data[index];
}

int bucket_push_back(bucket *b, const char *element, const int elementSize){
	int newElementSize;
	if (b->elementsNumber + 1 > b->maxCapacity)
		if (bucket_resize(b, (b->maxCapacity+2) * 2))
			return 1;
	//if in elementSize > maxElementSize element will be truncated
	if (elementSize > b->maxElementSize)
		strcpy_s(b->data[b->elementsNumber++], b->maxElementSize, element);
	else
		strcpy_s(b->data[b->elementsNumber++], elementSize, element);
	return 0;
}

int bucket_copy(bucket *destination, bucket *source){
	int i = 0;
	if (!destination || !source)
		return 1;
	//clear destination bucket
	if (destination->maxCapacity < source->elementsNumber)
		bucket_clear(destination, source->elementsNumber);
	else
		bucket_clear(destination);
	//copy data
	for (int i = 0; i < source->elementsNumber; i++)
		if (bucket_push_back(destination, source->data[i], source->maxElementSize))
			return 1;
	return 0;
}

bucket* bucket_merge(bucket *b1, bucket *b2){
	//set merged bucket element size
	int mergedElementSize;
	if (!b1 || !b2)
		return NULL;
	if (b1->maxElementSize > b2->maxElementSize)
		mergedElementSize = b1->maxElementSize;
	else
		mergedElementSize = b2->maxElementSize;
	//init merged bucket
	bucket *out = (bucket *) malloc(sizeof(bucket));
	if (!out || !b1->data || !b2->data)
		return NULL;
	if (bucket_init(out, mergedElementSize, b1->elementsNumber + b2->elementsNumber)){
		free(out);
		return NULL;
	}
	//merge
	if (bucket_merge_data(b1, b2, out))
		return NULL;

	return out;
}

int bucket_merge(bucket *b1, bucket *b2, bucket *mergedBucked){
	if (!b1 || !b2)
		return 1;
	// if output not initialized
	if (!mergedBucked){
		mergedBucked = bucket_merge(b1, b2);
		if (!mergedBucked)
			return 1; //merge failed
		else
			return 0;
	}
	//merge normally
	if (bucket_merge_data(b1, b2, mergedBucked))
		return 1; //merge failed
	return 0;
}

int bucket_merge_data(bucket *b1, bucket *b2, bucket *out){
	int b1_index = 0, b2_index = 0;
	if (!b1->data || !b2->data || !out->data)
		return 1;
	while (b1_index < b1->elementsNumber && b2_index < b2->elementsNumber)
		if (strcmp(b1->data[b1_index], b2->data[b2_index]) <= 0)
			bucket_push_back(out, b1->data[b1_index++], b1->maxElementSize);
		else
			bucket_push_back(out, b2->data[b2_index++], b2->maxElementSize);
	// fill data if left from bucket 1
	while (b1_index < b1->elementsNumber)
		bucket_push_back(out, b1->data[b1_index++], b1->maxElementSize);
	// fill data if left from bucket 2
	while (b2_index < b2->elementsNumber)
		bucket_push_back(out, b2->data[b2_index++], b2->maxElementSize);
	return 0;
}

void bucket_clear(bucket *b){
	b->elementsNumber = 0;
}

int bucket_clear(bucket *b, size_t capacity){
	b->elementsNumber = 0;
	return bucket_resize(b, capacity);
}

void bucket_erase(bucket *b){
	for (int i = b->maxCapacity - 1; i >= 0; i--)
		free(b->data[i]);
	free(b->data);
	b->data = NULL;
	b->elementsNumber = 0;
	b->maxCapacity = 0;
	b->maxElementSize = 0;
}

void bucket_print(bucket *b){
	printf("bucket[%d/%d]:\n", b->elementsNumber, b->maxCapacity);
	for (int i = 0; i < b->elementsNumber; i++)
		printf("%s", b->data[i]);
	printf("----------\n");
}
