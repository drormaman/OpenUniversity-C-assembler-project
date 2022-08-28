#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include "../assembler/assembler.h"
typedef char base32_word[3];
#define BASE 32
#define MEM_SKIP 100

void writeObjectFile(char *filename, words_img code_img, unsigned char IC, unsigned char DC);
void writeEntryFile(char *filename, symbol *symbol_table);
void writeExternFile(char *filename, symbol *symbol_table);

#endif