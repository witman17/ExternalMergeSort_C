#ifndef UTILS_H
#define UTILS_H

int compareStrings(const void * a, const void * b);
int fillStringBuffer(char **buffer, const int elementSize, const int bufferSize, FILE *fp);
void putStringBuffer(char **buffer, const int bufferSize, FILE *fp);

#endif

