#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preassembler.h"
#include "../strings/strings.h"

void preAssembler(char *filename)
{
    FILE *fdr, *fdw;
    macro *macros_root = NULL;
    macro *tmp_macro;
    label new_macro_name, tmp_word;
    line line, trimed_line;
    bool in_macro = false, is_empty_line = false;
    int i = 0;

    fdr = openReadFile(filename, INPUT_FILE_EXT);
    fdw = openWriteFile(filename, AFTER_MACROS_FILE_EXT);

    printf("\n----------MACRO-START-----------\n");
    while (fgets(line, MAX_LINE_LENGTH, fdr) != NULL)
    {
        is_empty_line = isEmptyString(line);

        strcpy(trimed_line, trim(line));

        if (!is_empty_line)
            strcpy(tmp_word, strtok(trimed_line, SPACE_STR));

        if (!in_macro)
        {
            if (!is_empty_line && (tmp_macro = isExistingMacro(macros_root, tmp_word)))
                fputs(tmp_macro->text, fdw);
            else if (!is_empty_line && !strcmp(tmp_word, MACRO))
            {
                in_macro = true;
                strcpy(new_macro_name, strtok(NULL, SPACE_STR));
                addMacro(&macros_root, new_macro_name);
            }
            else
            {
                fputs(line, fdw);
            }
        }
        else
        {
            if (strlen(tmp_word) > 0 && !strcmp(tmp_word, ENDMACRO))
                in_macro = false;
            else
                strcat((*macros_root).text, line);
        }
        tmp_word[0] = '\0';
    }
    printf("\n----------MACRO-END-----------");

    fclose(fdr);
    fclose(fdw);
    freeMacros(macros_root);
}