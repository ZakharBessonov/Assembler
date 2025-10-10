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

    int arrayOfLabels[MAX_CNT_OF_LABELS] = {};
    for (size_t i = 0; i < MAX_CNT_OF_LABELS; i++)
    {
        arrayOfLabels[i] = -1;
    }

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
        if (MoveCommandToOutput(arrayOfPointers[i], arrayOfLabels, i, &cntOfInstructions, output))
        {
            return 1;
        }
    }

    fclose(fp);

    if (InstallPointersInsteadOfLabels(output, arrayOfLabels, cntOfInstructions))
    {
        free(output);
        return 1;
    }

    fp = fopen("execute.bin", "wb");
    fwrite(output, sizeof(int), cntOfInstructions, fp);

    fclose(fp);
    free(output);

    return 0;
}

int MoveCommandToOutput(const char* line, int* arrayOfLabels, size_t numOfLine, size_t* cntOfInstructions, int* output)
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

    if (command[0] == ':')
    {
        int label = 0;
        if (sscanf(line, ":%d", &label) <= 0 || label <= 0 || (size_t)label > MAX_CNT_OF_LABELS)
        {
            fprintf(logfile, "ERROR: Invalid label on line %zu.\n", numOfLine + 1);
            return 1;
        }

        arrayOfLabels[label - 1] = (int)*cntOfInstructions - 4;  // -4 because fist four numbers is title
        return 0;

    }
    else if (strcmp(command, "push") == 0)
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
    else if (strcmp(command, "out") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = OUT;
        return 0;

    }
    else if (strcmp(command, "dmp") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = DMP;
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
    else if (strcmp(command, "in") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = IN;
        return 0;

    }
    else if (strcmp(command, "sqr") == 0)
    {
        if (sscanf(line, "%*s %s", tempRegister) > 0)
        {
            fprintf(logfile, "ERROR: %s no need argument on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = SQR;
        return 0;

    }
    else if (strcmp(command, "jmp") == 0)
    {
        if (sscanf(line, "%*s :%d", &tempArg) > 0)       // If there is a colon, then it means a label.
        {
            if (tempArg <= 0 || (size_t)tempArg > MAX_CNT_OF_LABELS)
            {
                fprintf(logfile, "ERROR: Invalid label for \"%s\" on line %zu.\n", command, numOfLine + 1);
                return 1;
            }
            output[(*cntOfInstructions)++] = JMP;
            output[(*cntOfInstructions)++] = -tempArg;
            return 0;
        }

        if (sscanf(line, "%*s %d", &tempArg) <= 0 || tempArg < 0)
        {
            fprintf(logfile, "ERROR: Invalid arguments for \"%s\" on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = JMP;
        output[(*cntOfInstructions)++] = tempArg;
        return 0;

    }
    else if (strcmp(command, "jb") == 0)
    {
        if (sscanf(line, "%*s :%d", &tempArg) > 0)       // If there is a colon, then it means a label.
        {
            if (tempArg <= 0 || (size_t)tempArg > MAX_CNT_OF_LABELS)
            {
                fprintf(logfile, "ERROR: Invalid label for \"%s\" on line %zu.\n", command, numOfLine + 1);
                return 1;
            }
            output[(*cntOfInstructions)++] = JB;
            output[(*cntOfInstructions)++] = -tempArg;
            return 0;
        }

        if (sscanf(line, "%*s %d", &tempArg) <= 0 || tempArg < 0)
        {
            fprintf(logfile, "ERROR: Invalid arguments for \"%s\" on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = JB;
        output[(*cntOfInstructions)++] = tempArg;
        return 0;

    }
    else if (strcmp(command, "jbe") == 0)
    {
        if (sscanf(line, "%*s :%d", &tempArg) > 0)       // If there is a colon, then it means a label.
        {
            if (tempArg <= 0 || (size_t)tempArg > MAX_CNT_OF_LABELS)
            {
                fprintf(logfile, "ERROR: Invalid label for \"%s\" on line %zu.\n", command, numOfLine + 1);
                return 1;
            }
            output[(*cntOfInstructions)++] = JBE;
            output[(*cntOfInstructions)++] = -tempArg;
            return 0;
        }

        if (sscanf(line, "%*s %d", &tempArg) <= 0 || tempArg < 0)
        {
            fprintf(logfile, "ERROR: Invalid arguments for \"%s\" on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = JBE;
        output[(*cntOfInstructions)++] = tempArg;
        return 0;

    }
    else if (strcmp(command, "ja") == 0)
    {
        if (sscanf(line, "%*s :%d", &tempArg) > 0)       // If there is a colon, then it means a label.
        {
            if (tempArg <= 0 || (size_t)tempArg > MAX_CNT_OF_LABELS)
            {
                fprintf(logfile, "ERROR: Invalid label for \"%s\" on line %zu.\n", command, numOfLine + 1);
                return 1;
            }
            output[(*cntOfInstructions)++] = JA;
            output[(*cntOfInstructions)++] = -tempArg;
            return 0;
        }

        if (sscanf(line, "%*s %d", &tempArg) <= 0 || tempArg < 0)
        {
            fprintf(logfile, "ERROR: Invalid arguments for \"%s\" on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = JA;
        output[(*cntOfInstructions)++] = tempArg;
        return 0;

    }
    else if (strcmp(command, "jae") == 0)
    {
        if (sscanf(line, "%*s :%d", &tempArg) > 0)       // If there is a colon, then it means a label.
        {
            if (tempArg <= 0 || (size_t)tempArg > MAX_CNT_OF_LABELS)
            {
                fprintf(logfile, "ERROR: Invalid label for \"%s\" on line %zu.\n", command, numOfLine + 1);
                return 1;
            }
            output[(*cntOfInstructions)++] = JAE;
            output[(*cntOfInstructions)++] = -tempArg;
            return 0;
        }

        if (sscanf(line, "%*s %d", &tempArg) <= 0 || tempArg < 0)
        {
            fprintf(logfile, "ERROR: Invalid arguments for \"%s\" on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = JAE;
        output[(*cntOfInstructions)++] = tempArg;
        return 0;

    }
    else if (strcmp(command, "je") == 0)
    {
        if (sscanf(line, "%*s :%d", &tempArg) > 0)       // If there is a colon, then it means a label.
        {
            if (tempArg <= 0 || (size_t)tempArg > MAX_CNT_OF_LABELS)
            {
                fprintf(logfile, "ERROR: Invalid label for \"%s\" on line %zu.\n", command, numOfLine + 1);
                return 1;
            }
            output[(*cntOfInstructions)++] = JE;
            output[(*cntOfInstructions)++] = -tempArg;
            return 0;
        }

        if (sscanf(line, "%*s %d", &tempArg) <= 0 || tempArg < 0)
        {
            fprintf(logfile, "ERROR: Invalid arguments for \"%s\" on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = JE;
        output[(*cntOfInstructions)++] = tempArg;
        return 0;

    }
    else if (strcmp(command, "jne") == 0)
    {
        if (sscanf(line, "%*s :%d", &tempArg) > 0)       // If there is a colon, then it means a label.
        {
            if (tempArg <= 0 || (size_t)tempArg > MAX_CNT_OF_LABELS)
            {
                fprintf(logfile, "ERROR: Invalid label for \"%s\" on line %zu.\n", command, numOfLine + 1);
                return 1;
            }
            output[(*cntOfInstructions)++] = JNE;
            output[(*cntOfInstructions)++] = -tempArg;
            return 0;
        }

        if (sscanf(line, "%*s %d", &tempArg) <= 0 || tempArg < 0)
        {
            fprintf(logfile, "ERROR: Invalid arguments for \"%s\" on line %zu.\n", command, numOfLine + 1);
            return 1;
        }
        output[(*cntOfInstructions)++] = JNE;
        output[(*cntOfInstructions)++] = tempArg;
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

int InstallPointersInsteadOfLabels(int* output, int* arrayOfLabels, size_t cntOfInstructions)
{
    int label = 0, address = 0;

    for (size_t i = 4; i < cntOfInstructions; i++)
    {
        if (IsThereCommandIAmongListed(output[i], 7, JMP, JB, JBE, JA, JAE, JE, JNE))
        {
            if (output[i + 1] < 0)
            {
                label = -output[i + 1];
                if (arrayOfLabels[label - 1] < 0)
                {
                    fprintf(logfile, "ERROR: Label %d was not initialized.", label);
                    return 1;
                }

                output[i + 1] = arrayOfLabels[label - 1];
                i++;
            }
            else
            {
                address = output[i + 1];
                if ((size_t)address >= cntOfInstructions - 4)       // -4 because fist four numbers is title
                {
                    fprintf(logfile, "ERROR: Label %d was not initialized.", label);
                    return 1;
                }
                i++;
            }
        }
        else if (IsThereCommandIAmongListed(output[i], 3, PUSH, PUSHREG, POPREG))
        {
            i++;
        }
    }

    return 0;
}
