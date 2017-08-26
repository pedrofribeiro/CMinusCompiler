#ifndef _ASM_H_
#define _ASM_H_

#include "globals.h"
#include "util.h"

typedef enum INSTR_TYPE {RTYPE, ITYPE, JTYPE, LTYPE} ASM_INSTR_TYPE;
typedef enum {ADD,SUB,MUL,DIV,RET,
              CALL,ATR,IF_F,GOTO,V_IN,
              EQL,DIFE,GRT,LST,GTE,
              LTE,MOVE,LW,ADDIU,SW,
              LI,JUMP,BEQ,BNE,BGT,
              BLT,JR,G_VAR,G_VET,VAR,
              VET,PARAM,AND,XOR,FNDECL,NONE
             } Operation;
typedef enum {$acc, $zero, $sp, $fp, $ra, $t1, $t2, $paramp, $gp, $none} Register;

int NUMBER_OF_ASM;
int NUMBER_OF_POSITIONS;

typedef struct POSITION {
  int identifier;
  int basePosition;
  int availablePositions;
  struct POSITION* next;
} POSITION;

POSITION* tempPos;
POSITION* positionList;

typedef struct ASM_RTYPE {
  Operation cpu_operation;
  Register rd;
  Register r1;
  Register r2;
} ASM_RTYPE;

typedef struct ASM_ITYPE {
  Operation cpu_operation;
  Register rd;
  Register r1;
  int immediate;
} ASM_ITYPE;

typedef struct ASM_JTYPE {
  Operation cpu_operation;
  int address;
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

int setFP(int n);
int getFP();
int setGP(int n);
int getGP();
int setSP(int n);
int getSP();

int setVarPosition(int id, int np, int op);
int getVarPosition(int id);
void printVars();

ASM_INSTR* createRTYPE(Operation cop, Register rd, Register r1, Register r2);
ASM_INSTR* createITYPE(Operation cop, Register rd, Register r1, int imm);
ASM_INSTR* createJTYPE(Operation cop, int addr);
ASM_INSTR* createLTYPE(char cop[], int addr);

void addASM(ASM_INSTR* t);

int adjustASM(int an, int field, int nv);

void initializeASMList();

char* toChar(Operation op, Register reg);

void printASM();

#endif
