#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "consts.h"
#include "assembler.h"
#include "helpfunc.h"
#include "registers.h"

FILE* logfile = NULL;

int main(int argc, const char *argv[])
{
    logfile = fopen(LOG_FILE_NAME, "w");
    if (argc <= 1)
    {
        PrintErrorInfo(__FILE__, __func__, __LINE__, "The file name to assemble was not passed.\n");
        return 1;
    }
    Assembler(argv[1]);

    fclose(logfile);

    return 0;
}
