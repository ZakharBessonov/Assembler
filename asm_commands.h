#ifndef ASM_COMMANDS_HEADER
#define ASM_COMMANDS_HEADER

#include "codes_of_commands.h"

enum TypeOfArg
{
    PARAM_ZERO,
    PARAM_NUMBER,
    PARAM_REGISTER,
    PARAM_LABEL
};

struct AsmCommand
{
    char            name[MAX_SIZE_OF_COMMAND];
    TypeOfArg       type;
    CodeOfCommand   code;
};

const AsmCommand cmds[] = {
    {"hlt",     PARAM_ZERO,     CMD_HLT     },
    {"push",    PARAM_NUMBER,   CMD_PUSH    },
    {"pushreg", PARAM_REGISTER, CMD_PUSHREG },
    {"pop",     PARAM_ZERO,     CMD_POP     },
    {"popreg",  PARAM_REGISTER, CMD_POPREG  },
    {"out",     PARAM_ZERO,     CMD_OUT     },
    {"add",     PARAM_ZERO,     CMD_ADD     },
    {"sub",     PARAM_ZERO,     CMD_SUB     },
    {"mul",     PARAM_ZERO,     CMD_MUL     },
    {"div",     PARAM_ZERO,     CMD_DIV     },
    {"mod",     PARAM_ZERO,     CMD_MOD     },
    {"sqr",     PARAM_ZERO,     CMD_SQR     },
    {"jmp",     PARAM_LABEL,    CMD_JMP     },
    {"jb",      PARAM_LABEL,    CMD_JB      },
    {"jbe",     PARAM_LABEL,    CMD_JBE     },
    {"ja",      PARAM_LABEL,    CMD_JA      },
    {"jae",     PARAM_LABEL,    CMD_JAE     },
    {"je",      PARAM_LABEL,    CMD_JE      },
    {"jne",     PARAM_LABEL,    CMD_JNE     },
    {"in",      PARAM_ZERO,     CMD_IN      },
    {"dmp",     PARAM_ZERO,     CMD_DMP     },
    {"call",    PARAM_LABEL,    CMD_CALL    },
    {"ret",     PARAM_ZERO,     CMD_RET     },
    {"outc",    PARAM_ZERO,     CMD_OUTC    },
    {"pushm",   PARAM_REGISTER, CMD_PUSHM   },
    {"popm",    PARAM_REGISTER, CMD_POPM    },
    {"draw",    PARAM_ZERO,     CMD_DRAW    },
};

#endif
