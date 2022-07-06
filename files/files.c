#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "files.h"

FILE *openReadFile(char *filename)
{
    FILE *fp;
    char file_with_ext[MAX_FILE_NAME_LENGTH];
    strcpy(file_with_ext, filename);
    strcat(file_with_ext, ".as");
    fp = fopen(file_with_ext, READ);
    if (!fp)
    {
        fprintf(stderr, "ERROR: File '%s' was not found\n", file_with_ext);
    }
    return fp;
}

FILE *openWriteFile(char *filename, char *ext)
{
    FILE *fp;
    char file_with_ext[MAX_FILE_NAME_LENGTH];
    strcpy(file_with_ext, filename);
    strcat(file_with_ext, ext);
    fp = fopen(file_with_ext, WRITE);
    if (!fp)
    {
        /* TODO change this error message */
        fprintf(stderr, "ERROR: File '%s' was not found\n", file_with_ext);
    }
    return fp;
}

/*char *trimLine(char *line)
{
    char new_line[MAX_LINE_LENGTH] = {0};
    int i;
    for (i = 0; isspace(*(line + i)); i++)
        ;
    strcpy(new_line, (line + i));
}

char *firstWordInLine(char *line)
{
    char word[MAX_LINE_LENGTH] = {0};
    sscanf(line, " %s ", word);
    return word;
}
*/