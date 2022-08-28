#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../strings/strings.h"
#include "../files/files.h"
#include "firstphase.h"
#include "../symbols/symbols.h"
#include "../assembler/assembler.h"
#include "../methods/methods.h"

int encodeDataline(words_img data_img, char *type, char *line, unsigned char DC);
int encodeCmd(words_img code_img, char *line, unsigned char IC, const cmd_metadata *cmds_metadata_list);
int encodeMissingInfo(words_img code_img, char *line, unsigned char IC, const cmd_metadata *cmds_metadata_list, symbol *symbols_table_root);

int firstphase1(FILE *fd, words_img code_img, words_img data_img, unsigned char *IC, unsigned char *DC, symbol **symbols_table_root, const cmd_metadata *cmds_metadata_list)
{
    line full_line, trimed_line;
    bool in_symbol = false, is_empty_line = false, ignore_line = false;
    label symbol_name;
    char *cp, *rest_line, *tmp_word;
    int tmp;

    while (fgets(full_line, MAX_LINE_LENGTH, fd) != NULL)
    {
        is_empty_line = isEmptyString(full_line);
        if (!is_empty_line)
            strcpy(trimed_line, trim(full_line));

        /* check if line is not empty or a comment */
        ignore_line = is_empty_line || (!is_empty_line && trimed_line[0] == COMMENT_SIGN);

        if (!ignore_line)
        {
            /* check if line starts with a label - step 3 */
            if ((cp = strchr(trimed_line, LABEL_SIGN)))
            {
                tmp = (cp - trimed_line);
                strncpy(symbol_name, trimed_line, tmp);
                symbol_name[tmp] = NULL;
                if (isValidLabel(symbol_name))
                    if (!isExistingSymbol(*symbols_table_root, symbol_name))
                        in_symbol = true;
                    else
                        printf("Error: label already exists\n");
                else
                    printf("Error: invalid label name\n");
            }

            rest_line = in_symbol ? trim(cp + 1) : trimed_line;
            if (rest_line[0] == DOT)
            {
                cp = strchr(rest_line, SPACE);
                tmp = (cp - rest_line);

                if (!strncmp(rest_line, EXTERN, tmp))
                {
                    tmp_word = strtok(rest_line + tmp, SPACE_STR);
                    while (tmp_word != NULL)
                    {

                        if (isValidLabel(tmp_word))
                            if (!isExistingSymbol(*symbols_table_root, tmp_word))
                                addSymbol(symbols_table_root, tmp_word, 0, false, true);
                            else
                                printf("Error: label already exists\n");
                        else
                            printf("Error: invalid label name\n");
                        tmp_word = strtok(NULL, SPACE_STR);
                    }
                }
                else if (strncmp(rest_line, ENTRY, tmp) != 0)
                {
                    /*
                        add to symbol (name, curr DC, isop = false, isext = false, next = NULL)
                        if symbol already found - print error
                        encode the data in data_img and update DC
                    */
                    tmp_word = rest_line;
                    rest_line[tmp] = 0;
                    rest_line += (tmp + 1);
                    if (in_symbol)
                        addSymbol(symbols_table_root, symbol_name, *DC, false, false);
                    (*DC) += encodeDataline(data_img, tmp_word, rest_line, *DC);
                }
            }
            else
            {
                /*
                    add to symbol (name, curr IC, isop = true, isext = 0, next = NULL)
                    if symbol already found - print error
                    encode the data in data_img and update DC
                */
                if (in_symbol)
                    addSymbol(symbols_table_root, symbol_name, *IC, true, false);
                (*IC) += encodeCmd(code_img, rest_line, *IC, cmds_metadata_list);
            }
            in_symbol = false;
            symbol_name[0] = NULL;
        }
    }
}

int firstphase2(FILE *fd, words_img code_img, unsigned char *IC, symbol **symbols_table_root, const cmd_metadata *cmds_metadata_list)
{
    line full_line, trimed_line;
    bool in_symbol = false, is_empty_line = false, ignore_line = false;
    char *cp, *rest_line, *tmp_word;
    int tmp;
    symbol *tmp_symbol;

    while (fgets(full_line, MAX_LINE_LENGTH, fd) != NULL)
    {
        is_empty_line = isEmptyString(full_line);
        if (!is_empty_line)
            strcpy(trimed_line, trim(full_line));

        printf("\n--Line: '%s'-\n", trimed_line);
        /* check if line is not empty or a comment */
        ignore_line = is_empty_line || (!is_empty_line && trimed_line[0] == COMMENT_SIGN);

        if (!ignore_line)
        {
            /* check if line starts with a label - step 3 */
            if ((cp = strchr(trimed_line, LABEL_SIGN)))
                in_symbol = true;

            rest_line = (cp = strchr(trimed_line, LABEL_SIGN)) ? trim(cp + 1) : trimed_line;

            if (rest_line[0] == DOT)
            {
                cp = strchr(rest_line, SPACE);
                tmp = (cp - rest_line);

                if (!strncmp(rest_line, ENTRY, tmp))
                {
                    tmp_word = strtok(rest_line + tmp, SPACE_STR);
                    while (tmp_word != NULL)
                    {
                        if (tmp_symbol = isExistingSymbol(*symbols_table_root, tmp_word))
                            (*tmp_symbol).isent = true;
                        else
                            printf("Error: entry label is not defined in current file\n");
                        tmp_word = strtok(NULL, SPACE_STR);
                    }
                }
            }
            else
            {
                (*IC) += encodeMissingInfo(code_img, rest_line, *IC, cmds_metadata_list, *symbols_table_root);
            }
            in_symbol = false;
        }
    }
}

int firstphase(char *filename, words_img code_img, words_img data_img, unsigned char *IC, unsigned char *DC, symbol **symbols_table_root, const cmd_metadata *cmds_metadata_list)
{
    int i;
    FILE *fd;
    symbol *tmp;

    printf("\n----------FIRSTPHASE-START-----------\n");
    fd = openReadFile(filename, AFTER_MACROS_FILE_EXT);
    firstphase1(fd, code_img, data_img, IC, DC, symbols_table_root, cmds_metadata_list);
    printf("\n----------FIRSTPHASE-END-----------\n");
    for (tmp = *symbols_table_root; tmp != NULL; tmp = tmp->next)
        if (!(*tmp).isext && !(*tmp).isop)
            (*tmp).address += *IC;

    for (i = (*IC); i < (*IC) + (*DC); i++)
    {
        code_img[i] = data_img[i - (*IC)];
    }

    for (i = 0; i < *IC; i++)
    {
        printf("%d\toperation - command: %u\tsourceop: %u\tdestop: %u\ttype: %u\n", i, code_img[i].code.code, code_img[i].code.srcop, code_img[i].code.dstop, code_img[i].code.type);
        printf("%d\tinfo std - value: %d\ttype: %u\n", i, code_img[i].info.std.value, code_img[i].info.std.type);
        printf("%d\tinfo reg - reg1 value: %u\treg2 value: %u\ttype: %u\n", i, code_img[i].info.reg.src, code_img[i].info.reg.dst, code_img[i].info.reg.type);
    }

    printSymbols(*symbols_table_root);
    *IC = 0;
    rewind(fd);
    printf("\n----------secondPHASE-START-----------\n");
    firstphase2(fd, code_img, IC, symbols_table_root, cmds_metadata_list);
    printf("\n----------secondPHASE-end-----------\n");

    return 1;
}

int encodeMissingOperands(words_img code_img, unsigned char IC, cmd_metadata *cmd_info, char *op1, char *op2, symbol *symbols_table_root)
{
    int size = 0;
    address_type_value op1_type, op2_type;
    symbol *tmp_symbol;
    char *tmp_chr;

    switch (cmd_info->valid_args_num)
    {
    case 2:
        op1_type = code_img[IC].code.srcop;
        op2_type = code_img[IC].code.dstop;
        size++;
        if (op1_type == direct)
        {
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op1))
            {
                code_img[IC + size].info.std.value = tmp_symbol->address;
                code_img[IC + size].info.std.type = tmp_symbol->isext ? external : relocateable;
            }
            else
            {
                printf("Error: unknown label '%s'\n", op1);
            }
        }
        else if (op1_type == struct_access)
        {
            tmp_chr = strchr(op1, DOT);
            op1[tmp_chr - op1] = 0;
            tmp_chr += 1;
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op1))
            {
                code_img[IC + size].info.std.value = tmp_symbol->address;
                code_img[IC + size].info.std.type = tmp_symbol->isext ? external : relocateable;
                size++;
                code_img[IC + size].info.std.value = atoi(tmp_chr);
                code_img[IC + size].info.std.type = absolute;
            }
            else
            {
                printf("Error: unknown label '%s'\n", op1);
            }
        }
        else if (op1_type == immediate)
        {
            code_img[IC + size].info.std.value = atoi(op1 + 1);
            code_img[IC + size].info.std.type = absolute;
        }
        else if (op1_type == dir_register)
        {
            code_img[IC + size].info.reg.src = atoi(op1 + 1);
            code_img[IC + size].info.reg.dst = 0;
            code_img[IC + size].info.reg.type = absolute;
        }
        size++;

        if (op2_type == direct)
        {
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op2))
            {
                code_img[IC + size].info.std.value = tmp_symbol->address;
                code_img[IC + size].info.std.type = tmp_symbol->isext ? external : relocateable;
            }
            else
            {
                printf("Error: unknown label '%s'\n", op2);
            }
        }
        else if (op2_type == struct_access)
        {
            tmp_chr = strchr(op2, DOT);
            op2[tmp_chr - op2] = 0;
            tmp_chr += 1;
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op2))
            {
                code_img[IC + size].info.std.value = tmp_symbol->address;
                code_img[IC + size].info.std.type = tmp_symbol->isext ? external : relocateable;
                size++;
                code_img[IC + size].info.std.value = atoi(tmp_chr);
                code_img[IC + size].info.std.type = absolute;
            }
            else
            {
                printf("Error: unknown label '%s'\n", op1);
            }
        }
        else if (op2_type == immediate)
        {
            code_img[IC + size].info.std.value = atoi(op2 + 1);
            code_img[IC + size].info.std.type = absolute;
        }
        else if (op2_type == dir_register)
        {
            if (op1_type == dir_register)
            {
                size--;
                code_img[IC + size].info.reg.dst = atoi(op2 + 1);
            }
            else
            {
                code_img[IC + size].info.reg.src = 0;
                code_img[IC + size].info.reg.dst = atoi(op2 + 1);
                code_img[IC + size].info.std.type = absolute;
            }
        }
        size++;
        break;
    case 1:
        op1_type = code_img[IC].code.dstop;
        size++;
        if (op1_type == direct)
        {
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op1))
            {
                code_img[IC + size].info.std.value = tmp_symbol->address;
                code_img[IC + size].info.std.type = tmp_symbol->isext ? external : relocateable;
            }
            else
            {
                printf("Error: unknown label\n");
            }
        }
        else if (op1_type == struct_access)
        {
            tmp_chr = strchr(op1, DOT);
            op1[tmp_chr - op1] = 0;
            tmp_chr += 1;
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op1))
            {
                code_img[IC + size].info.std.value = tmp_symbol->address;
                code_img[IC + size].info.std.type = tmp_symbol->isext ? external : relocateable;
                size++;
                code_img[IC + size].info.std.value = atoi(tmp_chr);
                code_img[IC + size].info.std.type = absolute;
            }
            else
            {
                printf("Error: unknown label\n");
            }
        }
        else if (op1_type == immediate)
        {
            code_img[IC + size].info.std.value = atoi(op1 + 1);
            code_img[IC + size].info.std.type = absolute;
        }
        else if (op1_type == dir_register)
        {
            code_img[IC + size].info.reg.src = 0;
            code_img[IC + size].info.reg.dst = atoi(op1 + 1);
            code_img[IC + size].info.reg.type = absolute;
        }
        size++;
    }
    return size;
}

int getSize(words_img code_img, unsigned char IC, cmd_metadata *cmd_info, char *op1, char *op2)
{
    int size = 0;
    address_type_value op1_type, op2_type;

    op1_type = getOperandAddressType(op1);
    op2_type = getOperandAddressType(op2);

    switch (cmd_info->valid_args_num)
    {
    case 2:
        if (isValidOperand(cmd_info, op1_type, true) && isValidOperand(cmd_info, op2_type, false))
        {
            code_img[IC].code.srcop = op1_type == none ? 0 : op1_type;
            code_img[IC].code.dstop = op2_type == none ? 0 : op2_type;
            size++;

            switch (op1_type)
            {
            case immediate:
            case direct:
            case dir_register:
                size++;
                break;
            case struct_access:
                size += 2;
                break;
            }

            switch (op2_type)
            {
            case immediate:
            case direct:
                size++;
                break;
            case dir_register:
                if (op1_type != dir_register)
                    size++;
                break;
            case struct_access:
                size += 2;
                break;
            }
        }
        else
            printf("Error: invalid operand type\n");
        break;
    case 1:
        if (isValidOperand(cmd_info, op1_type, false))
        {
            code_img[IC].code.srcop = 0;
            code_img[IC].code.dstop = op1_type == none ? 0 : op1_type;
            size++;

            switch (op1_type)
            {
            case immediate:
            case direct:
            case dir_register:
                size++;
                break;
            case struct_access:
                size += 2;
                break;
            }
        }
        else
            printf("Error: invalid operand type\n");
        break;
    }
    return size;
}

int encodeMissingInfo(words_img code_img, char *line, unsigned char IC, const cmd_metadata *cmds_metadata_list, symbol *symbols_table_root)
{
    int operands_num = 0, size = 0;
    cmd_name cmd;
    label op1, op2;
    char *tmp;
    cmd_metadata *cmd_info;

    strncpy(cmd, line, COMMAND_NAME_LENGTH);
    cmd_info = getCmdMetadata(cmds_metadata_list, cmd);
    if (cmd_info->valid_args_num == 0)
        size++;
    else
    {
        tmp = (line + COMMAND_NAME_LENGTH + 1);
        operands_num = sscanf(tmp, " %[^ ,\t] ,%s", op1, op2);
        size += encodeMissingOperands(code_img, IC, cmd_info, op1, op2, symbols_table_root);
    }
    return size;
}

int encodeCmd(words_img code_img, char *line, unsigned char IC, const cmd_metadata *cmds_metadata_list)
{
    int operands_num = 0, size = 0;
    cmd_name cmd;
    label op1, op2;
    char *tmp;
    cmd_metadata *cmd_info;

    strncpy(cmd, line, COMMAND_NAME_LENGTH);
    cmd_info = getCmdMetadata(cmds_metadata_list, cmd);
    if (cmd_info != NULL)
    {
        if (cmd_info->valid_args_num == 0)
        {
            if (strlen(line) > COMMAND_NAME_LENGTH)
                printf("Error: too many operands\n");
            else
            {
                code_img[IC].code.code = cmd_info->code;
                code_img[IC].code.srcop = immediate;
                code_img[IC].code.dstop = immediate;
                code_img[IC].code.type = absolute;
                size++;
            }
        }
        else
        {
            tmp = (line + COMMAND_NAME_LENGTH + 1);
            if (tmp == NULL)
                printf("Error: too few operands\n");
            else
            {
                operands_num = sscanf(tmp, " %[^ ,\t] ,%s", op1, op2);
                if (cmd_info->valid_args_num == operands_num)
                {
                    code_img[IC].code.code = cmd_info->code;
                    code_img[IC].code.type = absolute;
                    size += getSize(code_img, IC, cmd_info, op1, op2);
                }
                else
                    printf("Error: wrong number of operands\n");
            }
        }
    }
    else
        printf("Error: Invalid command\n");

    return size;
}

int encodeData(char *line, words_img data_img, unsigned char DC)
{
    int num = 0, size = 0;
    char *tmp;
    tmp = strtok(line, COMMA_STR);

    while (tmp != NULL)
    {
        num = atoi(tmp);
        data_img[DC + size++].data.value = num;
        tmp = strtok(NULL, COMMA_STR);
    }
    return size;
}

int encodeString(char *line, words_img data_img, unsigned char DC)
{
    int size = 0;
    char *start, *end;
    start = strchr(line, DBL_QUOTES);
    end = strchr(++start, DBL_QUOTES);
    while (start < end)
    {
        data_img[DC + size++].data.value = (*start++);
    }
    data_img[DC + size++].data.value = 0;
    return size;
}

int encodeStruct(char *line, words_img data_img, unsigned char DC)
{
    int num = 0, size = 0;
    char *tmp;
    tmp = strtok(line, COMMA_STR);
    num = atoi(tmp);
    data_img[DC + size++].data.value = num;
    tmp = strtok(NULL, COMMA_STR);
    size += encodeString(tmp, data_img, DC + size);

    return size;
}

int encodeDataline(words_img data_img, char *type, char *line, unsigned char DC)
{
    int size = 0;
    if (!strcmp(type, DATA))
        size = encodeData(line, data_img, DC);
    else if (!strcmp(type, STRING))
        size = encodeString(line, data_img, DC);
    else if (!strcmp(type, STRUCT))
        size = encodeStruct(line, data_img, DC);
    else
        printf("Error: Invalid data line");
    return size;
}