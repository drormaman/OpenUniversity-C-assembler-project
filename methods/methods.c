#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "methods.h"
#include "../strings/strings.h"

void initCmdList(cmd_metadata *list)
{
    cmd_code cmd_codes[NUMBER_OF_COMMANDS] = {mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, get, prn, jsr, rts, hlt};
    cmd_name cmd_names[NUMBER_OF_COMMANDS] = {MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, GET, PRN, JSR, RTS, HLT};
    valid_address_type cmd_valid_src_address_types[NUMBER_OF_COMMANDS] = {all, all, all, all, no_operands, no_operands, direct_and_struct_access, no_operands, no_operands, no_operands, no_operands, no_operands, no_operands, no_operands, no_operands, no_operands};
    valid_address_type cmd_valid_dst_address_types[NUMBER_OF_COMMANDS] = {all_no_immediate, all, all_no_immediate, all_no_immediate, all_no_immediate, all_no_immediate, all_no_immediate, all_no_immediate, all_no_immediate, all_no_immediate, all_no_immediate, all_no_immediate, all, all_no_immediate, no_operands, no_operands};
    cmd_args_num cmd_valid_args_group[NUMBER_OF_COMMANDS] = {group1, group1, group1, group1, group2, group2, group1, group2, group2, group2, group2, group2, group2, group2, group3, group3};
    int i;
    /*cmd_metadata *list = *list_root;*/

    for (i = 0; i < NUMBER_OF_COMMANDS; i++)
    {
        list[i].code = cmd_codes[i];
        strcpy(list[i].name, cmd_names[i]);
        list[i].valid_src.total = cmd_valid_src_address_types[i];
        list[i].valid_dst.total = cmd_valid_dst_address_types[i];
        list[i].valid_args_num = cmd_valid_args_group[i];
    }
}

cmd_metadata *getCmdMetadata(cmd_metadata *list, char *name)
{
    int i;
    cmd_metadata *result_metadata = NULL;

    for (i = 0; i < NUMBER_OF_COMMANDS; i++)
    {
        if (!strcmp(name, list[i].name))
        {
            result_metadata = &list[i];
        }
    }
    return result_metadata;
}

address_type_value getOperandAddressType(char *operand)
{
    address_type_value result;
    if (strlen(operand) == 0)
        result = none;
    else if (operand[0] == '#')
        result = immediate;
    else if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7')
        result = dir_register;
    else if (strstr(operand, ".1") || strstr(operand, ".2"))
        result = struct_access;
    else
        result = direct;
    return result;
}

bool isValidOperand(cmd_metadata *cmd_info, address_type_value operand_type, bool is_src)
{
    int i;
    bool res;

    if (operand_type == none)
        res = false;
    else if (is_src)
        res = (*cmd_info).valid_src.total >> operand_type & 1;
    else
        res = (*cmd_info).valid_dst.total >> operand_type & 1;

    return res;
}