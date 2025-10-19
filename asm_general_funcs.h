#ifndef ASM_GENERAL_FUNCS_HEADER
#define ASM_GENERAL_FUNCS_HEADER

#include "asm_structs.h"
#include "asm_commands.h"

int AsmCompileByteCode(DataForAssembly* dataForAssembly);

int AsmWriteLineToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly);

int AsmLineIsLabel(size_t numOfLine, DataForAssembly* dataForAssembly);

int AsmWriteCommandToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly, TypeOfArg* typeOfArg);

int AsmWriteParamNumberToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly);

int AsmWriteParamRegisterToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly);

int AsmWriteParamLabelToByteCode(size_t numOfLine, DataForAssembly* dataForAssembly);

int AsmIsRegisterInvalid(const char* tempRegister);

int AsmWriteByteCodeToBinFile(DataForAssembly* dataForAssembly, const char* outputFileName);

#endif
