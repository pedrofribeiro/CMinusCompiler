#ifndef _ASM_H_
#define _ASM_H_

#include "globals.h"
#include "util.h"

typedef enum INSTR_TYPE {RTYPE, ITYPE, JTYPE, LTYPE} ASM_INSTR_TYPE;
typedef enum {ADD,SUB,MUL,DIV,RET,CALL,ATR,IF_F,GOTO,V_IN,EQL,DIFE,GRT,LST,GTE,LTE,MOVE,LW,ADDIU,SW,LI,NONE} CPU_OPERATIONS;
typedef enum {$acc, $zero, $sp, $fp, $ra, $t1, $NONE} Register;

int NUMBER_OF_ASM;

typedef struct ASM_RTYPE {
  CPU_OPERATIONS cpu_operation;
  Register rd;
  Register r1;
  Register r2;
} ASM_RTYPE;

typedef struct ASM_ITYPE {
  CPU_OPERATIONS cpu_operation;
  Register rd;
  Register r1;
  int immediate;
} ASM_ITYPE;

typedef struct ASM_JTYPE {
  CPU_OPERATIONS cpu_operation;
  int im_address;
} ASM_JTYPE;

typedef struct ASM_LTYPE {
  char functionName[10];
  int asmAddress;
} ASM_LTYPE;

typedef struct ASM_INSTR {
  ASM_INSTR_TYPE type;
  union{
    ASM_RTYPE rtype;
    ASM_ITYPE itype;
    ASM_JTYPE jtype;
    ASM_LTYPE ltype;
  };
  int asmNumber;
  struct ASM_INSTR* next;
}ASM_INSTR;

ASM_INSTR* asmList;
ASM_INSTR* tempAsm;

ASM_INSTR* createRTYPE(CPU_OPERATIONS cop, Register rd, Register r1, Register r2);
ASM_INSTR* createITYPE(CPU_OPERATIONS cop, Register rd, Register r1, int imm);
ASM_INSTR* createJTYPE(CPU_OPERATIONS cop, int addr);
ASM_INSTR* createLTYPE(char cop[], int addr);

void addASM(ASM_INSTR* t);

void initializeASMList();

#endif
