#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "../strings/strings.h"
#include "../assembler/assembler.h"

typedef struct symbol
{
    label name;
    unsigned char address;
    bool isop;
    bool isext;
    struct symbol *next;
} symbol;

symbol *newSymbol(label name, unsigned char address, bool isop, bool isext);
void addSymbol(symbol **root, label name, unsigned char address, bool isop, bool isext);
symbol *isExistingSymbol(symbol *table, label name);
void freeSymbols(symbol *root);
void printSymbols(symbol *root);

#endif