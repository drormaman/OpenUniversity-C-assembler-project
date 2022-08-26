#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../preassembler/preassembler.h"

typedef struct
{
    unsigned int type : 2;
    unsigned int destop : 2;
    unsigned int sourceop : 2;
    unsigned int opcode : 4;
} codeword;

typedef struct
{
    unsigned int type : 2;
    int value : 8;
} std_infoword;

typedef struct
{
    unsigned int type : 2;
    unsigned int dest_reg : 4;
    unsigned int src_reg : 4;
} reg_infoword;

typedef union infoword
{
    std_infoword std;
    reg_infoword reg;
} infoword;

typedef struct
{
    int value : 10;
} dataword;

typedef union word
{
    codeword code;
    infoword info;
    dataword data;
} word;

#endif