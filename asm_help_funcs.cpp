#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "asm_consts.h"
#include "asm_help_funcs.h"
#include "codes_of_commands.h"
#include "asm_structs.h"
#include "asm_hashes.h"
#include "../size_of_file.h"


extern FILE* logfileAsm;
extern CommandAndHash tableOfHashes[];

static size_t AsmCountNumLine(DataForAssembly* dataForAssembly, size_t lengthOfBuffer)
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

int AsmOpenSourceFile(const char* sourceName, DataForAssembly* dataForAssembly)
{
    dataForAssembly->source = fopen(sourceName, "r");
    if (dataForAssembly->source == NULL)
    {
        PRINT_LOG_FILE_ASM("ERROR: Failed to open file \"%s\".\n", sourceName);
        return 1;
    }

    return 0;
}

size_t AsmReadFile(DataForAssembly* dataForAssembly)
{
    size_t sizeOfFile = SizeOfFile(dataForAssembly->source);
    dataForAssembly->buffer = (char*) calloc(sizeOfFile + 2, 1);

    if (dataForAssembly->buffer == NULL)
    {
        PRINT_LOG_FILE_ASM("ERROR: Failed to allocate memory for dataForAssembly->buffer.\n");
        return 1;
    }

    return (size_t)read(fileno(dataForAssembly->source), dataForAssembly->buffer, sizeOfFile);
}

static size_t AsmFillArrayOfPointers(DataForAssembly* dataForAssembly, size_t lengthOfBuffer)
{
    size_t numOfLine = AsmCountNumLine(dataForAssembly, lengthOfBuffer);
    dataForAssembly->arrayOfPointers = (char**) calloc(numOfLine + 1, sizeof(char*));

    if (dataForAssembly->arrayOfPointers == NULL)
    {
        PRINT_LOG_FILE_ASM("ERROR: Failed to allocate memory for dataForAssembly->arrayOfPointers.\n");
        return 1;
    }

    bool isStringNotEmpty = false;
    char* leftPosition = dataForAssembly->buffer;
    int j = 0;
    for (size_t i = 0; i < lengthOfBuffer; i++)
    {
        if (!isspace(dataForAssembly->buffer[i]) && dataForAssembly->buffer[i] != '\0')
        {
            isStringNotEmpty = true;
        }

        if (dataForAssembly->buffer[i] == '\0' && isStringNotEmpty)
        {
            dataForAssembly->arrayOfPointers[j++] = leftPosition;
            leftPosition = &dataForAssembly->buffer[i+1];
            isStringNotEmpty = false;
        }
        else if (!isStringNotEmpty)
        {
            leftPosition = &dataForAssembly->buffer[i+1];
            numOfLine--;
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
    dataForAssembly->buffer[lengthOfBuffer++] = '\0';

    dataForAssembly->cntOfLines = AsmFillArrayOfPointers(dataForAssembly, lengthOfBuffer);
}

void AsmInitLabels(DataForAssembly* dataForAssembly)
{
    for (size_t i = 0; i < MAX_CNT_OF_LABELS; i++)
    {
        dataForAssembly->arrayOfLabels[i] = -1;
    }
}

void AsmInitHeaderForByteCode(DataForAssembly* dataForAssembly)
{
    dataForAssembly->byteCode = (int*) calloc(2 * dataForAssembly->cntOfLines + 4, sizeof(int));

    dataForAssembly->byteCode[0] = VERSION;
    dataForAssembly->byteCode[1] = SIGNATURE_1;
    dataForAssembly->byteCode[2] = SIGNATURE_2;
    dataForAssembly->byteCode[3] = SIGNATURE_3;

    dataForAssembly->lengthOfByteCode = HEADER_OFFSET;
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

unsigned long AsmCountHashDjb2OfCommand(const char* command)
{
    unsigned long hash = 5381;

    while (*command != '\0')
    {
        hash = ((hash << 5) + hash) + (unsigned long)(*command);
        command++;
    }

    return hash;
}

int AsmOpenLogFile()
{
    logfileAsm = fopen(LOG_FILE_NAME_ASM, "w");
    if (logfileAsm == NULL)
    {
        printf("ERROR: An error occurred while opening logfileAsm.\n");
        return 1;
    }

    return 0;
}

void AsmCloseLogFile()
{
    fclose(logfileAsm);
}

int Signum(unsigned long number1, unsigned long number2)
{
    if (number1 < number2)
    {
        return -1;
    }
    else if (number1 > number2)
    {
        return 1;
    }

    return 0;
}

int AsmBinSearchCommandByHash(unsigned long hash)
{
    int leftBorder = 0;
    int rightBorder = CNT_OF_COMMANDS;
    int middle = 0;

    for (int i = 0; i < 27; i++)
    {
        printf("%d \"%s\": %lu\n", i, cmds[tableOfHashes[i].code].name, tableOfHashes[i].hash);
    }
    printf("\n\n");

    while (rightBorder - leftBorder > 1)
    {
        middle = (leftBorder + rightBorder) / 2;
        //
        printf("L: %-10d     R: %-10d\n", leftBorder, rightBorder);
        //

        if (tableOfHashes[middle].hash > hash)
        {
            rightBorder = middle;
        }
        else
        {
            leftBorder = middle;
        }
    }

    //
        printf("L: %-10d     R: %-10d\n", leftBorder, rightBorder);
    //

    if (tableOfHashes[leftBorder].hash == hash)
    {
        return leftBorder;
    }

    return -1;
}

int AsmScanfLine(DataForAssembly* dataForAssembly, size_t numOfLine, TypeOfReadArgument typeOfReadArgument, void* target)
{
    int cntOfReadChars = 0;
    int result = 0;
    int shift = dataForAssembly->lineShift;
    char* strPt = NULL;
    int* intPt = NULL;

    switch(typeOfReadArgument)
    {
        case STRING:
            strPt = (char*)target;
            result = sscanf(dataForAssembly->arrayOfPointers[numOfLine] + shift, "%s%n", strPt, &cntOfReadChars);
            break;
        case NUMBER:
            intPt = (int*)target;
            result = sscanf(dataForAssembly->arrayOfPointers[numOfLine] + shift, "%d%n", intPt, &cntOfReadChars);
            break;
        case LABEL:
            intPt = (int*)target;
            result = sscanf(dataForAssembly->arrayOfPointers[numOfLine] + shift, " :%d%n", intPt, &cntOfReadChars);
            break;
        default:
            break;
    }

    dataForAssembly->lineShift += cntOfReadChars;
    return result;
}

