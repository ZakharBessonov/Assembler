#include <stdio.h>

#include "asm_consts.h"
#include "asm_general_funcs.h"
#include "asm_help_funcs.h"
#include "commands.h"
#include "asm_listing_funcs.h"
#include "asm_structs.h"

FILE* logfileAsm = NULL;

int main(int argc, const char *argv[])
{
    logfileAsm = fopen(LOG_FILE_NAME, "w");
    if (argc <= REQUIRED_NUMBER_OF_ARGUMENTS)
    {
        fprintf(logfileAsm, "ERROR: Too little arguments were passed. \n"
                            "       Input data format: sourceFileName.asm outputFileName.bin\n");
        return 1;
    }

    DataForAssembly dataForAssembly = {};

    if (OpenSourceFile(argv[1], &dataForAssembly))
    {
        return 1;
    }

    AsmReadFileAndCreateArrayOfPointers(&dataForAssembly);
    AsmInitLabels(&dataForAssembly);
    AsmInitHeaderForByteCode(&dataForAssembly);

    if (AsmCompileByteCode(&dataForAssembly))
    {
        return 1;
    }

    AsmCompileByteCode(&dataForAssembly);   // Second compilation to convert labels into addresses.

    AsmWriteInformationToListingFile(&dataForAssembly, argv[1])

    AsmByteCodeDtor(&dataForAssembly);
    fclose(logfileAsm);

    return 0;
}
