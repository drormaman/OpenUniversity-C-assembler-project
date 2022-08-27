#ifndef METHODS_H
#define METHODS_H

#include "../strings/strings.h"

#define NUMBER_OF_COMMANDS 16
#define COMMAND_NAME_LENGTH 3
#define MOV "mov"
#define CMP "cmp"
#define ADD "add"
#define SUB "sub"
#define NOT "not"
#define CLR "clr"
#define LEA "lea"
#define INC "inc"
#define DEC "dec"
#define JMP "jmp"
#define BNE "bne"
#define GET "get"
#define PRN "prn"
#define JSR "jsr"
#define RTS "rts"
#define HLT "hlt"
#define PSW "PSW"

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
} cmd_code;

typedef enum
{
    absolute,
    external,
    relocateable
} encode_type_value;

typedef enum
{
    none = -1,
    immediate,
    direct,
    struct_access,
    dir_register,
} address_type_value;

typedef enum
{
    no_operands = 0,
    direct_and_struct_access = 6,
    all_no_immediate = 14,
    all = 15,
} valid_address_type;

typedef enum
{
    group3,
    group2,
    group1,
} cmd_args_num;

typedef char cmd_name[COMMAND_NAME_LENGTH + 1];

typedef union
{
    /*Todo maybe not needed */
    struct
    {
        unsigned int immediate : 1;
        unsigned int direct : 1;
        unsigned int struct_access : 1;
        unsigned int dir_register : 1;
    };
    unsigned int total : 4;
} cmd_valid_address_type;

typedef struct
{
    cmd_name name;
    cmd_code code;
    cmd_valid_address_type valid_src;
    cmd_valid_address_type valid_dst;
    cmd_args_num valid_args_num;
} cmd_metadata;

void initCmdList(cmd_metadata *list_ptr);
cmd_metadata *getCmdMetadata(cmd_metadata *list, char *name);
address_type_value getOpearndAddressType(char *operand);
bool isValidOperand(cmd_metadata *cmd_info, address_type_value op_type, bool is_src);

#endif