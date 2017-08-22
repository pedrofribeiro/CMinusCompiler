#ifndef _ASSEMBLY_CODE_H_
#define _ASSEMBLY_CODE_H_

#include "ASM.h"
#include "symtab.h"
#include "triple.h"

CPU_OPERATIONS recognizeOperation(triple* tr);

void instructionMatch(triple *tr);

void generateAssembly(triple* List);

#endif
