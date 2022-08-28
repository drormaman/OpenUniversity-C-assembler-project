#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../strings/strings.h"
#include "../files/files.h"
#include "encode.h"
#include "../symbols/symbols.h"
#include "../assembler/assembler.h"
#include "../methods/methods.h"

bool firstphase(FILE *fd, words_img code_img, words_img data_img, unsigned char *IC, unsigned char *DC, symbol **symbols_table_root, const cmd_metadata *cmds_metadata_list);
bool secondphase(FILE *fd, words_img code_img, unsigned char *IC, symbol **symbols_table_root, const cmd_metadata *cmds_metadata_list);

bool encodeImg(char *filename, words_img code_img, words_img data_img, unsigned char *IC, unsigned char *DC, symbol **symbols_table_root, const cmd_metadata *cmds_metadata_list)
{
    int i;
    FILE *fd;
    symbol *tmp;
    bool is_error;

    fd = openReadFile(filename, AFTER_MACROS_FILE_EXT);
    /* first round over source code (as in guide algorithm) */
    is_error = firstphase(fd, code_img, data_img, IC, DC, symbols_table_root, cmds_metadata_list);

    if (!is_error)
    {
        for (tmp = *symbols_table_root; tmp != NULL; tmp = tmp->next)
            if (!(*tmp).isext && !(*tmp).isop)
                (*tmp).address += *IC;
        for (i = (*IC); i < (*IC) + (*DC); i++)
        {
            code_img[i] = data_img[i - (*IC)];
        }
        *IC = 0;
        rewind(fd);
        /* second round over source code (as in guide algorithm) */
        is_error = secondphase(fd, code_img, IC, symbols_table_root, cmds_metadata_list);
    }

    return is_error;
}

int encodeMissingOperands(words_img code_img, unsigned char IC, cmd_metadata *cmd_info, char *op1, char *op2, symbol *symbols_table_root, int line_num)
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

        switch (op1_type)
        {
        case immediate:
            code_img[IC + size].info.std.value = atoi(op1 + 1);
            code_img[IC + size].info.std.type = absolute;
            break;
        case direct:
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op1))
            {
                if (tmp_symbol->isext)
                {
                    code_img[IC + size].info.std.value = 0;
                    code_img[IC + size].info.std.type = external;
                    addLocation(&tmp_symbol, IC + size);
                }
                else
                {
                    code_img[IC + size].info.std.value = tmp_symbol->address;
                    code_img[IC + size].info.std.type = relocateable;
                }
            }
            else
                printf("Error: unknown label. Line %d\n", line_num);
            break;
        case struct_access:
            tmp_chr = strchr(op1, DOT);
            op1[tmp_chr - op1] = 0;
            tmp_chr += 1;
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op1))
            {
                if (tmp_symbol->isext)
                {
                    code_img[IC + size].info.std.value = 0;
                    code_img[IC + size].info.std.type = external;
                    addLocation(&tmp_symbol, IC + size);
                }
                else
                {
                    code_img[IC + size].info.std.value = tmp_symbol->address;
                    code_img[IC + size].info.std.type = relocateable;
                }
                size++;
                code_img[IC + size].info.std.value = atoi(tmp_chr);
                code_img[IC + size].info.std.type = absolute;
            }
            else
                printf("Error: unknown label. Line %d\n", line_num);
            break;
        case dir_register:
            code_img[IC + size].info.reg.src = atoi(op1 + 1);
            code_img[IC + size].info.reg.dst = 0;
            code_img[IC + size].info.reg.type = absolute;
            break;
        }
        size++;

        switch (op2_type)
        {
        case immediate:
            code_img[IC + size].info.std.value = atoi(op2 + 1);
            code_img[IC + size].info.std.type = absolute;
            break;
        case direct:
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op2))
            {
                if (tmp_symbol->isext)
                {
                    code_img[IC + size].info.std.value = 0;
                    code_img[IC + size].info.std.type = external;
                    addLocation(&tmp_symbol, IC + size);
                }
                else
                {
                    code_img[IC + size].info.std.value = tmp_symbol->address;
                    code_img[IC + size].info.std.type = relocateable;
                }
            }
            else
                printf("Error: unknown label. Line %d\n", line_num);
            break;
        case struct_access:
            tmp_chr = strchr(op2, DOT);
            op2[tmp_chr - op2] = 0;
            tmp_chr += 1;
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op2))
            {
                if (tmp_symbol->isext)
                {
                    code_img[IC + size].info.std.value = 0;
                    code_img[IC + size].info.std.type = external;
                    addLocation(&tmp_symbol, IC + size);
                }
                else
                {
                    code_img[IC + size].info.std.value = tmp_symbol->address;
                    code_img[IC + size].info.std.type = relocateable;
                }
                size++;
                code_img[IC + size].info.std.value = atoi(tmp_chr);
                code_img[IC + size].info.std.type = absolute;
            }
            else
                printf("Error: unknown label. Line %d\n", line_num);
            break;
        case dir_register:
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
            break;
        }
        size++;
        break;
    case 1:
        op1_type = code_img[IC].code.dstop;
        size++;
        switch (op1_type)
        {
        case immediate:
            code_img[IC + size].info.std.value = atoi(op1 + 1);
            code_img[IC + size].info.std.type = absolute;
            break;
        case direct:
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op1))
            {
                if (tmp_symbol->isext)
                {
                    code_img[IC + size].info.std.value = 0;
                    code_img[IC + size].info.std.type = external;
                    addLocation(&tmp_symbol, IC + size);
                }
                else
                {
                    code_img[IC + size].info.std.value = tmp_symbol->address;
                    code_img[IC + size].info.std.type = relocateable;
                }
            }
            else
                printf("Error: unknown label. Line %s %d\n", op1, line_num);
            break;
        case struct_access:
            tmp_chr = strchr(op1, DOT);
            op1[tmp_chr - op1] = 0;
            tmp_chr += 1;
            if (tmp_symbol = isExistingSymbol(symbols_table_root, op1))
            {
                if (tmp_symbol->isext)
                {
                    code_img[IC + size].info.std.value = 0;
                    code_img[IC + size].info.std.type = external;
                    addLocation(&tmp_symbol, IC + size);
                }
                else
                {
                    code_img[IC + size].info.std.value = tmp_symbol->address;
                    code_img[IC + size].info.std.type = relocateable;
                }
                size++;
                code_img[IC + size].info.std.value = atoi(tmp_chr);
                code_img[IC + size].info.std.type = absolute;
            }
            else
                printf("Error: unknown label. Line %d\n", line_num);
            break;
        case dir_register:
            code_img[IC + size].info.reg.src = 0;
            code_img[IC + size].info.reg.dst = atoi(op1 + 1);
            code_img[IC + size].info.reg.type = absolute;
            break;
        }
        size++;
    }
    return size;
}

int encodeMissingInfo(words_img code_img, char *line, unsigned char IC, const cmd_metadata *cmds_metadata_list, symbol *symbols_table_root, int line_num)
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
        size += encodeMissingOperands(code_img, IC, cmd_info, op1, op2, symbols_table_root, line_num);
    }
    return size;
}

int getSize(words_img code_img, unsigned char IC, cmd_metadata *cmd_info, char *op1, char *op2, int line_num)
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
            printf("Error: invalid operand type. Line %d\n", line_num);
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
            printf("Error: invalid operand type. Line %d\n", line_num);
        break;
    }
    return size;
}

int encodeCmd(words_img code_img, char *line, unsigned char IC, const cmd_metadata *cmds_metadata_list, int line_num)
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
                printf("Error: too many operands. Line %d\n", line_num);
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
                printf("Error: too few operands. Line %d\n", line_num);
            else
            {
                operands_num = sscanf(tmp, " %[^ ,\t] ,%s", op1, op2);
                if (cmd_info->valid_args_num == operands_num)
                {
                    code_img[IC].code.code = cmd_info->code;
                    code_img[IC].code.type = absolute;
                    size += getSize(code_img, IC, cmd_info, op1, op2, line_num);
                }
                else
                    printf("Error: invalid number of operands. Line %d\n", line_num);
            }
        }
    }
    else
        printf("Error: Invalid command. Line %d\n", line_num);

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

int encodeDataline(words_img data_img, char *type, char *line, unsigned char DC, int line_num)
{
    int size = 0;
    if (!strcmp(type, DATA))
        size = encodeData(line, data_img, DC);
    else if (!strcmp(type, STRING))
        size = encodeString(line, data_img, DC);
    else if (!strcmp(type, STRUCT))
        size = encodeStruct(line, data_img, DC);
    else
        printf("Error: Invalid data definition. Line %d\n", line_num);
    return size;
}

bool firstphase(FILE *fd, words_img code_img, words_img data_img, unsigned char *IC, unsigned char *DC, symbol **symbols_table_root, const cmd_metadata *cmds_metadata_list)
{
    int line_num = 1, tmp, size;
    line full_line, trimed_line;
    bool in_symbol = false, is_empty_line = false, ignore_line = false, is_error = false;
    label symbol_name;
    char *cp, *rest_line, *tmp_word;

    while (fgets(full_line, MAX_LINE_LENGTH, fd) != NULL)
    {
        is_empty_line = isEmptyString(full_line);
        if (!is_empty_line)
            strcpy(trimed_line, trim(full_line));

        /* check if line is not empty or a comment */
        ignore_line = is_empty_line || (!is_empty_line && trimed_line[0] == COMMENT_SIGN);

        if (!ignore_line)
        {
            /* check if line starts with a label */
            if ((cp = strchr(trimed_line, LABEL_SIGN)))
            {
                tmp = (cp - trimed_line);
                strncpy(symbol_name, trimed_line, tmp);
                symbol_name[tmp] = NULL;
                if (isValidLabel(symbol_name))
                    if (!isExistingSymbol(*symbols_table_root, symbol_name))
                        in_symbol = true;
                    else
                        printf("Error: label already exists. Line %d\n", line_num);
                else
                    printf("Error: invalid label name. Line %d\n", line_num);
            }

            rest_line = in_symbol ? trim(cp + 1) : trimed_line;
            if (rest_line[0] == DOT)
            {
                cp = strchr(rest_line, SPACE);
                tmp = (cp - rest_line);

                /* check if extern */
                if (!strncmp(rest_line, EXTERN, tmp))
                {
                    tmp_word = strtok(rest_line + tmp, SPACE_STR);
                    while (tmp_word != NULL)
                    {

                        if (isValidLabel(tmp_word))
                            if (!isExistingSymbol(*symbols_table_root, tmp_word))
                                addSymbol(symbols_table_root, tmp_word, 0, false, true);
                            else
                                printf("Error: label already exists. Line %d\n", line_num);
                        else
                            printf("Error: invalid label name. Line %d\n", line_num);
                        tmp_word = strtok(NULL, SPACE_STR);
                    }
                }
                /* check if not entry */
                else if (strncmp(rest_line, ENTRY, tmp) != 0)
                {
                    tmp_word = rest_line;
                    rest_line[tmp] = 0;
                    rest_line += (tmp + 1);
                    if (in_symbol)
                        addSymbol(symbols_table_root, symbol_name, *DC, false, false);
                    size = encodeDataline(data_img, tmp_word, rest_line, *DC, line_num);
                    if (!size)
                        is_error = true;
                    (*DC) += size;
                }
            }
            else
            {
                if (in_symbol)
                    addSymbol(symbols_table_root, symbol_name, *IC, true, false);
                size = encodeCmd(code_img, rest_line, *IC, cmds_metadata_list, line_num);
                if (!size)
                    is_error = true;
                (*IC) += size;
            }
            in_symbol = false;
            symbol_name[0] = NULL;
        }
        line_num++;
    }
    return is_error;
}

bool secondphase(FILE *fd, words_img code_img, unsigned char *IC, symbol **symbols_table_root, const cmd_metadata *cmds_metadata_list)
{
    int tmp, line_num = 1, size;
    line full_line, trimed_line;
    bool in_symbol = false, is_empty_line = false, ignore_line = false, is_error = false;
    char *cp, *rest_line, *tmp_word;
    symbol *tmp_symbol;

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
                in_symbol = true;

            rest_line = (cp = strchr(trimed_line, LABEL_SIGN)) ? trim(cp + 1) : trimed_line;

            if (rest_line[0] == DOT)
            {
                cp = strchr(rest_line, SPACE);
                tmp = (cp - rest_line);

                /* check if entry */
                if (!strncmp(rest_line, ENTRY, tmp))
                {
                    tmp_word = strtok(rest_line + tmp, SPACE_STR);
                    while (tmp_word != NULL)
                    {
                        if (tmp_symbol = isExistingSymbol(*symbols_table_root, tmp_word))
                            (*tmp_symbol).isent = true;
                        else
                        {
                            is_error = true;
                            printf("Error: entry label is not defined in current file. Line %d\n", line_num);
                        }
                        tmp_word = strtok(NULL, SPACE_STR);
                    }
                }
            }
            else
            {
                size = encodeMissingInfo(code_img, rest_line, *IC, cmds_metadata_list, *symbols_table_root, line_num);
                if (!size)
                    is_error = true;
                (*IC) += size;
            }
            in_symbol = false;
        }
        line_num++;
    }
    return is_error;
}