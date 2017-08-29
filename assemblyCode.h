#ifndef _ASSEMBLY_CODE_H_
#define _ASSEMBLY_CODE_H_

#include "ASM.h"
#include "symtab.h"
#include "triple.h"

Operation getOperation(triple* tr);

void asmCode(triple *tr);

void cleanTriples();

void generateAssembly(triple* List);

#endif
