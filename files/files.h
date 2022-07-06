#ifndef FILES_S
#define FILES_S

#include <stdio.h>
#define MAX_LINE_LENGTH 80
#define MAX_FILE_NAME_LENGTH 31
#define READ "r"
#define WRITE "w"

FILE *openReadFile(char *filename);
FILE *openWriteFile(char *filename, char *ext);
#endif