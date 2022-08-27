#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "files.h"
#include "../strings/strings.h"

FILE *openReadFile(char *filename, char *ext)
{
    FILE *fp;
    label filename_w_ext;
    strcpy(filename_w_ext, filename);
    strcat(filename_w_ext, ext);
    fp = fopen(filename_w_ext, READ);
    if (!fp)
    {
        fprintf(stderr, "ERROR: File '%s' was not found\n", filename_w_ext);
    }
    return fp;
}

FILE *openWriteFile(char *filename, char *ext)
{
    FILE *fp;
    label filename_w_ext;
    strcpy(filename_w_ext, filename);
    strcat(filename_w_ext, ext);
    fp = fopen(filename_w_ext, WRITE);
    if (!fp)
    {
        /* TODO change this error message */
        fprintf(stderr, "ERROR: File '%s' was not found\n", filename_w_ext);
    }
    return fp;
}