#ifndef _RUN_TIME_ENVIRONMENT_
#define _RUN_TIME_ENVIRONMENT_

#include "globals.h"
#include "util.h"

typedef enum {$zero, $one, $acc, $sp, $fp,
              $ra, $rv, $paramp, $gp, $t1,
              $t2, $t3, $t4, $t5, $t6, $t7,
              $t8, $t9, $t10, $t11, $t12,
              $t13, $t14, $t15, $t16, $t17,
              $t18, $t19, $t20, $t21, $t22, $none} Register;

int OCCUPIED_REGS;
int REGS_TO_FREE;

typedef struct VARIABLE {
  int identifier;
  int basePosition;
  int availablePositions;
  struct VARIABLE* next;
} VARIABLE;

VARIABLE* tempVar;
VARIABLE* variablesList;

typedef struct REGISTER {
  int regNumber;
  int isFree;
  int storesTriple;
  int storesVar;
  int storesConst;
  int contents;
}REGISTER;

REGISTER** REGISTER_BANK;
REGISTER** tempRegister;

typedef struct MEMORY_UNIT {
  int positionNumber;
  int isFree;
  int storesVar;
  int contents;
}MEMORY_UNIT;

MEMORY_UNIT** MEMORY;
MEMORY_UNIT* tempMemory;

void initializeMemory();
void initializeRegisterBank();
void initializeVariables();
void cleanRuntimeEnvironment();
int allocateMemory(int n);
int freeMemory(int initialPosition, int finalPosition);
int allocateRegister(int type, int id);
int deallocateRegister(int n);
void listToDeallocate(int rn);
int deallocateRegisters();
void setRegister(int rn, int value);
void memoryHandler(int position, int value);
int requestMemory(int identifier, int numberOfPositions);
void printMemory();
void printRegisterBank();
void RuntimeEnvironmentStart();
void RuntimeEnvironmentStop();


void setFP();
int getFP();
void setGP();
int getGP();
void setSP();
int getSP();

VARIABLE* createPosition(int id, int ap);
int setNamePosition(int id, int np);
int getNamePosition(int id);
void printVars();

void initializeAlignments();
void setAlignment(int asmNumber, int destination);

#endif
