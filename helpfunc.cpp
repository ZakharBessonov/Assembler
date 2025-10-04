#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "consts.h"
#include "assembler.h"
#include "helpfunc.h"
#include "registers.h"

extern FILE* logfile;
extern FILE* input;

void PrintErrorInfo(const char* file, const char* func, int line, const char* message, ...)
{
    time_t var1 = time(NULL);
    fprintf(logfile, "%.24s %s %s:%d ", ctime(&var1), func, file, line);

    va_list arg_ptr;
    va_start(arg_ptr, message);
    vfprintf(logfile, message, arg_ptr);
    fflush(logfile);
    va_end(arg_ptr);
}

size_t SizeOfFile(FILE* fp)
{
    int descriptor = fileno(fp);
    struct stat statistics = {};
    fstat(descriptor, &statistics);
    return (size_t)statistics.st_size;
}

size_t ReadFile(char** buffer, FILE* fp)
{
    // Узнаём размер файла
    size_t sizeOfFile = SizeOfFile(fp);
    // Узнали

    *buffer = (char*) calloc(sizeOfFile + 2, 1);
    assert(buffer != NULL);

    return (size_t)read(fileno(fp), *buffer, sizeOfFile);
}

size_t CountNumLine(char* buffer, size_t lengthOfBuffer)
{
    assert(buffer != NULL);

    size_t ans = 0;
    for (size_t i = 0; i < lengthOfBuffer; i++)
    {
        if (buffer[i] == '\n')
        {
            ans++;
            buffer[i] = '\0';
        }
    }

    return ans + 1;
}

size_t FillArrayOfPointers(char* buffer, char*** arrayOfPointers, size_t lengthOfBuffer)
{
    assert(buffer != NULL);
    assert(arrayOfPointers != NULL);

    size_t numOfLine = CountNumLine(buffer, lengthOfBuffer);
    (*arrayOfPointers) = (char**) calloc(numOfLine + 1, sizeof(char*));
    assert((*arrayOfPointers) != NULL);

    char* leftPosition = buffer;
    int j = 0;
    for (size_t i = 0; i < lengthOfBuffer; i++)
    {
        if (buffer[i] == '\0')
        {
            (*arrayOfPointers)[j] = leftPosition;
            leftPosition = &buffer[i+1];
            j++;
        }
    }

    (*arrayOfPointers)[j] = leftPosition;

    return numOfLine;
}

size_t ReadFileAndCreateArrayOfPointers(FILE* fp, char** buffer, char*** arrayOfPointers)
{
    assert(fp != NULL);
    assert(buffer != NULL);
    assert(arrayOfPointers != NULL);

    size_t lengthOfBuffer = ReadFile(buffer, fp);

    if (lengthOfBuffer == 0)
    {
        return 0;
    }
    while (isspace((*buffer)[lengthOfBuffer - 1]) || (*buffer)[lengthOfBuffer - 1] == '\0')
    {
        lengthOfBuffer--;
        if (lengthOfBuffer == 0)
        {
            return 0;
        }
    }
    (*buffer)[lengthOfBuffer++] = '\0';

    return FillArrayOfPointers(*buffer, arrayOfPointers, lengthOfBuffer);
}

