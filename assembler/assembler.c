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
        printf("\n______DEBUG______%s\n", argv[i]);
        preAssembler(argv[i]);
        printf("\n______DEBUG______\n");
        firstphase(argv[i], code_img, data_img, &IC, &DC, symbols_table_root, cmds_metadata_list);
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