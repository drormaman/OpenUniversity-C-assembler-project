#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "../strings/strings.h"
#include "../assembler/assembler.h"

typedef struct location
{
    unsigned char address;
    struct location *next;
} location;

typedef struct symbol
{
    label name;
    unsigned char address;
    bool isop;
    bool isext;
    bool isent;
    location *access_address;
    struct symbol *next;
} symbol;

symbol *newSymbol(label name, unsigned char address, bool isop, bool isext);
void addSymbol(symbol **root, label name, unsigned char address, bool isop, bool isext);
symbol *isExistingSymbol(symbol *table, label name);
void addLocation(symbol **sym, unsigned char address);
void freeSymbols(symbol *root);

#endif