#ifndef FIRSTPHASE_H
#define FIRSTPHASE_H

#include "../assembler/assembler.h"
#include "../symbols/symbols.h"
int firstAssemblerMain(char *filename, word code_img[255], word data_img[255], unsigned char *IC, unsigned char *DC, Symbol *symbol_table);

#endif