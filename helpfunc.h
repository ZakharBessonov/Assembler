#ifndef HELPFUNC_HEADER
#define HELPFUNC_HEADER

void PrintErrorInfo(const char* file, const char* func, int line, const char* message, ...);

size_t SizeOfFile(FILE* fp);

size_t ReadFile(char** buffer, FILE* fp);

size_t CountNumLine(char* buffer, size_t lengthOfBuffer);

size_t FillArrayOfPointers(char* buffer, char*** arrayOfPointers, size_t lengthOfBuffer);

size_t ReadFileAndCreateArrayOfPointers(FILE* fp, char** buffer, char*** arrayOfPointers);

#endif
