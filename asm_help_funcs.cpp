#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "asm_consts.h"
#include "asm_help_funcs.h"
#include "commands.h"
#include "asm_structs.h"

extern FILE* logfileAsm;

size_t AsmSizeOfFile(FILE* fp)
{
    int descriptor = fileno(fp);
    struct stat statistics = {};
    fstat(descriptor, &statistics);
    return (size_t)statistics.st_size;
}

int AsmOpenSourceFile(const char* sourceName, DataForAssembly* dataForAssembly)
{
    dataForAssembly->source = fopen(sourceName, "r");
    if (fp == NULL)
    {
        fprintf(logfileAsm, "ERROR: Failed to open file \"%s\".\n", sourceName);
        return 1;
    }

    return 0;
}

size_t AsmReadFile(DataForAssembly* dataForAssembly)
{
    size_t sizeOfFile = AsmSizeOfFile(dataForAssembly->source);
    dataForAssembly->buffer = (char*) calloc(sizeOfFile + 2, 1);

    if (dataForAssembly->buffer == NULL)
    {
        fprintf(logfileAsm, "ERROR: Failed to allocate memory for dataForAssembly->buffer.\n");
        return 1;
    }

    return (size_t)read(fileno(dataForAssembly->source), dataForAssembly->buffer, sizeOfFile);
}

size_t AsmCountNumLine(DataForAssembly* dataForAssembly, size_t lengthOfBuffer)
{
    size_t ans = 0;
    for (size_t i = 0; i < lengthOfBuffer; i++)
    {
        if (dataForAssembly->buffer[i] == '\n')
        {
            ans++;
            dataForAssembly->buffer[i] = '\0';
        }
    }

    return ans + 1;
}

size_t AsmFillArrayOfPointers(DataForAssembly* dataForAssembly, size_t lengthOfBuffer)
{
    size_t numOfLine = AsmCountNumLine(dataForAssembly, lengthOfBuffer);
    dataForAssembly->arrayOfPointers = (char**) calloc(numOfLine + 1, sizeof(char*));

    if (dataForAssembly->arrayOfPointers == NULL)
    {
        fprintf(logfileAsm, "ERROR: Failed to allocate memory for dataForAssembly->arrayOfPointers.\n");
        return 1;
    }

    char* leftPosition = dataForAssembly->buffer;
    int j = 0;
    for (size_t i = 0; i < lengthOfBuffer; i++)
    {
        if (dataForAssembly->buffer[i] == '\0')
        {
            dataForAssembly->arrayOfPointers[j] = leftPosition;
            leftPosition = &dataForAssembly->buffer[i+1];
            j++;
        }
    }

    dataForAssembly->arrayOfPointers[j] = leftPosition;

    return numOfLine;
}

void AsmReadFileAndCreateArrayOfPointers(DataForAssembly* dataForAssembly)
{
    size_t lengthOfBuffer = AsmReadFile(dataForAssembly);

    if (lengthOfBuffer == 0)
    {
        return;
    }

    // Remove whitespace characters from the end of the file
    while (isspace(dataForAssembly->buffer[lengthOfBuffer - 1]) || dataForAssembly->buffer[lengthOfBuffer - 1] == '\0')
    {
        lengthOfBuffer--;
        if (lengthOfBuffer == 0)
        {
            return;
        }
    }
    (*buffer)[lengthOfBuffer++] = '\0';

    dataForAssembly->cntOfLines = AsmFillArrayOfPointers(dataForAssembly, lengthOfBuffer);
}

void AsmInitLabels(DataForAssembly* dataForAssembly)
{
    for (size_t i = 0; i < MAX_CNT_OF_LABELS; i++)
    {
        dataForAssembly->arrayOfLabels[i] = -1;
    }
}

void AsmInitHeaderForOutput(DataForAssembly* dataForAssembly)
{
    dataForAssembly->byteCode = (int*) calloc(2 * dataForAssembly->cntOfLines + 4, sizeof(int));

    dataForAssembly->byteCode[0] = VERSION;
    dataForAssembly->byteCode[1] = SIGNATURE_1;
    dataForAssembly->byteCode[2] = SIGNATURE_2;
    dataForAssembly->byteCode[3] = SIGNATURE_3;

    dataForAssembly->lengthOfOutput = 4;
}

void AsmByteCodeDtor(DataForAssembly* dataForAssembly)
{
    fclose(dataForAssembly->source);
    free(dataForAssembly->arrayOfPointers);
    free(dataForAssembly->buffer);
    free(dataForAssembly->byteCode);
    dataForAssembly->cntOfLines = 0;
    dataForAssembly->lengthOfByteCode = 0;
}
