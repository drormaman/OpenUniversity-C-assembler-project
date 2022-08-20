#include <stdlib.h>
#include <string.h>
#include "symbols.h"

Symbol *newSymbol(Label name, unsigned char address, bool isop, bool isext)
{
    Symbol *tmp = (Symbol *)malloc(sizeof(Symbol));
    if (tmp)
    {
        strcpy(tmp->name, name);
        tmp->address = address;
        tmp->isop = isop;
        tmp->isext = isext;
        tmp->next = NULL;
    }
    return tmp;
}

void addSymbol(Symbol **root, Label name, unsigned char address, bool isop, bool isext)
{
    Symbol *curr;
    Symbol *new_symbol = newSymbol(name, address, isop, isext);
    printf("Symbol Address %d --- %d\n", root, *root);
    printf("new symbol  %d \n", new_symbol);
    if (new_symbol)
    {
        if (*root == NULL)
        {
            *root = new_symbol;
        }
        else
        {
            for (curr = *root; curr->next != NULL; curr = curr->next)
                ;
            curr->next = new_symbol;
        }
    }
    return;
}

Symbol *isExistingSymbol(Symbol *table, Label name)
{
    Symbol *curr;
    for (curr = table; curr != NULL && strcmp(curr->name, name); curr = curr->next)
        ;
    return curr;
}

void freeSymbols(Symbol *root)
{
    Symbol *tmp;
    while (root != NULL)
    {
        tmp = root;
        root = root->next;
        free(tmp);
    }
}

void printSymbols(Symbol *root)
{
    Symbol *curr;
    printf("-----------------------------\n");
    for (curr = root; curr != NULL; curr = curr->next)
    {
        printf("name: %s, address: %u, op: %d, extern: %d\n", curr->name, curr->address, curr->isop, curr->isext);
    }
    printf("-----------------------------\n");
}