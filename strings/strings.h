#ifndef STRINGS_H
#define STRINGS_H

#define MAX_LABEL_LENGTH 30

typedef char Label[MAX_LABEL_LENGTH + 1];
typedef enum
{
    false,
    true
} bool;
bool isValidLabel(Label name);
char *trimLeft(char *str);
int isEmptyString(const char *s);

#endif