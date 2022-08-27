#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "../files/files.h"
#include "../methods/methods.h"
#include "strings.h"

/* check if a string is a valid label name
    max length 30
    first letter is alphanumeric
    all other letter are alphanumeric or numbers */
bool isValidLabel(label name)
{
    int i;
    size_t str_len;
    bool is_valid = true;
    /* todo - get from methods */
    cmd_name cmd_names[NUMBER_OF_COMMANDS] = {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, GET, PRN, JSR, RTS, HLT};

    str_len = strlen(name);

    /* check if general register name  todo - WET code here (methods.c) */
    if (str_len == 2 && name[0] == 'r' && name[1] >= '0' && name[1] <= '7')
        is_valid = false;
    else if (str_len == 3)
    {
        /* check if command name */
        for (i = 0; i < NUMBER_OF_COMMANDS && is_valid; i++)
        {
            if (!strcmp(name, cmd_names[i]))
                is_valid = false;
        }
        /* check if 'PSW' register */
        if (!strcmp(name, PSW))
            is_valid = false;
    }
    else
    {
        /* check if all valid charachters */
        if (str_len < MAX_LABEL_LENGTH && isalpha(name[0]))
            for (i = 1; i <= str_len && isalnum(name[i]); i++)
                ;
        is_valid = str_len == i;
    }

    return is_valid;
}

char *trim(char *str)
{
    static line tmp = {0};
    int count = 0, i, j;
    while (isspace(str[count]))
        count++;

    for (i = count, j = 0; str[i] != '\0'; i++, j++)
    {
        tmp[j] = str[i];
    }
    tmp[j] = '\0';
    for (i = j - 1; i >= 0 && isspace(tmp[i]); i--)
    {
        tmp[i] = '\0';
    }

    return tmp;
}

int isEmptyString(const char *s)
{
    if (s == NULL)
    {
        return 1;
    }
    while (*s != '\0')
    {
        if (!isspace((unsigned char)*s))
            return 0;
        s++;
    }
    return 1;
}