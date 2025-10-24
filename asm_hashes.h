#ifndef ASM_HASHES_HEADER
#define ASM_HASHES_HEADER

#include "codes_of_commands.h"

struct CommandAndHash
{
    CodeOfCommand   code;
    unsigned long   hash;
};

void AsmCountHashesDjb2();

int CompareHashesDjb2(const void* hash1, const void* hash2);

void AsmCountAndSortHashesDjb2();

#endif
