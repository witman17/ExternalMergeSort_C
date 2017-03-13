#ifndef EXTERNAL_SORT_H   /* Include guard */
#define EXTERNAL_SORT_H

int compare(const void * a, const void * b);
int merge(char *baseFilePath, int fileNumber);
int splitFile(char * filePath);

#endif 