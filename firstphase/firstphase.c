#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../strings/strings.h"
#include "../files/files.h"
#include "firstphase.h"
#include "../symbols/symbols.h"
#include "../assembler/assembler.h"
#include "../methods/methods.h"

#define DATA ".data"
#define STRUCT ".struct"
#define STRING ".string"
#define ENTRY ".entry"
#define EXTERN ".extern"

int encodeDataline(word data_img[255], char *line, unsigned char *DC, Symbol **symbol_table);
int encodeOpline(word code_img[255], char *line, unsigned char *IC, Symbol **symbol_table, const op_metadata *methods_list);

int firstAssemblerMain(char *filename, word code_img[255], word data_img[255], unsigned char *IC, unsigned char *DC, Symbol *symbol_table, const op_metadata *methods_list)
{
    FILE *fd;
    Line full_line, rest_line;
    bool in_symbol = false;
    Label symbol_name;
    Label curr_word;
    int res, i;

    fd = openReadFile(filename, ".am");
    while (fgets(full_line, 80, fd) != NULL)
    {
        /* check if line is not empty or a comment */
        if (!isEmptyString(full_line) || full_line[0] == ';')
        {
            /* check if line starts with a label - step 3 */
            /* printf("first %d", strstr(full_line, ":")); */
            if (strstr(full_line, ":"))
            {
                strcpy(symbol_name, strtok(full_line, ":"));
                if (isValidLabel(symbol_name))
                {
                    in_symbol = true; /* step 4 */
                }
                else
                {
                    printf("Error: Bad label name\n");
                }
            }

            strcpy(rest_line, trimLeft(strtok(in_symbol ? NULL : full_line, "\n")));
            if (rest_line[0] == '.')
            {
                /*
                    add to symbol (name, curr DC, isop = false, isext = 0, next = NULL)
                    if symbol already found - print error
                    encode the data in data_img and update DC
                */
                if (in_symbol)
                {

                    if (!isExistingSymbol(symbol_table, symbol_name))
                    {
                        addSymbol(&symbol_table, symbol_name, *DC, false, false);
                    }
                    else
                    {
                        printf("Error: symbol already existing\n");
                    }
                }
                encodeDataline(data_img, rest_line, DC, &symbol_table);
            }
            else
            {
                /*printf("\n%s\t%s", in_symbol ? symbol_name : "", rest_line);*/
                /*
                    add to symbol (name, curr IC, isop = true, isext = 0, next = NULL)
                    if symbol already found - print error
                    encode the data in data_img and update DC
                */
                if (in_symbol)
                {

                    if (!isExistingSymbol(symbol_table, symbol_name))
                    {
                        addSymbol(&symbol_table, symbol_name, *IC, true, false);
                    }
                    else
                    {
                        printf("Error: symbol already existing\n");
                    }
                }
                encodeOpline(code_img, rest_line, IC, &symbol_table, methods_list);
            }

            in_symbol = false;
            symbol_name[0] = 0;
        }
    }
    printf("\nDC: %d, IC: %d\n", *DC, *IC);
    for (i = 0; i < *DC; i++)
    {
        printf("%d\t%d\n", i, data_img[i].data.value);
    }
    for (i = 0; i < *IC; i++)
    {
        printf("%d\toperation - command: %u\tsourceop: %u\tdestop: %u\ttype: %u\n", i, code_img[i].code.opcode, code_img[i].code.sourceop, code_img[i].code.destop, code_img[i].code.type);
        printf("%d\tinfo std - value: %d\ttype: %u\n", i, code_img[i].info.std.value, code_img[i].info.std.type);
        printf("%d\tinfo reg - reg1 value: %u\treg2 value: %u\ttype: %u\n", i, code_img[i].info.reg.src_reg, code_img[i].info.reg.dest_reg, code_img[i].info.reg.type);
    }
    printSymbols(symbol_table);
    return 1;
}

int encodeOperands(word code_img[255], unsigned char *IC, op_metadata *cmd_info, int operands_num, char *operand1, char *operand2)
{

    addressing_values operand1_type, operand2_type;
    operand1_type = getAddressingType(operand1);
    operand2_type = getAddressingType(operand2);
    bool is_op1_valid, is_op2_valid;
    switch (operands_num)
    {
    case 2:
        code_img[*IC].code.sourceop = operand1_type == none ? 0 : operand1_type;
        code_img[*IC].code.destop = operand2_type == none ? 0 : operand2_type;
        (*IC)++;
        break;
    case 1:
        code_img[*IC].code.sourceop = 0;
        code_img[*IC].code.destop = operand1_type == none ? 0 : operand1_type;
        (*IC)++;
        break;
    case 0:
        code_img[*IC].code.sourceop = 0;
        code_img[*IC].code.destop = 0;
        break;
    }
    printf("\ntype op1: '%d' --- op2: '%d'", operand1_type, operand2_type);
    switch (operands_num)
    {
    case 0:
    {
        break;
    }
    case 1:
    {
        is_op1_valid = isValidOperand(cmd_info, operand1_type, false);
        if (is_op1_valid)
        {

            switch (operand1_type)
            {
            case immediate:
                code_img[*IC].info.std.value = atoi(operand1 + 1);
                code_img[*IC].info.std.type = absolute;
                break;
            case direct:
                code_img[*IC].info.std.value = 0;
                code_img[*IC].info.std.type = relocateable;
                break;
            case struct_access:
                code_img[*IC].info.std.value = 0;
                code_img[*IC].info.std.type = relocateable;
                (*IC)++;
                code_img[*IC].info.std.value = atoi((strstr(operand1, ".") + 1));
                code_img[*IC].info.std.type = absolute;
                break;
            case dir_register:
                code_img[*IC].info.reg.dest_reg = atoi(operand1 + 1);
                code_img[*IC].info.std.type = absolute;
                break;
            }
        }
        else
        {
            /* Error - destnationion operand type is invalid */
            printf("\ndest error @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
        }
        break;
    }
    case 2:
    {
        is_op1_valid = isValidOperand(cmd_info, operand1_type, true);
        is_op2_valid = isValidOperand(cmd_info, operand2_type, false);
        if (is_op1_valid)
        {

            switch (operand1_type)
            {
            case immediate:
                code_img[*IC].info.std.value = atoi(operand1 + 1);
                code_img[*IC].info.std.type = absolute;
                break;
            case direct:
                code_img[*IC].info.std.value = 0;
                code_img[*IC].info.std.type = relocateable;
                break;
            case struct_access:
                code_img[*IC].info.std.value = 0;
                code_img[*IC].info.std.type = relocateable;
                (*IC)++;
                code_img[*IC].info.std.value = atoi((strstr(operand1, ".") + 1));
                code_img[*IC].info.std.type = absolute;
                break;
            case dir_register:
                code_img[*IC].info.reg.src_reg = atoi(operand1 + 1);
                code_img[*IC].info.reg.dest_reg = 0;
                code_img[*IC].info.std.type = absolute;
                break;
            }
        }
        else
        {
            /* Error - source operand type is invalid */
            printf("\nsource error @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
        }
        (*IC)++;
        if (is_op2_valid)
        {
            switch (operand2_type)
            {
            case immediate:
                code_img[*IC].info.std.value = atoi(operand1 + 1);
                code_img[*IC].info.std.type = absolute;
                break;
            case direct:
                code_img[*IC].info.std.value = 0;
                code_img[*IC].info.std.type = relocateable;
                break;
            case struct_access:
                code_img[*IC].info.std.value = 0;
                code_img[*IC].info.std.type = relocateable;
                (*IC)++;
                code_img[*IC].info.std.value = atoi((strstr(operand1, ".") + 1));
                code_img[*IC].info.std.type = absolute;
                break;
            case dir_register:
                if (operand1_type == dir_register)
                {
                    code_img[--(*IC)].info.reg.dest_reg = atoi(operand2 + 1);
                }
                else
                {
                    code_img[*IC].info.reg.src_reg = 0;
                    code_img[*IC].info.reg.dest_reg = atoi(operand2 + 1);
                    code_img[*IC].info.std.type = absolute;
                }
                break;
            }
        }
        else
        {
            /* Error - destination operand type is invalid */
            printf("\ndest error @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
        }
        break;
    }
    default:
        printf("\nhere default");
        break;
    }
    return 0;
}

int encodeOpline(word code_img[255], char *line, unsigned char *IC, Symbol **symbol_table, const op_metadata *methods_list)
{

    int cmd_len, operands = 0, line_len; /* == L */
    Label cmd;
    unsigned short res;
    Line operand1, operand2;
    op_metadata *cmd_info;
    strncpy(cmd, line, 3);
    printf("\n_______\n`IC: %d --- Line: %s --- CMD: %s", *IC, line, cmd);
    cmd_info = findOpMetadata(methods_list, cmd);

    if (strlen(line) > strlen(cmd))
    {
        operands = sscanf(line + 4, "%[^ ,\t] ,%s", operand1, operand2);
        printf("\n%d -- op1: '%s' --- op2: '%s'", operands, operand1, operand2);
    }
    cmd_info = findOpMetadata(methods_list, cmd);
    if (cmd_info->valid_operands_num == operands)
    {
        code_img[*IC].code.opcode = cmd_info->code;
        code_img[*IC].code.type = absolute;
        encodeOperands(code_img, IC, cmd_info, operands, operand1, operand2);
        (*IC)++;
    }
    else
    {
        /* Error - invalid number of operands */
    }

    printf("\nINFO: name: %s, operands_num: %d", cmd_info->name, cmd_info->valid_operands_num);

    return res;
}

int encodeData(char *vars, word *data_img, unsigned char *DC)
{
    int number = 0;
    char *temp;
    temp = strtok(vars, ",");
    while (temp != NULL)
    {
        number = atoi(temp);
        (data_img + (*DC)++)->data.value = number;
        temp = strtok(NULL, ",");
    }
    return *DC;
}

int encodeString(char *vars, word *data_img, unsigned char *DC)
{
    char *start, *end;
    start = strstr(vars, "\"");
    end = strstr(++start, "\"");
    while (start < end)
    {
        (data_img + (*DC)++)->data.value = (*start);
        start++;
    }
    (data_img + (*DC)++)->data.value = 0;
    return *DC;
}

int encodeStruct(char *vars, word *data_img, unsigned char *DC)
{
    int number = 0;
    char *tmp;
    tmp = strtok(vars, ",");
    number = atoi(tmp);
    (data_img + (*DC)++)->data.value = number;
    tmp = strtok(NULL, ",");
    encodeString(tmp, data_img, DC);

    return *DC;
}

int encodeExtern(char *vars, Symbol **symbol_table)
{
    Label name;
    strcpy(name, trimLeft(strtok(vars, "")));
    if (!isExistingSymbol(*symbol_table, name))
    {
        addSymbol(symbol_table, name, 0, false, true);
    }
    else
    {
        printf("Error: symbol already existing\n");
    }

    return 1;
}

int encodeDataline(word data_img[255], char *line, unsigned char *DC, Symbol **symbol_table)
{
    Label type;
    unsigned short res;
    Line vars;

    strcpy(type, strtok(line, " "));
    strcpy(vars, strtok(NULL, ""));
    if (!strcmp(type, DATA))
        res = encodeData(vars, data_img, DC);
    else if (!strcmp(type, STRUCT))
        res = encodeStruct(vars, data_img, DC);
    else if (!strcmp(type, STRING))
        res = encodeString(vars, data_img, DC);
    else if (!strcmp(type, EXTERN))
        res = encodeExtern(vars, symbol_table);
    else
    {
        if (strcmp(type, ENTRY))
        {
            printf("Error: Invalid data line\n");
            res = 0;
        }
    }

    return res;
}