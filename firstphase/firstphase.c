#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../strings/strings.h"
#include "../files/files.h"
#include "firstphase.h"
#include "../symbols/symbols.h"

#define DATA ".data"
#define STRUCT ".struct"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERN ".extern"

int encodeDataline(word data_img[255], char *line, unsigned char *DC, Symbol **symbol_table);

int firstAssemblerMain(char *filename, word code_img[255], word data_img[255], unsigned char *IC, unsigned char *DC, Symbol *symbol_table)
{
    FILE *fd;
    Line full_line, rest_line;
    bool in_symbol = false;
    Label symbol_name;
    Label curr_word;
    int res, i;

    fd = openReadFile(filename, ".am");
    while (fgets(full_line, 80, fd) != NULL)
    {
        /* check if line is not empty */
        if (!isEmptyString(full_line))
        {
            /* check if line starts with a label - step 3 */
            /* printf("first %d", strstr(full_line, ":")); */
            if (strstr(full_line, ":"))
            {
                strcpy(symbol_name, strtok(full_line, ":"));
                if (isValidLabel(symbol_name))
                {
                    in_symbol = true; /* step 4 */
                    /* printf("new symbol: %s\n", symbol_name); */
                }
                else
                {
                    printf("Error: Bad label name\n");
                }
            }

            strcpy(rest_line, trimLeft(strtok(in_symbol ? NULL : full_line, "\n")));
            /* printf("rest line '%s'\n", rest_line); */
            if (rest_line[0] == '.')
            {
                /*
                    add to symbol (name, curr DC, isop = false, isext = 0, next = NULL)
                    if symbol already found - print error
                    encode the data in data_img and update DC
                */
                if (in_symbol)
                {

                    if (!isExistingSymbol(symbol_table, symbol_name))
                    {
                        addSymbol(&symbol_table, symbol_name, *DC, false, false);
                        sscanf(rest_line, ".%s", curr_word);
                    }
                    else
                    {
                        printf("Error: symbol already existing\n");
                    }
                }
                encodeDataline(data_img, rest_line, DC, &symbol_table);
            }

            in_symbol = false;
            symbol_name[0] = 0;
        }
    }
    printSymbols(symbol_table);
    printf("after\n");
    printf("last ----- Symbol Address %d --- %d, %d\n", &symbol_table, symbol_table, sizeof(*symbol_table));
    return 1;
}

int encodeData(char *vars, word *data_img, unsigned char *DC)
{
    int number = 0;
    char *temp;
    temp = strtok(vars, ",");
    while (temp != NULL)
    {
        number = atoi(temp);
        (data_img + (*DC)++)->data.value = number;
        temp = strtok(NULL, ",");
    }
    return *DC;
}
int encodeString(char *vars, word *data_img, unsigned char *DC)
{
    char *start, *end;
    start = strstr(vars, "\"");
    end = strstr(++start, "\"");
    while (start < end)
    {
        (data_img + (*DC)++)->data.value = (*start);
        start++;
    }
    (data_img + (*DC)++)->data.value = 0;
    return *DC;
}

int encodeStruct(char *vars, word *data_img, unsigned char *DC)
{
    int number = 0;
    char *tmp;
    tmp = strtok(vars, ",");
    number = atoi(tmp);
    (data_img + (*DC)++)->data.value = number;
    tmp = strtok(NULL, ",");
    encodeString(tmp, data_img, DC);

    return *DC;
}

int encodeExtern(char *vars, Symbol **symbol_table)
{
    Label name;
    strcpy(name, trimLeft(strtok(vars, "")));
    if (!isExistingSymbol(*symbol_table, name))
    {
        addSymbol(symbol_table, name, 0, false, true);
    }
    else
    {
        printf("Error: symbol already existing\n");
    }

    return 1;
}

int encodeDataline(word data_img[255], char *line, unsigned char *DC, Symbol **symbol_table)
{
    Label type;
    unsigned short res;
    Line vars;

    strcpy(type, strtok(line, " "));
    strcpy(vars, strtok(NULL, ""));
    if (!strcmp(type, DATA))
        res = encodeData(vars, data_img, DC);
    else if (!strcmp(type, STRUCT))
        res = encodeStruct(vars, data_img, DC);
    else if (!strcmp(type, STRING))
        res = encodeString(vars, data_img, DC);
    else if (!strcmp(type, EXTERN))
        res = encodeExtern(vars, symbol_table);
    else
    {
        if (strcmp(type, ENTRY))
        {
            printf("Error: Invalid data line\n");
            res = 0;
        }
    }

    return res;
}