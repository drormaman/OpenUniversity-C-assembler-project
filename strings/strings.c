#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "../files/files.h"
#include "strings.h"

/* check if a string is a valid label name
    max length 30
    1 letter alphanumeric
    all other letter alphanumeric || numbers */
bool isValidLabel(Label name)
{
    int i;
    size_t str_len;

    str_len = strlen(name);

    if (str_len <= MAX_LABEL_LENGTH && isalpha(name[0]))
        for (i = 1; i <= str_len && isalnum(name[i]); i++)
            ;

    return i == str_len ? true : false;
}

char *trimLeft(char *str)
{
    static Line tmp = {0};
    int count = 0, i, j;
    while (isspace(str[count]))
        count++;

    for (i = count, j = 0; str[i] != '\0'; i++, j++)
    {
        tmp[j] = str[i];
    }
    tmp[j] = '\0';

    return tmp;
}

int isEmptyString(const char *s)
{
    while (*s != '\0')
    {
        if (!isspace((unsigned char)*s))
            return 0;
        s++;
    }
    return 1;
}