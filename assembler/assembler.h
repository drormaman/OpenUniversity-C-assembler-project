#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../preassembler/preassembler.h"

#define MAX_MEMORY_SIZE 255

typedef struct
{
    unsigned int type : 2;
    unsigned int dstop : 2;
    unsigned int srcop : 2;
    unsigned int code : 4;
} codeword;

/* standard info word */
typedef struct
{
    unsigned int type : 2;
    int value : 8;
} std_infoword;

/* registers info word */
typedef struct
{
    unsigned int type : 2;
    unsigned int dst : 4;
    unsigned int src : 4;
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

typedef word words_img[MAX_MEMORY_SIZE];

#endif