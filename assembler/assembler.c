#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../files/files.h"
#include "../firstphase/firstphase.h"
#include "../symbols/symbols.h"
#include "assembler.h"

int main(int argc, char *argv[])
{
    int i;
    word code_img[255];
    word data_img[255];
    Symbol *symbol_table = NULL;
    unsigned char IC = 0, DC = 0;

    for (i = 1; i < argc; i++)
    {
        preAssembler(argv[i]);
        firstAssemblerMain(argv[i], code_img, data_img, &IC, &DC, symbol_table);
    }

    printSymbols(symbol_table);
    freeSymbols(symbol_table);
    return 0;
}