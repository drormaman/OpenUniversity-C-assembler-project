#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "../strings/strings.h"

Macro *newMacro(Label name)
{
    Macro *tmp = (Macro *)malloc(sizeof(Macro));
    if (tmp)
    {
        strcpy(tmp->name, name);
        tmp->next = NULL;
    }
    return tmp;
}

void addMacro(Macro **root, Label name)
{
    Macro *tmp;
    Macro *new_macro = newMacro(name);
    if (new_macro)
    {
        tmp = *root;
        *root = new_macro;
        (*new_macro).next = tmp;
        /*
        if (*root == NULL)
        {
            *root = new_macro;
        }
        else
        {
            for (curr = *root; curr->next != NULL; curr = curr->next)
                ;
            curr->next = new_macro;
        }*/
    }
}

void loopMacro(Macro *root)
{
    Macro *curr;
    for (curr = root; curr != NULL; curr = curr->next)
    {
        printf("%s\t%s", curr->name, curr->text);
    }
}

Macro *isExistingMacro(Macro *root, char *line)
{
    Macro *curr;
    char name[MAX_MACRO_NAME_LENGTH];
    sscanf(line, "%s", name);
    for (curr = root; curr != NULL && strcmp(curr->name, name); curr = curr->next)
        ;
    return curr;
}

void freeMacros(Macro *root)
{
    Macro *tmp;
    while (root != NULL)
    {
        tmp = root;
        root = root->next;
        free(tmp);
    }
}