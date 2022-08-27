#ifndef STRINGS_H
#define STRINGS_H

#define MAX_LABEL_LENGTH 31
#define MAX_LINE_LENGTH 82
#define SPACE ' '
#define SPACE_STR " "
#define DOT '.'
#define COMMA ','
#define COMMA_STR ","
#define DBL_QUOTES '"'
#define COMMENT_SIGN ';'
#define LABEL_SIGN ':'

typedef char label[MAX_LABEL_LENGTH];
typedef char line[MAX_LINE_LENGTH];

typedef enum
{
    false,
    true
} bool;

bool isValidLabel(label name);
char *trim(char *str);
int isEmptyString(const char *s);

#endif