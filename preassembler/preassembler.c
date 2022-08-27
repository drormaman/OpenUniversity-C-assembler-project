#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preassembler.h"
#include "../strings/strings.h"

void preAssembler(char *filename)
{
    FILE *fpr, *fpw;
    Macro *r_macro = NULL;
    Macro *tmp_macro;
    Label new_macro_name;
    char new_macro_text[MAX_LINE_LENGTH * MAX_MACRO_LINES] = {0};
    char word[MAX_MACRO_NAME_LENGTH] = {0};
    Line line, trimed_line;
    bool in_macro = false;
    int i = 0;

    fpr = openReadFile(filename, ".as");
    fpw = openWriteFile(filename, ".am");

    printf("\n----------MACRO-START-----------\n");
    while (fgets(line, 80, fpr) != NULL)
    {
        /*line[strlen(line) - 1] = '\0';*/
        strcpy(trimed_line, trimLeft(line));

        printf("\n'%s'", line);
        /*printf("\n'%s',int: %d size: %d", trimed_line, *trimed_line, strlen(trimed_line));*/

        if (!isEmptyString(line))
        {
            strcpy(word, strtok(trimed_line, " "));
        }

        if (!in_macro)
        {
            if ((tmp_macro = isExistingMacro(r_macro, word)))
            {
                fputs(tmp_macro->text, fpw);
            }
            else if (!strcmp(word, "macro"))
            {

                in_macro = true;
                strcpy(new_macro_name, strtok(NULL, " "));
                printf("\nmacro name: '%s'", new_macro_name);
                addMacro(&r_macro, new_macro_name);
            }
            else
            {
                fputs(line, fpw);
            }
        }
        else
        {
            if (!strcmp(word, "endmacro"))
            {
                in_macro = false;
            }
            else
            {
                strcat((*r_macro).text, line);
            }
        }

        word[0] = '\0';
        /*


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
                else if (sscanf(line, " macro %s", new_macro_name))
                {
                    printf("\nmacro name: '%s' line: '%s'", new_macro_name, line);
                    in_macro = 1;
                }
                else
                {
                    fputs(line, fpw);
                }*/
    }
    printf("\n----------MACRO-END-----------");

    fclose(fpr);
    fclose(fpw);
    freeMacros(r_macro);
}