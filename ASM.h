#ifndef _ASM_H_
#define _ASM_H_

#include "globals.h"
#include "util.h"

typedef enum INSTR_TYPE {RTYPE, ITYPE, JTYPE} ASM_INSTR_TYPE;
enum {ADD,SUB,MUL,DIV,RET,CALL,ATR,IF_F,GOTO,V_IN,EQL,DIFE,GRT,LST,GTE,LTE} knownOperations;

int NUMBER_OF_ASM;

typedef struct op {
  char ope[6];
  int opeNumber;
}op;

typedef struct ASM_RTYPE {
  char cpu_operation[5];
  char reg_dest[5];
  char reg_op_a[5];
  char reg_op_b[5];
} ASM_RTYPE;

typedef struct ASM_ITYPE {
  char cpu_operation[5];
  char reg_dest[5];
  char reg_op_a[5];
  int immediate;
} ASM_ITYPE;

typedef struct ASM_JTYPE {
  char cpu_operation[5];
  int im_address;
} ASM_JTYPE;

typedef struct ASM_INSTR {
  ASM_INSTR_TYPE type;
  union{
    ASM_RTYPE rtype;
    ASM_ITYPE itype;
    ASM_JTYPE jtype;
  };
  int asmNumber;
  struct ASM_INSTR* next;
}ASM_INSTR;

ASM_INSTR* asmList;
ASM_INSTR* tempAsm;

ASM_INSTR* createRTYPE(char cop[], char rd[], char ra[], char rb[]);
ASM_INSTR* createITYPE(char cop[], char rd[], char ra[], int imm);
ASM_INSTR* createJTYPE(char cop[], int addr);

void addASM(ASM_INSTR* t);

void initializeASMList();

#endif
