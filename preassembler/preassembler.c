#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preassembler.h"

void preAssembler(char *filename)
{

    FILE *fpr, *fpw;
    Macro *r_macro = NULL;
    Macro *tmp_macro;
    char new_macro_name[MAX_MACRO_NAME_LENGTH] = {0};
    char new_macro_text[MAX_LINE_LENGTH * MAX_MACRO_LINES] = {0};
    char word[MAX_MACRO_NAME_LENGTH] = {0};
    char line[MAX_LINE_LENGTH];
    char in_macro = 0;

    fpr = openReadFile(filename);
    fpw = openWriteFile(filename, ".am");

    while (fgets(line, 80, fpr) != NULL)
    {
        sscanf(line, " %s ", word);
        if (in_macro)
        {
            if (!strcmp(word, "endmacro"))
            {
                in_macro = 0;
                addMacro(&r_macro, new_macro_name, new_macro_text);
            }
            else
            {
                strcat(new_macro_text, line);
            }
        }
        else if ((tmp_macro = isExistingMacro(r_macro, word)))
        {
            fputs(tmp_macro->text, fpw);
        }
        else if (sscanf(line, " macro %s ", new_macro_name))
        {
            in_macro = 1;
        }
        else
        {
            fputs(line, fpw);
        }
    }

    fclose(fpr);
    fclose(fpw);
    freeMacros(r_macro);
    /*if (in_macro)
    {

        if (strstr(line, "endmacro") != NULL)
        {
            in_macro = 0;
            addMacro(&r_macro, macro_name, macro_text);
            *macro_name = NULL;
            *macro_text = NULL;
        }
        else
        {
            strcat(macro_text, line);
            printf("macro %s\n", macro_text);
            printf("line %s\n", line);
        }
    }
    else if (sscanf(line, " macro %s", macro_name) == 1)
    {
        in_macro = 1;
    }
    else
    {
        tmp_macro = isExistingMacro(r_macro, line);
        if (tmp_macro)
        {
            fprintf(fpw, "%s", tmp_macro->text);
        }
        else
        {
            fprintf(fpw, "%s", line);
        }
    }
}
*/
}