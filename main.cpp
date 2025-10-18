#include <stdio.h>
#include <stdlib.h>

#include "asm_consts.h"
#include "asm_generalFuncs.h"
#include "asm_helpFuncs.h"
#include "commands.h"
#include "asm_listingFuncs.h"
#include "asm_structs"

FILE* logfileAsm = NULL;

int main(int argc, const char *argv[])
{
    logfileAsm = fopen(LOG_FILE_NAME, "w");
    if (argc <= REQUIRED_NUMBER_OF_ARGUMENTS)
    {
        fprintf(logfileAsm, "ERROR: Too little arguments were passed. \n"
                            "Input data format: sourceFileName.asm outputFileName.bin\n");
        return 1;
    }

    DataForAssembly dataForAssembly = {};

    if (OpenSourceFile(argv[1], &dataForAssembly))
    {
        return 1;
    }

    AsmReadFileAndCreateArrayOfPointers(&dataForAssembly);
    AsmInitLabels(&dataForAssembly);
    AsmInitHeaderForOutputCode(&dataForAssembly);

    if (AsmCompileByteCode(&dataForAssembly))
    {
        return 1;
    }

    AsmCompileByteCode(&dataForAssembly);   // Second compilation to convert labels into addresses.

    AsmWriteInformationToListingFile(&dataForAssembly, argv[1])

    AsmByteCodeDtor(&dataForAssembly);

    return 0;
}
