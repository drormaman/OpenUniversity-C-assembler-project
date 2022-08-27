#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "../strings/strings.h"

macro *newMacro(label name)
{
    /* todo check if valid label name */
    macro *tmp = (macro *)malloc(sizeof(macro));
    if (tmp)
    {
        strcpy(tmp->name, name);
        tmp->next = NULL;
    }
    return tmp;
}

void addMacro(macro **root, label name)
{
    macro *tmp;
    macro *new_macro = newMacro(name);
    if (new_macro)
    {
        tmp = *root;
        *root = new_macro;
        (*new_macro).next = tmp;
    }
}

void loopMacro(macro *root)
{
    macro *curr;
    for (curr = root; curr != NULL; curr = curr->next)
    {
        printf("%s\t%s", curr->name, curr->text);
    }
}

macro *isExistingMacro(macro *root, char *line)
{
    macro *curr;
    char name[MAX_LABEL_LENGTH];
    sscanf(line, "%s", name);
    for (curr = root; curr != NULL && strcmp(curr->name, name); curr = curr->next)
        ;
    return curr;
}

void freeMacros(macro *root)
{
    macro *tmp;
    while (root != NULL)
    {
        tmp = root;
        root = root->next;
        free(tmp);
    }
}