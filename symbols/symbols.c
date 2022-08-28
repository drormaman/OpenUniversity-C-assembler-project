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
        tmp->access_address = NULL;
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

void addLocation(symbol **sym, unsigned char address)
{
    location *new_loc;

    new_loc = (location *)malloc(sizeof(location));
    if (new_loc)
    {
        new_loc->address = address;
        new_loc->next = (**sym).access_address;
        (**sym).access_address = new_loc;
    }
}