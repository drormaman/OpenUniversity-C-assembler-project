#ifndef METHODS_H
#define METHODS_H

#define NUMBER_OF_OPERATIONS 16
#define OPERATIONS_NAME_LENGTH 4
#define MAX_NUMBER_OF_ADDRESSING_METHODS 4
#include "../strings/strings.h"

typedef enum
{
    absolute,
    external,
    relocateable
} type_values;

typedef enum
{
    none = -1,
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

typedef char method_name[3];
typedef short valid_addressing_values[4];
typedef struct
{
    unsigned int immediate : 1;
    unsigned int direct : 1;
    unsigned int struct_access : 1;
    unsigned int dir_register : 1;
} valid_addressing_methods_bit;
typedef struct
{
    unsigned int all : 4;
} valid_addressing_methods_all;

typedef union valid_addressing_methods
{
    valid_addressing_methods_bit bit;
    valid_addressing_methods_all all;
} valid_addressing_methods;

typedef struct
{
    method_name name;
    short code;
    valid_addressing_methods valid_source;
    valid_addressing_methods valid_dest;
    short valid_operands_num;
} op_metadata;

void initOpMetadata(op_metadata **list_ptr);
op_metadata *findOpMetadata(op_metadata *list, char *op_name);
addressing_values getAddressingType(char *operand);
bool isValidOperand(op_metadata *cmd_info, addressing_values op_type, bool is_src);

#endif