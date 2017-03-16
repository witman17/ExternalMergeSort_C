#ifndef EXTERNAL_SORT_H   /* Include guard */
#define EXTERNAL_SORT_H

#ifndef MAX_LINE_SIZE
#define  MAX_LINE_SIZE 500
#endif

#ifndef MAX_LINES_NBR
#define  MAX_LINES_NBR 2
#endif

int merge(char *baseFilePath, int fileNumber);
void mergeToFiles(char *filePathIn_1, char *filePathIn_2, char *filePathOut);
int splitFile(char * filePath);
int compare(const void * a, const void * b);
int fillStringBuffer(char buffer[][MAX_LINE_SIZE], const int bufferSize, FILE *fp);
void putStringBuffer(char buffer[][MAX_LINE_SIZE], const int bufferSize, FILE *fp);

#endif 