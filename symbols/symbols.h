#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "../strings/strings.h"
#include "../assembler/assembler.h"

typedef struct Symbol
{
    Label name;
    unsigned char address;
    bool isop;
    bool isext;
    struct Symbol *next;
} Symbol;

Symbol *newSymbol(Label name, unsigned char address, bool isop, bool isext);
void addSymbol(Symbol **root, Label name, unsigned char address, bool isop, bool isext);
Symbol *isExistingSymbol(Symbol *table, Label name);
void freeSymbols(Symbol *root);
void printSymbols(Symbol *root);

#endif