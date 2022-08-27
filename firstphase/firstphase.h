#ifndef FIRSTPHASE_H
#define FIRSTPHASE_H

#include "../assembler/assembler.h"
#include "../symbols/symbols.h"
#include "../methods/methods.h"

#define DATA ".data"
#define STRUCT ".struct"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERN ".extern"

int firstphase(char *filename, words_img code_img, words_img data_img, unsigned char *IC, unsigned char *DC, symbol **symbols_table_root, const cmd_metadata *cmds_metadata_list);

#endif