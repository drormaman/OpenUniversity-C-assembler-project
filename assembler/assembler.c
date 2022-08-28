#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../files/files.h"
#include "../encode/encode.h"
#include "../symbols/symbols.h"
#include "../methods/methods.h"
#include "../output/output.h"
#include "assembler.h"

int main(int argc, char *argv[])
{
    int i;
    words_img code_img, data_img;
    bool is_error = false;
    symbol *symbols_table_root;
    unsigned char IC, DC;
    cmd_metadata cmds_metadata_list[NUMBER_OF_COMMANDS];
    initCmdList(cmds_metadata_list);

    for (i = 1; i < argc; i++)
    {
        IC = 0;
        DC = 0;
        symbols_table_root = NULL;
        preAssembler(argv[i]);
        is_error = encodeImg(argv[i], code_img, data_img, &IC, &DC, &symbols_table_root, cmds_metadata_list);
        if (!is_error)
        {
            writeObjectFile(argv[i], &code_img, IC, DC);
            writeEntryFile(argv[i], symbols_table_root);
            writeExternFile(argv[i], symbols_table_root);
        }
    }
    freeSymbols(symbols_table_root);
    return 0;
}