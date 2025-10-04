#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consts.h"
#include "assembler.h"
#include "helpfunc.h"
#include "registers.h"
#include "commands.h"

extern FILE* logfile;

int Assembler(const char* source)
{
    FILE* fp = fopen(source, "r");
    if (fp == NULL)
    {
        fprintf(logfile, "ERROR: Failed to open file \"%s\".\n", source);
        return 1;
    }

    char* buffer = NULL;
    char** arrayOfPointers = NULL;

    size_t cntOfLines = ReadFileAndCreateArrayOfPointers(fp, &buffer, &arrayOfPointers);

    int* output = (int*) calloc(2 * cntOfLines + 4, sizeof(int));

    // Создание заголовка
    output[0] = VERSION;
    output[1] = 20;
    output[2] = 2;
    output[3] = 2007;
    //

    size_t cntOfInstructions = 4;

    // Для отладки
    printf("%zu", cntOfLines);
    //

    for (size_t i = 0; i < cntOfLines; i++)
    {
        if (MoveCommandToOutput(arrayOfPointers[i], i, &cntOfInstructions, output))
        {
            return 1;
        }
    }

    fclose(fp);

    fp = fopen("execute.bin", "wb");
    fwrite(output, sizeof(int), cntOfInstructions, fp);

    fclose(fp);
    free(output);

    return 0;
}

int MoveCommandToOutput(const char* line, size_t numOfLine, size_t* cntOfInstructions, int* output)
{
    char command[MAX_SIZE_OF_COMMAND] = "";
    int tempArg = 0;
    char tempRegister[3] = "";
    int tempNumOfRegister = 0;

    int result = sscanf(line, "%s", command);
    if (result <= 0)
    {
        fprintf(logfile, "ERROR: Invalid command \"%s\" on line %zu.\n", command, numOfLine + 1);
        return 1;
    }

    if (strcmp(command, "push") == 0)
    {
        if (sscanf(line, "%*s %d", &tempArg) <= 0)
        {
            fprintf(logfile, "ERROR: Invalid arguments for \"%s\" on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = PUSH;
        output[(*cntOfInstructions)++] = tempArg;
        return 0;

    }
    else if (strcmp(command, "pushreg") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) <= 0 || (tempNumOfRegister = CheckRegister(tempRegister)) == -1)
        {
            fprintf(logfile, "ERROR: Invalid arguments for \"%s\" on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = PUSHREG;
        output[(*cntOfInstructions)++] = tempNumOfRegister;
        return 0;

    }
    else if (strcmp(command, "pop") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = POP;
        return 0;

    }
    else if (strcmp(command, "popreg") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) <= 0 || (tempNumOfRegister = CheckRegister(tempRegister)) == -1)
        {
            fprintf(logfile, "ERROR: Invalid arguments for \"%s\" on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = POPREG;
        output[(*cntOfInstructions)++] = tempNumOfRegister;
        return 0;

    }
    else if (strcmp(command, "add") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = ADD;
        return 0;

    }
    else if (strcmp(command, "sub") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = SUB;
        return 0;

    }
    else if (strcmp(command, "mul") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = MUL;
        return 0;

    }
    else if (strcmp(command, "div") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = DIV;
        return 0;

    }
    else if (strcmp(command, "mod") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = MOD;
        return 0;

    }
    else if (strcmp(command, "hlt") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = HLT;
        return 0;

    }
    else
    {
        fprintf(logfile, "ERROR: Invalid command \"%s\" on line %zu.\n", command, numOfLine + 1);
        return 1;
    }
}

int CheckRegister(char* tempRegister)
{
    for (int i = 0; i < (int)numOfRegisters; i++)
    {
        if (strcmp(tempRegister, registers[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}
