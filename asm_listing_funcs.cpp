#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm_consts.h"
#include "codes_of_commands.h"
#include "asm_listing_funcs.h"
#include "asm_structs.h"
#include "asm_help_funcs.h"

extern FILE* logfileAsm;

int AsmWriteInformationToListingFile(DataForAssembly* dataForAssembly, const char* sourceFileName)
{
    FILE* listingFile = fopen(LISTING_FILE_NAME, "w");
    if (listingFile == NULL)
    {
        PRINT_LOG_FILE_ASM("ERROR: An error occurred while opening the listing file.\n");
        return 1;
    }

    fprintf(listingFile, "--------------------- Listing file for %s ---------------------\n\n", sourceFileName);

    for (int i = HEADER_OFFSET; i < (int)dataForAssembly->lengthOfByteCode; i++)
    {
        if (dataForAssembly->byteCode[i] < 0 || dataForAssembly->byteCode[i] >= CNT_OF_COMMANDS)
        {
            fprintf(listingFile, "[%08d] Unknown command\n", i - HEADER_OFFSET);
            continue;
        }

        int codeOfCmd = cmds[dataForAssembly->byteCode[i]].code;
        int arg = 0;
        switch(cmds[dataForAssembly->byteCode[i]].type)
        {
            case PARAM_ZERO:
                fprintf(listingFile, "[%08d] %-10d %-20s\n", i - HEADER_OFFSET, codeOfCmd, cmds[codeOfCmd].name);
                break;
            case PARAM_RAM_MEMORY:
            case PARAM_REGISTER:
                arg = dataForAssembly->byteCode[i + 1];
                fprintf(listingFile, "[%08d] %-5d%-5d %-10s%cX\n", i - HEADER_OFFSET, codeOfCmd, arg, cmds[codeOfCmd].name, arg + 'A');
                i++;
                break;
            case PARAM_NUMBER:
            case PARAM_LABEL:
                arg = dataForAssembly->byteCode[i + 1];
                fprintf(listingFile, "[%08d] %-5d%-5d %-10s%-10d\n", i - HEADER_OFFSET, codeOfCmd, arg, cmds[codeOfCmd].name, arg);
                i++;
                break;
            default:
                break;
        }
    }

    fclose(listingFile);
    return 0;
}
