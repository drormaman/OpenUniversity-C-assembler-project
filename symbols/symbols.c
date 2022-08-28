#include <stdlib.h>
#include <string.h>
#include "symbols.h"

symbol *newSymbol(label name, unsigned char address, bool isop, bool isext)
{
    symbol *tmp = (symbol *)malloc(sizeof(symbol));
    if (tmp)
    {
        strcpy(tmp->name, name);
        tmp->address = address;
        tmp->isop = isop;
        tmp->isext = isext;
        tmp->isent = false;
        tmp->next = NULL;
    }
    return tmp;
}

void addSymbol(symbol **root, label name, unsigned char address, bool isop, bool isext)
{
    symbol *tmp, *new_symbol;
    new_symbol = newSymbol(name, address, isop, isext);
    if (new_symbol)
    {
        if (*root == NULL)
            *root = new_symbol;
        else
        {
            for (tmp = *root; tmp->next != NULL; tmp = tmp->next)
                ;
            tmp->next = new_symbol;
        }
    }
    return;
}

symbol *isExistingSymbol(symbol *table, label name)
{
    symbol *tmp;
    for (tmp = table; tmp != NULL && strcmp(tmp->name, name); tmp = tmp->next)
        ;
    return tmp;
}

void freeSymbols(symbol *root)
{
    symbol *tmp;
    while (root != NULL)
    {
        tmp = root;
        root = root->next;
        free(tmp);
    }
}

void printSymbols(symbol *root)
{
    symbol *curr;
    printf("-----------------------------\n");
    for (curr = root; curr != NULL; curr = curr->next)
    {
        printf("name: %s, address: %u, op: %d, ext: %d, ent: %d\n", curr->name, curr->address, curr->isop, curr->isext, curr->isent);
    }
    printf("-----------------------------\n");
}