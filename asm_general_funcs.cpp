#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm_consts.h"
#include "asm_general_funcs.h"
#include "asm_help_funcs.h"
#include "codes_of_commands.h"
#include "asm_structs.h"
#include "asm_hashes.h"

extern FILE* logfileAsm;
extern CommandAndHash tableOfHashes[];

int AsmCompileByteCode(DataForAssembly* dataForAssembly)
{
    dataForAssembly->lengthOfByteCode = HEADER_OFFSET;

    for (size_t numOfLine = 0; numOfLine < dataForAssembly->cntOfLines; numOfLine++)
    {
        dataForAssembly->lineShift = 0;

        if (AsmWriteLineToByteCode(numOfLine, dataForAssembly))
        {
            return 1;
        }
    }

    return 0;
}

int AsmWriteLineToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    if (AsmLineIsLabel(numOfLine, dataForAssembly))
    {
        return AsmProcessLabel(numOfLine, dataForAssembly);
    }

    TypeOfArg typeOfArg = PARAM_ZERO;
    if (AsmWriteCommandToByteCode(numOfLine, dataForAssembly, &typeOfArg))
    {
        return 1;
    }

    switch(typeOfArg)
    {
        case PARAM_ZERO:
            return 0;
        case PARAM_NUMBER:
            return AsmWriteParamNumberToByteCode(numOfLine, dataForAssembly);
        case PARAM_REGISTER:
            return AsmWriteParamRegisterToByteCode(numOfLine, dataForAssembly);
        case PARAM_LABEL:
            return AsmWriteParamLabelToByteCode(numOfLine, dataForAssembly);
        case PARAM_RAM_MEMORY:
            return AsmWriteParamRamMemoryToByteCode(numOfLine, dataForAssembly);
        default:
            return 1;
    }
}

int AsmLineIsLabel(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    return dataForAssembly->arrayOfPointers[numOfLine][0] == ':';
}

int AsmProcessLabel(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    int label = -1;
    AsmScanfLine(dataForAssembly, numOfLine, LABEL, &label);

    if (label <= -1 || label >= (int)MAX_CNT_OF_LABELS)
    {
        PRINT_LOG_FILE_ASM("ERROR: Invalid label on line %zu.\n", numOfLine + 1);
        return 1;
    }

    dataForAssembly->arrayOfLabels[label] = (int)dataForAssembly->lengthOfByteCode - HEADER_OFFSET;
    return 0;
}

int AsmWriteCommandToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly, TypeOfArg* typeOfArg)
{
    char command[MAX_SIZE_OF_COMMAND] = "";

    AsmScanfLine(dataForAssembly, numOfLine, STRING, command);
    unsigned long tempHash = AsmCountHashDjb2OfCommand(command);

    int indexInTableOfHashes = AsmBinSearchCommandByHash(tempHash);

    if (indexInTableOfHashes == -1 || strcmp(command, cmds[tableOfHashes[indexInTableOfHashes].code].name) != 0)
    {
        PRINT_LOG_FILE_ASM("ERROR: Invalid command on line %zu.\n", numOfLine + 1);
        return 1;
    }

    CodeOfCommand foundCode = tableOfHashes[indexInTableOfHashes].code;
    dataForAssembly->byteCode[dataForAssembly->lengthOfByteCode++] = foundCode;
    *typeOfArg = cmds[foundCode].type;

    return 0;
}

int AsmWriteParamNumberToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    int number = 0;
    if (AsmScanfLine(dataForAssembly, numOfLine, NUMBER, &number) <= 0)
    {
        PRINT_LOG_FILE_ASM("ERROR: Invalid number-argument on line %zu.\n", numOfLine + 1);
        return 1;
    }

    dataForAssembly->byteCode[dataForAssembly->lengthOfByteCode++] = number;

    return 0;
}

int AsmWriteParamRegisterToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    char tempRegister[RESERVE_VOLUME_FOR_STRINGS] = "";

    if (AsmScanfLine(dataForAssembly, numOfLine, STRING, tempRegister) <= 0 ||
        AsmIsRegisterInvalid(tempRegister))
    {
        PRINT_LOG_FILE_ASM("ERROR: Invalid register-argument on line %zu.\n", numOfLine + 1);
        return 1;
    }

    int numberOfRegister = 0;
    numberOfRegister = tempRegister[0] - 'A';

    dataForAssembly->byteCode[dataForAssembly->lengthOfByteCode++] = numberOfRegister;

    return 0;
}

int AsmWriteParamRamMemoryToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    char tempRamRegister[RESERVE_VOLUME_FOR_STRINGS] = "";

    if (AsmScanfLine(dataForAssembly, numOfLine, STRING, tempRamRegister) <= 0 ||
        AsmIsRamRegisterInvalid(tempRamRegister))
    {
        PRINT_LOG_FILE_ASM("ERROR: Invalid ram-memory-argument on line %zu.\n", numOfLine + 1);
        return 1;
    }

    int numberOfRegister = 0;
    numberOfRegister = tempRamRegister[1] - 'A';

    dataForAssembly->byteCode[dataForAssembly->lengthOfByteCode++] = numberOfRegister;

    return 0;
}

int AsmWriteParamLabelToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    int label = -1;

    if (AsmScanfLine(dataForAssembly, numOfLine, LABEL, &label) <= 0 || label < 0 || label >= (int)MAX_CNT_OF_LABELS)
    {
        PRINT_LOG_FILE_ASM("ERROR: Invalid label-argument on line %zu.\n", numOfLine + 1);
        return 1;
    }

    dataForAssembly->byteCode[dataForAssembly->lengthOfByteCode++] = dataForAssembly->arrayOfLabels[label];

    return 0;
}

int AsmIsRegisterInvalid(const char* tempRegister)
{
    return (strlen(tempRegister) != MAX_LENGTH_OF_REGISTER_NAME) || (tempRegister[0] < 'A') ||
           (tempRegister[0] >= 'A' + CNT_OF_REGISTERS) || tempRegister[1] != 'X';
}

int AsmIsRamRegisterInvalid(const char* tempRamRegister)
{
    return (strlen(tempRamRegister) != MAX_LENGTH_OF_REGISTER_NAME + 2) || (tempRamRegister[1] < 'A') ||
           (tempRamRegister[1] >= 'A' + CNT_OF_REGISTERS) || tempRamRegister[2] != 'X' || tempRamRegister[0] != '[' ||
           tempRamRegister[3] != ']';
}

int AsmWriteByteCodeToBinFile(DataForAssembly* dataForAssembly, const char* outputFileName)
{
    FILE* outputFile = fopen(outputFileName, "wb");
    if (outputFile == NULL)
    {
        PRINT_LOG_FILE_ASM("ERROR: An error occurred while opening outputFile.bin.\n");
        return 1;
    }

    fwrite(dataForAssembly->byteCode, sizeof(int), dataForAssembly->lengthOfByteCode, outputFile);
    fclose(outputFile);
    return 0;
}
