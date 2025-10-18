#ifndef ASM_STRUCTS_HEADER
#define ASM_STRUCTS_HEADER

struct DataForAssembly
{
    FILE*   source;
    char*   buffer;
    char**  arrayOfPointers;
    size_t  cntOfLines;
    int     arrayOfLabels[MAX_CNT_OF_LABELS];
    int*    byteCode;
    size_t  lengthOfByteCode;
}

#endif
