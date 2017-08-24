#include "ASM.h"

int getMemoryPosition(int n){
  if (n < 0) { callException("getMemoryPosition",8,5); return -999; }
  int nextFreePosition;
  nextFreePosition = MEMORY_POSITION;
  MEMORY_POSITION = MEMORY_POSITION + n;
  return nextFreePosition;
}

int freeMemoryPosition(int n){
  if (n < 0) { callException("freeMemoryPosition: n",8,5); return -999; }
  MEMORY_POSITION = MEMORY_POSITION - n;
  if (MEMORY_POSITION < 0) { callException("freeMemoryPosition: MEMORY_POSITION",8,5); return -999; }
  return 1;
}

POSITION* createPosition(int id, int ap){
  POSITION* newPosition = (POSITION*) malloc(sizeof(POSITION)*1);
  if (newPosition == NULL) { callException("createPosition",3,5); return -999; }
  newPosition->identifier = id;
  newPosition->basePosition = getMemoryPosition(ap);
  newPosition->availablePositions = ap;
  newPosition->next = NULL;
  return newPosition;
}

int getVarPosition(int id){
  if ((id == -2) || (id == -1)) return -999; /*special control variables have those ids*/
  else if (id < 0) { callException("getVarPosition",8,5); return -999; }

  tempPos = positionList->next;
  int SAFE_LOOP = 0;

  while (tempPos != NULL) {
    if (tempPos->identifier == id) { return tempPos->basePosition; }
    tempPos = tempPos->next;
    /*safe loop measure*/
    SAFE_LOOP++;
    if(SAFE_LOOP > SAFE_LOOP_SIZE) { callException("getVarPosition",10,5); return -999; }
  }
  return -999;
}

int setVarPosition(int id, int ap){
  if (id < 0) { callException("setVarPosition",8,5); return -999; }

  /*tests if the var is already on the list*/
  int existenceTest = 0;
  existenceTest = getVarPosition(id);
  if (existenceTest != -999) {
    callException("setPosition",19,5);
    return -999;
  }

  /*if it's not, creates a new position for it*/
  POSITION * newPosition = createPosition(id,ap);
  if (newPosition == NULL) { callException("setVarPosition",20,5); return -999; }

  /*inserts the new position on the positions list*/
  if (positionList->next == NULL) {

      positionList->next = (POSITION*) malloc(sizeof(POSITION)*1);
      if (positionList->next == NULL) { callException("setPosition: positionList",3,5); return -999; }
      positionList->next = newPosition;
      return 1;

  } else {

      int SAFE_LOOP = 0;
      tempPos = positionList->next;

      while (tempPos->next != NULL) {
        tempPos = tempPos->next;
        /*safe loop measure*/
        SAFE_LOOP++;
        if(SAFE_LOOP > SAFE_LOOP_SIZE) { callException("setPosition",10,5); return -999; }
      }
      tempPos->next = newPosition;
  }
  return 1;
}

ASM_INSTR* createRTYPE(Operation cop, Register rd, Register r1, Register r2){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createRTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = RTYPE;
  newInstruction->rtype.cpu_operation = cop;
  newInstruction->rtype.rd = rd;
  newInstruction->rtype.r1 = r1;
  newInstruction->rtype.r2 = r2;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
      printf("[R] %d: %d %d, %d, %d \n",newInstruction->asmNumber,newInstruction->rtype.cpu_operation,newInstruction->rtype.rd,newInstruction->rtype.r1,newInstruction->rtype.r2);
  return newInstruction;
}

ASM_INSTR* createITYPE(Operation cop, Register rd, Register r1, int imm){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createITYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = ITYPE;
  newInstruction->itype.cpu_operation = cop;
  newInstruction->itype.rd = rd;
  newInstruction->itype.r1 = r1;
  newInstruction->itype.immediate = imm;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
      printf("[I] %d: %d %d, %d, %d \n",newInstruction->asmNumber,newInstruction->rtype.cpu_operation,newInstruction->itype.rd,newInstruction->itype.r1,newInstruction->itype.immediate);
  return newInstruction;
}

ASM_INSTR* createJTYPE(Operation cop, int addr){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createJTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = JTYPE;
  newInstruction->jtype.cpu_operation = cop;
  newInstruction->jtype.address = addr;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
      printf("[J] %d: %d %d \n",newInstruction->asmNumber,newInstruction->jtype.cpu_operation,newInstruction->jtype.address);
  return newInstruction;
}

ASM_INSTR* createLTYPE(char cop[], int addr){
  ASM_INSTR* newInstruction = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
  if (newInstruction == NULL) {
    callException("createLTYPE",3,5);
    return NULL;
  }
  NUMBER_OF_ASM = NUMBER_OF_ASM + 1;
  newInstruction->type = LTYPE;
  strcpy(newInstruction->ltype.functionName,cop);
  newInstruction->ltype.asmAddress = addr;
  newInstruction->asmNumber = NUMBER_OF_ASM;
  newInstruction->next = NULL;
      printf("[L] %d: %s %d ",newInstruction->asmNumber,newInstruction->ltype.functionName,newInstruction->ltype.asmAddress);
  return newInstruction;
}

void addASM(ASM_INSTR* newInstruction){

  if (newInstruction == NULL) {
    callException("addASM",14,5);
    return;
  }

  if (asmList->next == NULL) {
      asmList->next = (ASM_INSTR*) malloc(sizeof(ASM_INSTR)*1);
      if (asmList->next == NULL) {
        callException("addASM",3,5);
        return;
      } else {
        asmList->next = newInstruction;
      }
  } else {

      int SAFE_LOOP = 0;
      tempAsm = asmList->next;
      while (tempAsm->next != NULL) {
        tempAsm = tempAsm->next;
        /*safe loop measure*/
        SAFE_LOOP++;
        if(SAFE_LOOP > SAFE_LOOP_SIZE){
          callException("addASM",10,5);
          return;
        }
      }

      tempAsm->next = newInstruction;
  }

}

int adjustASM(int an, int field, int nv){
  if ((an <= 0) || (an > NUMBER_OF_ASM)) {
    callException("adjustASM: number of asm",8,5);
    return -1;
  }
  if ((field < 1) || (field > 3)){
    callException("adjustASM: field",8,5);
    return -1;
  }
  int SAFE_LOOP = 0;
  tempAsm = asmList->next;
  while (tempAsm->next != NULL) {
    if (tempAsm->asmNumber == an) {
      if (field == 1) { tempAsm->itype.immediate = nv; }
      else if (field == 2) { tempAsm->jtype.address = nv; }
      else if (field == 3) { tempAsm->ltype.asmAddress = nv; }
      return 1;
    } else { tempAsm = tempAsm->next; }
    /*safe loop measure*/
    SAFE_LOOP++;
    if(SAFE_LOOP > SAFE_LOOP_SIZE){
      callException("adjustTriple",10,4);
      return -1;
    }
  }
  return -1;
}

void printASM(){

  if (asmList == NULL) {
      callException("printASM",4,5);
      return;
  } else if (asmList->next == NULL) {
      callException("printASM",4,5);
      return;
  }

  int SAFE_LOOP = 0;
  tempAsm = asmList->next;

  while (tempAsm != NULL) {

      /*safe loop measure*/
      SAFE_LOOP++;
      if(SAFE_LOOP > SAFE_LOOP_SIZE){
        callException("printASM",10,5);
        return;
      }

      switch (tempAsm->type) {
        case RTYPE:
            printf("%d: %d %d, %d, %d \n",tempAsm->asmNumber,tempAsm->rtype.cpu_operation,tempAsm->rtype.rd,tempAsm->rtype.r1,tempAsm->rtype.r2);
        break;
        case ITYPE:
            printf("%d: %d %d, %d, %d \n",tempAsm->asmNumber,tempAsm->itype.cpu_operation,tempAsm->itype.rd,tempAsm->itype.r1,tempAsm->itype.immediate);
        break;
        case JTYPE:
            printf("%d: %d, %d \n",tempAsm->asmNumber,tempAsm->jtype.cpu_operation,tempAsm->jtype.address);
        break;
        case LTYPE:
            printf("%d: %s %d ",tempAsm->asmNumber,tempAsm->ltype.functionName,tempAsm->ltype.asmAddress);
        break;
        default:
            callException("printASM",15,5);
        break;
      }
      tempAsm = tempAsm->next;
	}
}


void initializeASMList(){
  NUMBER_OF_ASM = -2;
  NUMBER_OF_POSITIONS = 0;
  ALIGNED_GLOBALS_POINTER = FALSE;
  MEMORY_POSITION = -1;
  asmList = createRTYPE(NONE,$none,$none,$none);
  tempAsm = createRTYPE(NONE,$none,$none,$none);
  positionList = createPosition(-2,0);
  tempPos = createPosition(-1,0);
}
