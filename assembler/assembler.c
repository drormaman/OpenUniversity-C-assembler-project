#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"

int main(int argc, char *argv[])
{
    int i;
    for (i = 1; i < argc; i++)
    {
        preAssembler(argv[i]);
    }
    return 0;
}