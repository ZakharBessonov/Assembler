#include <stdio.h>
#include <stdlib.h>

#include "asm_hashes.h"
#include "asm_help_funcs.h"
#include "asm_structs.h"
#include "codes_of_commands.h"

CommandAndHash tableOfHashes[] =
{
    {CMD_HLT     ,0 },
    {CMD_PUSH    ,0 },
    {CMD_PUSHREG ,0 },
    {CMD_POP     ,0 },
    {CMD_POPREG  ,0 },
    {CMD_OUT     ,0 },
    {CMD_ADD     ,0 },
    {CMD_SUB     ,0 },
    {CMD_MUL     ,0 },
    {CMD_DIV     ,0 },
    {CMD_MOD     ,0 },
    {CMD_SQR     ,0 },
    {CMD_JMP     ,0 },
    {CMD_JB      ,0 },
    {CMD_JBE     ,0 },
    {CMD_JA      ,0 },
    {CMD_JAE     ,0 },
    {CMD_JE      ,0 },
    {CMD_JNE     ,0 },
    {CMD_IN      ,0 },
    {CMD_DMP     ,0 },
    {CMD_CALL    ,0 },
    {CMD_RET     ,0 },
    {CMD_OUTC    ,0 },
    {CMD_PUSHM   ,0 },
    {CMD_POPM    ,0 },
    {CMD_DRAW    ,0 }
};

void AsmCountHashesDjb2()
{
    for (int i = 0; i < CNT_OF_COMMANDS; i++)
    {
        tableOfHashes[i].hash = AsmCountHashDjb2OfCommand(cmds[i].name);
    }
}

int CompareHashesDjb2(const void* hash1, const void* hash2)
{
    const CommandAndHash commandAndHash1 = *(const CommandAndHash*)hash1;
    const CommandAndHash commandAndHash2 = *(const CommandAndHash*)hash2;

    return Signum(commandAndHash1.hash, commandAndHash2.hash);
}

void AsmCountAndSortHashesDjb2()
{
    AsmCountHashesDjb2();
    qsort(tableOfHashes, CNT_OF_COMMANDS, sizeof(CommandAndHash), CompareHashesDjb2);
}
