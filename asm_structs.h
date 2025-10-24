#ifndef ASM_STRUCTS_HEADER
#define ASM_STRUCTS_HEADER

#include "asm_consts.h"

struct DataForAssembly
{
    FILE*   source;
    char*   buffer;
    char**  arrayOfPointers;
    size_t  cntOfLines;
    int     lineShift;
    int     arrayOfLabels[MAX_CNT_OF_LABELS];
    int*    byteCode;
    size_t  lengthOfByteCode;
};

enum TypeOfReadArgument
{
    NUMBER,
    LABEL,
    STRING
};

#endif
