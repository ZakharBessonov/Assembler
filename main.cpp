#include <stdio.h>
#include <stdlib.h>

#include "asm_consts.h"
#include "asm_general_funcs.h"
#include "asm_help_funcs.h"
#include "asm_listing_funcs.h"
#include "asm_structs.h"

FILE* logfileAsm = NULL;

int main(int argc, const char *argv[])
{
    atexit(AsmCloseLogFile);

    if (AsmOpenLogFile())
    {
        return 0;
    }

    logfileAsm = fopen(LOG_FILE_NAME_ASM, "w");
    if (argc <= REQUIRED_NUMBER_OF_ARGUMENTS)
    {
        fprintf(logfileAsm, "ERROR: Too little arguments were passed. \n"
                            "       Input data format: sourceFileName.asm outputFileName.bin\n");
        return 0;
    }

    DataForAssembly dataForAssembly = {};

    if (AsmOpenSourceFile(argv[1], &dataForAssembly))
    {
        return 0;
    }

    AsmReadFileAndCreateArrayOfPointers(&dataForAssembly);
    AsmInitLabels(&dataForAssembly);
    AsmInitHeaderForByteCode(&dataForAssembly);

    if (AsmCompileByteCode(&dataForAssembly))
    {
        return 0;
    }

    AsmCompileByteCode(&dataForAssembly);   // Second compilation to convert labels into addresses.

    AsmWriteInformationToListingFile(&dataForAssembly, argv[1]);

    if (AsmWriteByteCodeToBinFile(&dataForAssembly, argv[2]))
    {
        return 0;
    }

    AsmByteCodeDtor(&dataForAssembly);

    return 0;
}
