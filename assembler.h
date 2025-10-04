#ifndef ASSEMBLER_HEADER
#define ASSEMBLER_HEADER

int Assembler(const char* source);

int MoveCommandToOutput(const char* line, size_t numOfLine, size_t* cntOfInstructions, int* output);

int CheckRegister(char* tempRegister);

#endif
