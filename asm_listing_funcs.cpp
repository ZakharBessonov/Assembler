#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm_consts.h"
#include "commands.h"
#include "asm_listing_funcs.h"
#include "asm_structs.h"

extern FILE* logfile;

int AsmWriteInformationToListingFile(DataForAssembly* dataForAssembly, const char* sourceFileName)
{
    FILE* listingFile = fopen(LISTING_FILE_NAME, fileOpenMode);
    if (listingFile == NULL)
    {
        fprintf(logfile, "ERROR: An error occurred while opening the listing file.\n");
        return 1;
    }

    fprintf(listingFile, "--------------------- Listing file for %s ---------------------\n\n", sourceFileName);

    for (size_t i = HEADER_OFFSET; i < dataForAssembly->lengthOfByteCode; i++)
    {
        if (dataForAssembly->byteCode[i] < 0 || dataForAssembly->byteCode[i] >= CNT_OF_COMMANDS)
        {
            fprintf(listingFile, "[%zu] Unknown command\n", i);
            continue;
        }

        int codeOfCmd = commands[dataForAssembly->byteCode[i]].code;
        switch(commands[dataForAssembly->byteCode[i]].type)
        {
            case PARAM_ZERO:
                fprintf(listingFile, "[%8zu] %-10d %-20s\n", i, codeOfCmd, comands[codeOfCmd].name);
                break;
            case PARAM_REGISTER:
                int arg = dataForAssembly->byteCode[i + 1];
                fprintf(listingFile, "[%8zu] %-5d%-5d %-10s%cX\n", i, codeOfCmd, arg, comands[codeOfCmd].name, arg + 'A');
                i++;
                break;
            default:
                int arg = dataForAssembly->byteCode[i + 1];
                fprintf(listingFile, "[%8zu] %-5d%-5d %-10s%-10d\n", i, codeOfCmd, arg, comands[codeOfCmd].name, arg);
                i++;
                break;
        }
    }

    fclose(listingFile);
    return 0;
}
