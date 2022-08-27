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
int encodeOpline(words_img code_img, char *line, unsigned char IC, const cmd_metadata *cmds_metadata_list);

int firstphase(char *filename, words_img code_img, words_img data_img, unsigned char *IC, unsigned char *DC, symbol **symbols_table_root, const cmd_metadata *cmds_metadata_list)
{
    FILE *fd;
    line full_line, trimed_line;
    bool in_symbol = false, is_empty_line = false, ignore_line = false;
    label symbol_name;
    char *cp, *rest_line, *tmp_word;
    int res, i, tmp;

    printf("\n----------FIRSTPHASE-START-----------\n");
    fd = openReadFile(filename, AFTER_MACROS_FILE_EXT);
    while (fgets(full_line, MAX_LINE_LENGTH, fd) != NULL)
    {
        printf("\n----------FIRSTPHASE-WHILE IC: %d, DC: %d\n", *IC, *DC);
        is_empty_line = isEmptyString(full_line);
        if (!is_empty_line)
            strcpy(trimed_line, trim(full_line));

        /* check if line is not empty or a comment */
        ignore_line = is_empty_line || (!is_empty_line && trimed_line[0] == COMMENT_SIGN);

        if (!ignore_line)
        {
            printf("not ignoring line: '%s'\n", trimed_line);
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
                else if (!strncmp(rest_line, ENTRY, tmp))
                {
                    /* todo handle entry
                    tmp_word = strtok(rest_line+tmp, SPACE_STR);
                    while (tmp != NULL)
                    {
                        if (isValidLabel(tmp))
                            if (!isExistingSymbol(*symbols_table_root, tmp_word))
                                addSymbol(symbols_table_root, tmp_word, 0, false, false);
                            else
                                printf("Error: label already exists\n");
                        else
                            printf("Error: invalid label name\n");
                        tmp_word = strtok(NULL, SPACE_STR);
                    }*/
                }
                else
                {
                    /*
                        add to symbol (name, curr DC, isop = false, isext = false, next = NULL)
                        if symbol already found - print error
                        encode the data in data_img and update DC
                    */
                    tmp_word = rest_line;
                    rest_line[tmp] = 0;
                    rest_line += (tmp + 1);
                    printf("\n rest_line: '%s', tmp_word: '%s'", rest_line, tmp_word);
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
                (*IC) += encodeOpline(code_img, rest_line, *IC, cmds_metadata_list);
            }
            in_symbol = false;
            symbol_name[0] = NULL;
            printSymbols(*symbols_table_root);
        }
    }
    printf("\n----------FIRSTPHASE-END-----------\n");
    return 1;
}

int encodeOperands(words_img code_img, unsigned char IC, cmd_metadata *cmd_info, char *op1, char *op2)
{
    int size = 0;
    address_type_value op1_type, op2_type;
    bool is_op1_valid, is_op2_valid;

    op1_type = getOperandAddressType(op1);
    op2_type = getOperandAddressType(op2);

    switch (cmd_info->valid_args_num)
    {
    case 2:
        is_op1_valid = isValidOperand(cmd_info, op1_type, true);
        is_op2_valid = isValidOperand(cmd_info, op2_type, false);
        if (is_op1_valid && is_op2_valid)
        {
            code_img[IC].code.srcop = op1_type == none ? 0 : op1_type;
            code_img[IC].code.dstop = op2_type == none ? 0 : op2_type;
            size++;

            switch (op1_type)
            {
            case immediate:
                code_img[IC + size].info.std.value = atoi(op1 + 1);
                code_img[IC + size].info.std.type = absolute;
                break;
            case direct:
                code_img[IC + size].info.std.value = 0;
                code_img[IC + size].info.std.type = relocateable;
                break;
            case struct_access:
                code_img[IC + size].info.std.value = 0;
                code_img[IC + size].info.std.type = relocateable;
                size++;
                code_img[IC + size].info.std.value = atoi((strchr(op1, DOT) + 1));
                code_img[IC + size].info.std.type = absolute;
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
                code_img[IC + size].info.std.value = atoi(op1 + 1);
                code_img[IC + size].info.std.type = absolute;
                break;
            case direct:
                code_img[IC + size].info.std.value = 0;
                code_img[IC + size].info.std.type = relocateable;
                break;
            case struct_access:
                code_img[IC + size].info.std.value = 0;
                code_img[IC + size].info.std.type = relocateable;
                size++;
                code_img[IC + size].info.std.value = atoi((strchr(op1, DOT) + 1));
                code_img[IC + size].info.std.type = absolute;
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
        }
        else
            printf("Error: invalid operand type\n");
        break;
    case 1:
        is_op1_valid = isValidOperand(cmd_info, op1_type, false);
        if (is_op1_valid)
        {
            code_img[IC].code.srcop = 0;
            code_img[IC].code.dstop = op1_type == none ? 0 : op1_type;
            size++;

            switch (op1_type)
            {
            case immediate:
                code_img[IC + size].info.std.value = atoi(op1 + 1);
                code_img[IC + size].info.std.type = absolute;
                break;
            case direct:
                code_img[IC + size].info.std.value = 0;
                code_img[IC + size].info.std.type = relocateable;
                break;
            case struct_access:
                code_img[IC + size].info.std.value = 0;
                code_img[IC + size].info.std.type = relocateable;
                size++;
                code_img[IC + size].info.std.value = atoi((strchr(op1, DOT) + 1));
                code_img[IC + size].info.std.type = absolute;
                break;
            case dir_register:
                code_img[IC + size].info.reg.src = 0;
                code_img[IC + size].info.reg.dst = atoi(op1 + 1);
                code_img[IC + size].info.reg.type = absolute;
                break;
            }
            size++;
        }
        else
            printf("Error: invalid operand type\n");
        break;
    }
    return size;
}

int encodeOpline(words_img code_img, char *line, unsigned char IC, const cmd_metadata *cmds_metadata_list)
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
            if ((int)strlen(line) > COMMAND_NAME_LENGTH)
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
                operands_num = sscanf(tmp, "%[^ ,\t] ,%s", op1, op2);

                if (cmd_info->valid_args_num == operands_num)
                {
                    code_img[IC].code.code = cmd_info->code;
                    code_img[IC].code.type = absolute;
                    size += encodeOperands(code_img, IC, cmd_info, op1, op2);
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