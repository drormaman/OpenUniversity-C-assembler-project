#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../preassembler/preassembler.h"

typedef enum
{
    absolute,
    external,
    relocateable
} type_values;

typedef enum
{
    immediate,
    direct,
    struct_access,
    dir_register
} addressing_values;

typedef enum
{
    mov,
    cmp,
    add,
    sub,
    not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    get,
    prn,
    jsr,
    rts,
    hlt
} opcode_values;

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
    unsigned int value : 8;
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