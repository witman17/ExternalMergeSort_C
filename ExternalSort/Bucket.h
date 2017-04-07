#ifndef BUCKET_H

typedef struct CharBucket
{
	char **data;
	int elementsNumber;
	int maxCapacity;
	int maxElementSize;
}bucket;

int bucket_init(bucket *b, size_t elementSize, size_t capacity);
int bucket_resize(bucket *b, size_t newCapacity);
int bucket_push_back(bucket *b, char *element, int elementSize);
bucket* bucket_merge(bucket *b1, bucket *b2);
int bucket_clear(bucket *b);

#endif