#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm_consts.h"
#include "asm_general_funcs.h"
#include "asm_help_funcs.h"
#include "asm_commands.h"
#include "asm_structs.h"

extern FILE* logfileAsm;

int AsmCompileByteCode(DataForAssembly* dataForAssembly)
{
    dataForAssembly->lengthOfByteCode = HEADER_OFFSET;

    for (size_t numOfLine = 0; numOfLine < dataForAssembly->cntOfLines; numOfLine++)
    {
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
        return 0;
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
        default:
            return 1;
    }
}

int AsmLineIsLabel(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    if (dataForAssembly->arrayOfPointers[numOfLine][0] != ':')
    {
        return 0;
    }

    int label = -1;
    sscanf(dataForAssembly->arrayOfPointers[numOfLine], ":%d", &label);

    if (label == -1 || label >= (int)MAX_CNT_OF_LABELS)
    {
        fprintf(logfileAsm, "ERROR: Invalid label on line %zu.\n", numOfLine + 1);
        return 0;
    }

    dataForAssembly->arrayOfLabels[label] = (int)dataForAssembly->lengthOfByteCode - HEADER_OFFSET;
    return 1;
}

int AsmWriteCommandToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly, TypeOfArg* typeOfArg)
{
    char command[MAX_SIZE_OF_COMMAND] = "";

    sscanf(dataForAssembly->arrayOfPointers[numOfLine], "%s", command);

    for (int i = 0; i < CNT_OF_COMMANDS; i++)
    {
        if (strcmp(command, cmds[i].name) == 0)
        {
            dataForAssembly->byteCode[dataForAssembly->lengthOfByteCode++] = cmds[i].code;
            *typeOfArg = cmds[i].type;
            return 0;
        }
    }

    fprintf(logfileAsm, "ERROR: Invalid command on line %zu.\n", numOfLine + 1);

    return 1;
}

int AsmWriteParamNumberToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    int number = 0;
    if (sscanf(dataForAssembly->arrayOfPointers[numOfLine], "%*s %d", &number) <= 0)
    {
        fprintf(logfileAsm, "ERROR: Invalid number-argument on line %zu.\n", numOfLine + 1);
        return 1;
    }

    dataForAssembly->byteCode[dataForAssembly->lengthOfByteCode++] = number;

    return 0;
}

int AsmWriteParamRegisterToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    char tempRegister[RESERVE_VOLUME_FOR_STRINGS] = "";
    if (sscanf(dataForAssembly->arrayOfPointers[numOfLine], "%*s %s", tempRegister) <= 0 || AsmIsRegisterInvalid(tempRegister))
    {
        fprintf(logfileAsm, "ERROR: Invalid register-argument on line %zu.\n", numOfLine + 1);
        return 1;
    }

    int numberOfRegister = 0;
    if (tempRegister[0] == '[')
    {
        numberOfRegister = tempRegister[1] - 'A';
    }
    else
    {
        numberOfRegister = tempRegister[0] - 'A';
    }

    dataForAssembly->byteCode[dataForAssembly->lengthOfByteCode++] = numberOfRegister;

    return 0;
}

int AsmWriteParamLabelToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly)
{
    int label = -1;
    if (sscanf(dataForAssembly->arrayOfPointers[numOfLine], "%*s :%d", &label) <= 0 || label < 0 || label >= (int)MAX_CNT_OF_LABELS)
    {
        fprintf(logfileAsm, "ERROR: Invalid label-argument on line %zu.\n", numOfLine + 1);
        return 1;
    }

    dataForAssembly->byteCode[dataForAssembly->lengthOfByteCode++] = dataForAssembly->arrayOfLabels[label];

    return 0;
}

int AsmIsRegisterInvalid(const char* tempRegister)
{
    if (tempRegister[0] == '[')
    {
        return (strlen(tempRegister) != MAX_LENGTH_OF_REGISTER_NAME + 2) || (tempRegister[1] < 'A')
            || (tempRegister[1] >= 'A' + CNT_OF_REGISTERS);
    }

    return (strlen(tempRegister) != MAX_LENGTH_OF_REGISTER_NAME) || (tempRegister[0] < 'A')
            || (tempRegister[0] >= 'A' + CNT_OF_REGISTERS);
}

int AsmWriteByteCodeToBinFile(DataForAssembly* dataForAssembly, const char* outputFileName)
{
    FILE* outputFile = fopen(outputFileName, "wb");
    if (outputFile == NULL)
    {
        fprintf(logfileAsm, "ERROR: An error occurred while opening outputFile.bin.\n");
        return 1;
    }

    fwrite(dataForAssembly->byteCode, sizeof(int), dataForAssembly->lengthOfByteCode, outputFile);
    fclose(outputFile);
    return 0;
}
