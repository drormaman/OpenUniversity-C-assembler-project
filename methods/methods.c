#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "methods.h"
#include "../strings/strings.h"

void initOpMetadata(op_metadata **list_ptr)
{
    op_metadata *list = *list_ptr;
    short op_codes[NUMBER_OF_OPERATIONS] = {mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, get, prn, jsr, rts, hlt};
    char op_names[NUMBER_OF_OPERATIONS][OPERATIONS_NAME_LENGTH] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};
    unsigned int op_valid_source_addressing_methods[NUMBER_OF_OPERATIONS] = {15, 15, 15, 15, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int op_valid_dest_addressing_methods[NUMBER_OF_OPERATIONS] = {14, 15, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 14, 0, 0};
    short op_valid_operands_num[NUMBER_OF_OPERATIONS] = {2, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0};

    int i;

    for (i = 0; i < NUMBER_OF_OPERATIONS; i++)
    {
        list[i].code = op_codes[i];
        strcpy(list[i].name, op_names[i]);
        list[i].valid_source.all.all = op_valid_source_addressing_methods[i];
        list[i].valid_dest.all.all = op_valid_dest_addressing_methods[i];
        /*memcpy(list[i].valid_source, op_valid_source_addressing_methods[i], sizeof(valid_addressing_values));
        memcpy(list[i].valid_dest, op_valid_dest_addressing_methods[i], sizeof(valid_addressing_values));*/
        list[i].valid_operands_num = op_valid_operands_num[i];
    }
}

op_metadata *findOpMetadata(op_metadata *list, char *op_name)
{

    int i;
    op_metadata *result_metadata = NULL;

    for (i = 0; i < NUMBER_OF_OPERATIONS; i++)
    {
        if (!strcmp(op_name, list[i].name))
        {
            result_metadata = &list[i];
        }
    }
    if (result_metadata == NULL)
    {
        printf("Error: invalid command name");
    }
    return result_metadata;
}

addressing_values getAddressingType(char *operand)
{
    /*// starts with # - immediate
    // label - direct
    // struct_access - struct label with . and 1 or 2
    // register - r0, r1, r2, r3, r4, r5, r6, r7*/
    addressing_values result;
    if (strlen(operand) == 0)
    {
        result = none;
    }
    else if (operand[0] == '#')
    {
        result = immediate;
    }
    else if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7')
    {
        result = dir_register;
    }
    else if (strstr(operand, ".1") || strstr(operand, ".2"))
    {
        result = struct_access;
    }
    else
    {
        result = direct;
    }
    return result;
}

bool isValidOperand(op_metadata *cmd_info, addressing_values op_type, bool is_src)
{
    int i;
    int res;
    switch (op_type)
    {
    case immediate:
        res = is_src ? (cmd_info)->valid_source.bit.immediate : (cmd_info)->valid_dest.bit.immediate;
        break;
    case direct:
        res = is_src ? (cmd_info)->valid_source.bit.direct : (cmd_info)->valid_dest.bit.direct;
        break;
    case struct_access:
        res = is_src ? (cmd_info)->valid_source.bit.struct_access : (cmd_info)->valid_dest.bit.struct_access;
        break;
    case dir_register:
        res = is_src ? (cmd_info)->valid_source.bit.dir_register : (cmd_info)->valid_dest.bit.dir_register;
        break;
    case none:
    default:
        res = 0;
    }

    return res ? true : false;
}