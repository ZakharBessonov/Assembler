#ifndef ASM_HELP_FUNCS_HEADER
#define ASM_HELP_FUNCS_HEADER

#include "asm_structs.h"

#define PRINT_LOG_FILE_ASM(X, ...) fprintf(logfileAsm, "%s:%d %s: " X,__FILE__, __LINE__, __func__, ##__VA_ARGS__);\
                                   fflush(logfileAsm)

int AsmOpenSourceFile(const char* sourceName, DataForAssembly* dataForAssembly);

size_t AsmReadFile(DataForAssembly* dataForAssembly);

size_t AsmCountNumLine(DataForAssembly* dataForAssembly);

size_t AsmFillArrayOfPointers(DataForAssembly* dataForAssembly);

void AsmReadFileAndCreateArrayOfPointers(DataForAssembly* dataForAssembly);

void AsmInitLabels(DataForAssembly* dataForAssembly);

void AsmInitHeaderForByteCode(DataForAssembly* dataForAssembly);

void AsmByteCodeDtor(DataForAssembly* dataForAssembly);

unsigned long AsmCountHashDjb2OfCommand(const char* command);

int AsmOpenLogFile();

void AsmCloseLogFile();

int Signum(unsigned long number1, unsigned long number2);

int AsmBinSearchCommandByHash(unsigned long hash);

int AsmScanfLine(DataForAssembly* dataForAssembly, size_t numOfLine, TypeOfReadArgument typeOfReadArgument, void* target);

#endif
