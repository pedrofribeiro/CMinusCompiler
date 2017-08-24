#ifndef _INTERMEDIARY_CODE_H_
#define _INTERMEDIARY_CODE_H_

#include "globals.h"
#include "symtab.h"

int TRACE_TREE_WALK;

#define VERBOSE if (TRACE_TREE_WALK == TRUE)


void evalStmt(TreeNode *node);

void evalExp(TreeNode *node);

void evalProgram(TreeNode *node);

void generateIntermediaryCode(TreeNode * t);

#endif
