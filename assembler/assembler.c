#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../files/files.h"
#include "../firstphase/firstphase.h"
#include "../symbols/symbols.h"
#include "../methods/methods.h"
#include "assembler.h"

int main(int argc, char *argv[])
{
    int i, j;
    words_img code_img, data_img;
    symbol *symbols_table_root = NULL;
    unsigned char IC = 0, DC = 0;
    cmd_metadata cmds_metadata_list[NUMBER_OF_COMMANDS];
    initCmdList(cmds_metadata_list);

    for (i = 1; i < argc; i++)
    {
        preAssembler(argv[i]);
        firstphase(argv[i], code_img, data_img, &IC, &DC, &symbols_table_root, cmds_metadata_list);
    }

    printf("\nDC: %d, IC: %d\n", DC, IC);
    for (i = 0; i < DC; i++)
    {
        printf("%d\t%d\n", i, data_img[i].data.value);
    }
    for (i = 0; i < IC; i++)
    {
        printf("%d\toperation - command: %u\tsourceop: %u\tdestop: %u\ttype: %u\n", i, code_img[i].code.code, code_img[i].code.srcop, code_img[i].code.dstop, code_img[i].code.type);
        printf("%d\tinfo std - value: %d\ttype: %u\n", i, code_img[i].info.std.value, code_img[i].info.std.type);
        printf("%d\tinfo reg - reg1 value: %u\treg2 value: %u\ttype: %u\n", i, code_img[i].info.reg.src, code_img[i].info.reg.dst, code_img[i].info.reg.type);
    }
    printSymbols(symbols_table_root);
    freeSymbols(symbols_table_root);
    return 0;
}
/*printf("after init all\n");*/
/*for (i = 0; i < 16; i++)
{
    /*printf("\n%s\tvalid num: %d, srctypes: %d, desttypes: %d", methods_list[i].name, methods_list[i].valid_operands_num, methods_list[i].valid_source, methods_list[i].valid_dest);
    printf("\n%s\tvalid num: %d, srctypes: ", methods_list[i].name, methods_list[i].valid_operands_num);

    if (methods_list[i].valid_source.bit.immediate)
        printf("\t%d", 0);
    if (methods_list[i].valid_source.bit.direct)
        printf("\t%d", 1);
    if (methods_list[i].valid_source.bit.struct_access)
        printf("\t%d", 2);
    if (methods_list[i].valid_source.bit.dir_register)
        printf("\t%d", 3);
    printf("\tdesttypes:");
    if (methods_list[i].valid_dest.bit.immediate)
        printf("\t%d", 0);
    if (methods_list[i].valid_dest.bit.direct)
        printf("\t%d", 1);
    if (methods_list[i].valid_dest.bit.struct_access)
        printf("\t%d", 2);
    if (methods_list[i].valid_dest.bit.dir_register)
        printf("\t%d", 3);
}*/