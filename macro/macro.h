#ifndef MACRO_H
#define MACRO_H

#include "../files/files.h"
#include "../strings/strings.h"

#define MAX_MACRO_LINES 20

typedef struct macro
{
    label name;
    char text[MAX_MACRO_LINES * MAX_LINE_LENGTH];
    struct macro *next;
} macro;

void addMacro(macro **root, label name);
void loopMacro(macro *root);
macro *isExistingMacro(macro *root, char *line);
void freeMacros(macro *root);

#endif