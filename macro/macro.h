#ifndef MACRO_H
#define MACRO_H

#include "../files/files.h"

#define MAX_MACRO_NAME_LENGTH 31
#define MAX_MACRO_LINES 10

typedef struct Macro
{
    char name[MAX_MACRO_NAME_LENGTH];
    char text[MAX_MACRO_LINES * MAX_LINE_LENGTH];
    struct Macro *next;
} Macro;

Macro *newMacro(char *name, char *text);
void addMacro(Macro **root, char *name, char *text);
void loopMacro(Macro *root);
Macro *isExistingMacro(Macro *root, char *line);
void freeMacros(Macro *root);

#endif