#ifndef FILES_S
#define FILES_S

#include <stdio.h>

#define READ "r"
#define WRITE "w"
#define INPUT_FILE_EXT ".as"
#define AFTER_MACROS_FILE_EXT ".am"
#define OBJECT_FILE_EXT ".ob"
#define ENTRIES_FILE_EXT ".ent"
#define EXTERNALS_FILE_EXT ".ext"

FILE *openReadFile(char *filename, char *ext);
FILE *openWriteFile(char *filename, char *ext);
#endif