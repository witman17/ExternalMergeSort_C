#ifndef BUCKET_H
#define BUCKET_H

typedef struct StringBucket
{
	char **data;
	int elementsNumber;
	int maxCapacity;
	int maxElementSize;
}bucket;

int bucket_init(bucket *b, size_t elementSize, size_t capacity);
int bucket_resize(bucket *b, size_t newCapacity);
char* bucket_get(bucket *b, const int index);
int bucket_push_back(bucket *b, const char *element, const int elementSize);
int bucket_copy(bucket *destination, bucket *source);
int bucket_to_array(bucket b, char **outArray, int arraySize);
bucket* bucket_merge(bucket *b1, bucket *b2);
int bucket_merge(bucket *b1, bucket *b2, bucket *mergedBucked);
int bucket_merge_data(bucket *b1, bucket *b2, bucket *mergedBucked);
void bucket_clear(bucket *b);
int bucket_clear(bucket *b, size_t capacity);
void bucket_erase(bucket *b);

void bucket_print(bucket *b);

#endif