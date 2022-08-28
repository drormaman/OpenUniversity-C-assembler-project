#include <stdio.h>
#include <string.h>
#include "../files/files.h"
#include "../assembler/assembler.h"
#include "../strings/strings.h"
#include "../symbols/symbols.h"
#include "../methods/methods.h"
#include "output.h"

char *convertDecTo32(int num, bool padded)
{
    char base_nums[BASE] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};
    unsigned int mod, div;
    static base32_word result = {0};

    if (!padded && num < BASE)
    {
        result[0] = base_nums[num];
    }
    else
    {
        div = num / BASE;
        mod = num % BASE;
        result[1] = base_nums[mod];
        result[0] = base_nums[div];
    }

    return result;
}

char *convertBitsTo32(word data, bool padded)
{
    char *res;
    int num = 0;
    if (data.bits.b0)
        num |= 1;
    if (data.bits.b1)
        num |= 2;
    if (data.bits.b2)
        num |= 4;
    if (data.bits.b3)
        num |= 8;
    if (data.bits.b4)
        num |= 16;
    if (data.bits.b5)
        num |= 32;
    if (data.bits.b6)
        num |= 64;
    if (data.bits.b7)
        num |= 128;
    if (data.bits.b8)
        num |= 256;
    if (data.bits.b9)
        num |= 512;
    res = convertDecTo32(num, padded);
    return res;
}

void writeObjectFile(char *filename, words_img code_img, unsigned char IC, unsigned char DC)
{
    int i;
    FILE *fdw;
    fdw = openWriteFile(filename, OBJECT_FILE_EXT);
    base32_word address, value, ic, dc;
    strcpy(ic, convertDecTo32(IC, false));
    strcpy(dc, convertDecTo32(DC, false));
    fprintf(fdw, "\t%s %s\n", ic, dc);

    for (i = 0; i < IC + DC; i++)
    {
        strcpy(address, convertDecTo32(i + MEM_SKIP, true));
        if (i < IC && code_img[i].info.rel.type == relocateable)
            code_img[i].info.rel.type += MEM_SKIP;
        strcpy(value, convertBitsTo32((word)code_img[i].data, true));
        fprintf(fdw, "%s\t\t%s\n", address, value);
    }
}

void writeEntryFile(char *filename, symbol *symbol_table)
{
    FILE *fdw;
    base32_word address;
    label symbol_name;
    symbol *tmp;

    fdw = openWriteFile(filename, ENTRIES_FILE_EXT);
    for (tmp = symbol_table; tmp != NULL; tmp = tmp->next)
    {
        if ((*tmp).isent)
        {
            strcpy(symbol_name, (*tmp).name);
            strcpy(address, convertDecTo32((*tmp).address + MEM_SKIP, true));
            fprintf(fdw, "%s\t\t%s\n", symbol_name, address);
        }
    }
}

void writeExternFile(char *filename, symbol *symbol_table)
{
    FILE *fdw;
    base32_word address;
    label symbol_name;
    symbol *tmp;
    location *locp;

    fdw = openWriteFile(filename, EXTERNALS_FILE_EXT);
    for (tmp = symbol_table; tmp != NULL; tmp = tmp->next)
    {
        if ((*tmp).isext)
        {
            strcpy(symbol_name, (*tmp).name);
            for (locp = tmp->access_address; locp != NULL; locp = locp->next)
            {
                strcpy(address, convertDecTo32((*locp).address + MEM_SKIP, true));
                fprintf(fdw, "%s\t\t%s\n", symbol_name, address);
            }
        }
    }
}