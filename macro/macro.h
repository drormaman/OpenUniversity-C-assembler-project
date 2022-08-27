#ifndef MACRO_H
#define MACRO_H

#include "../files/files.h"
#include "../strings/strings.h"

#define MAX_MACRO_NAME_LENGTH 31
#define MAX_MACRO_LINES 10

typedef struct Macro
{
    Label name;
    char text[MAX_MACRO_LINES * MAX_LINE_LENGTH];
    struct Macro *next;
} Macro;

void addMacro(Macro **root, Label name);
void loopMacro(Macro *root);
Macro *isExistingMacro(Macro *root, char *line);
void freeMacros(Macro *root);

#endif