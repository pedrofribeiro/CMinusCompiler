#ifndef _INTERMEDIARY_CODE_H_
#define _INTERMEDIARY_CODE_H_

#include "globals.h"
#include "symtab.h"
#include "ASM.h"

void evalStmt(TreeNode *node);

void evalExp(TreeNode *node);

void evalProgram(TreeNode *node);

void generateIntermediaryCode(TreeNode * t);

#endif
