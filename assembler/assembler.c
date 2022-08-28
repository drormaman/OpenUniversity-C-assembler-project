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
            writeObjectFile(argv[i], code_img, IC, DC);
            writeEntryFile(argv[i], symbols_table_root);
            writeExternFile(argv[i], symbols_table_root);
        }
    }

    for (i = 0; i < IC; i++)
    {
        printf("%d\toperation - command: %u\tsourceop: %u\tdestop: %u\ttype: %u\n", i, code_img[i].code.code, code_img[i].code.srcop, code_img[i].code.dstop, code_img[i].code.type);
        printf("%d\tinfo std - value: %d\ttype: %u\n", i, code_img[i].info.std.value, code_img[i].info.std.type);
        printf("%d\tinfo reg - reg1 value: %u\treg2 value: %u\ttype: %u\n", i, code_img[i].info.reg.src, code_img[i].info.reg.dst, code_img[i].info.reg.type);
    }
    for (i = 0; i < DC; i++)
    {
        printf("%d\t%d\n", i + IC, code_img[i + IC].data.value);
    }
    freeSymbols(symbols_table_root);
    return 0;
}