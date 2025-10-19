#ifndef ASM_HELP_FUNCS_HEADER
#define ASM_HELP_FUNCS_HEADER

#include "asm_structs.h"

size_t AsmSizeOfFile(FILE* fp);

int AsmOpenSourceFile(const char* sourceName, DataForAssembly* dataForAssembly);

size_t AsmReadFile(DataForAssembly* dataForAssembly);

size_t AsmCountNumLine(DataForAssembly* dataForAssembly);

size_t AsmFillArrayOfPointers(DataForAssembly* dataForAssembly);

void AsmReadFileAndCreateArrayOfPointers(DataForAssembly* dataForAssembly);

void AsmInitLabels(DataForAssembly* dataForAssembly);

void AsmInitHeaderForByteCode(DataForAssembly* dataForAssembly);

void AsmByteCodeDtor(DataForAssembly* dataForAssembly);

int AsmOpenLogFile();

void AsmCloseLogFile();

#endif
