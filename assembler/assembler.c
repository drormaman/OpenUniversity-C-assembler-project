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
    word code_img[255];
    word data_img[255];
    Symbol *symbol_table = NULL;

    op_metadata *methods_list = (op_metadata *)calloc(NUMBER_OF_OPERATIONS, sizeof(op_metadata));
    /*methods_list = initOpMetadata(methods_list);*/
    initOpMetadata(&methods_list);

    /*printf("after init all\n");*/
    for (i = 0; i < 16; i++)
    {
        /*printf("\n%s\tvalid num: %d, srctypes: %d, desttypes: %d", methods_list[i].name, methods_list[i].valid_operands_num, methods_list[i].valid_source, methods_list[i].valid_dest);*/
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
    }
    unsigned char IC = 0, DC = 0;

    for (i = 1; i < argc; i++)
    {
        preAssembler(argv[i]);
        firstAssemblerMain(argv[i], code_img, data_img, &IC, &DC, symbol_table, methods_list);
    }

    printSymbols(symbol_table);
    free(methods_list);
    freeSymbols(symbol_table);
    return 0;
}