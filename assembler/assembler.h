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

/* absolute info word */
typedef struct
{
    unsigned int type : 2;
    int value : 8;
} abs_infoword;

/* relocateable info word */
typedef struct
{
    unsigned int type : 2;
    unsigned int value : 8;
} rel_infoword;

/* registers info word */
typedef struct
{
    unsigned int type : 2;
    unsigned int dst : 4;
    unsigned int src : 4;
} reg_infoword;

typedef union infoword
{
    rel_infoword rel;
    abs_infoword abs;
    reg_infoword reg;
} infoword;

typedef struct
{
    int value : 10;
} dataword;

typedef struct bits
{
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 1;
    unsigned int b7 : 1;
    unsigned int b8 : 1;
    unsigned int b9 : 1;
} bits_word;

typedef union word
{
    codeword code;
    infoword info;
    dataword data;
    bits_word bits;
} word;

typedef word words_img[MAX_MEMORY_SIZE];

#endif