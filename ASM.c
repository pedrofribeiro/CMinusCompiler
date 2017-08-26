#include "ASM.h"

int requestMemory(int n){
  if (n < 0) { callException("getMemory",24,5); return -999; }
  int returnBase = MEMORY;
  MEMORY = MEMORY + n;
  return returnBase;
}

int setFP(int n){
  if (n <= 0) { callException("setFP",23,5); return -999; }
  FRAME_POINTER = n;
  return 1;
}

int getFP(){ return FRAME_POINTER; }

int setGP(int n){
  if (n < 0) { callException("setGP",23,5); return -999; }
  GLOBAL_POINTER = n;
  return 1;
}

int getGP(){ return GLOBAL_POINTER; }

int setSP(int n){
  if (n <= 0) { callException("setSP",23,5); return -999; }
  STACK_POINTER = n;
  return 1;
}

int getSP(){ return STACK_POINTER; }


POSITION* createPosition(int id, int ap){
  POSITION* newPosition = (POSITION*) malloc(sizeof(POSITION)*1);
  if (newPosition == NULL) { callException("createPosition",3,5); return NULL; }
  newPosition->identifier = id;
  _VERBOSE_5 printf("Memory position %d allocated for id (%d)\n",MEMORY,id);
  newPosition->basePosition = requestMemory(ap);
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


int setVarPosition(int id, int ap, int op){
/*if op == 0, ap is how many positions in memory id needs
if op == 1, ap is the new base position of id.
*/

  if (id < 0) { callException("setVarPosition",8,5); return -999; }

  /*creates a new var*/
  if (op == 0) {

      /*tests if the var is already on the list*/
      int existenceTest = 0;
      existenceTest = getVarPosition(id);
      if (existenceTest != -999) {
        callException("setVarPosition",19,5);
        return -999;
      }

      /*if it's not, creates a new position for it*/
      POSITION * newPosition = createPosition(id,ap);
      if (newPosition == NULL) { callException("setVarPosition",20,5); return -999; }

      /*inserts the new position on the positions list*/
      if (positionList->next == NULL) {

          positionList->next = (POSITION*) malloc(sizeof(POSITION)*1);
          if (positionList->next == NULL) { callException("setVarPosition: positionList",3,5); return -999; }
          positionList->next = newPosition;
          return 1;

      } else {

          int SAFE_LOOP = 0;
          tempPos = positionList->next;

          while (tempPos->next != NULL) {
            tempPos = tempPos->next;
            /*safe loop measure*/
            SAFE_LOOP++;
            if(SAFE_LOOP > SAFE_LOOP_SIZE) { callException("setVarPosition",10,5); return -999; }
          }
          tempPos->next = newPosition;
          return 1;

      }

  } else if (op == 1) { /*redefines the base position of an existing var*/

    /*tests if the var is already on the list*/
    int nexistenceTest = 0;
    nexistenceTest = getVarPosition(id);
    if (nexistenceTest == -999) {
      callException("setPosition",22,5);
      return -999;
    }

      int SAFE_LOOP = 0;
      tempPos = positionList->next;

      while (tempPos != NULL) {
        if (tempPos->identifier == id) { tempPos->basePosition = ap; return 1; }
        tempPos = tempPos->next;
        /*safe loop measure*/
        SAFE_LOOP++;
        if(SAFE_LOOP > SAFE_LOOP_SIZE) { callException("setVarPosition",10,5); return -999; }
      }
      return -999;

  } else {
      callException("setVarPosition",22,5);
  }

  return -999;
}

void printVars(){
  if(positionList == NULL) { callException("printVars",4,5); return; }
  if(positionList->next == NULL) { callException("printVars",4,5); return; }

  int SAFE_LOOP = 0;
  tempPos = positionList->next;

  printf("\n\nVariables currently allocated on memory.\n");
  while (tempPos != NULL) {
    printf("id (%d), bp(%d), ap (%d)\n",tempPos->identifier,tempPos->basePosition,tempPos->availablePositions);
    tempPos = tempPos->next;
    /*safe loop measure*/
    SAFE_LOOP++;
    if (SAFE_LOOP > SAFE_LOOP_SIZE) { callException("printVars",10,5); return; }
  }

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
  _VERBOSE_5 printf("[R] %d: %d %d, %d, %d \n",newInstruction->asmNumber,newInstruction->rtype.cpu_operation,newInstruction->rtype.rd,newInstruction->rtype.r1,newInstruction->rtype.r2);
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
  _VERBOSE_5 printf("[I] %d: %d %d, %d, %d \n",newInstruction->asmNumber,newInstruction->rtype.cpu_operation,newInstruction->itype.rd,newInstruction->itype.r1,newInstruction->itype.immediate);
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
  _VERBOSE_5 printf("[J] %d: %d %d \n",newInstruction->asmNumber,newInstruction->jtype.cpu_operation,newInstruction->jtype.address);
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
  _VERBOSE_5 printf("[L] %d: %s %d ",newInstruction->asmNumber,newInstruction->ltype.functionName,newInstruction->ltype.asmAddress);
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

char* toChar(Operation op, Register reg){

  int control;
  char* resultString = (char*) malloc(sizeof(char)*6);

  if ((op != NONE) && (reg == $none)) control = 1;
  else if ((op == NONE) && (reg != $none)) control = 2;
  else if ((op != NONE) && (reg != $none)){ callException("toChar",21,5); return NULL; }
  else if ((op == NONE) && (reg == $none)){ callException("toChar",21,5); return NULL; }

  switch (control) {
    case 1:

        switch (op) {
          case ADD:
            strcpy(resultString,"ADD");
          break;
          case SUB:
            strcpy(resultString,"SUB");
          break;
          case MUL:
            strcpy(resultString,"MUL");
          break;
          case DIV:
            strcpy(resultString,"DIV");
          break;
          case ADDIU:
            strcpy(resultString,"ADDIU");
          break;
          case LW:
            strcpy(resultString,"LW");
          break;
          case LI:
            strcpy(resultString,"LI");
          break;
          case SW:
            strcpy(resultString,"SW");
          break;
          case JUMP:
            strcpy(resultString,"J");
          break;
          case BEQ:
            strcpy(resultString,"BEQ");
          break;
          case BNE:
            strcpy(resultString,"BNE");
          break;
          case BGT:
            strcpy(resultString,"BGT");
          break;
          case BLT:
            strcpy(resultString,"BLT");
          break;
          case AND:
            strcpy(resultString,"AND");
          break;
          case XOR:
            strcpy(resultString,"XOR");
          break;
          case MOVE:
            strcpy(resultString,"MOV");
          break;
          case JR:
            strcpy(resultString,"JR");
          break;
          case NONE:
            strcpy(resultString,"");
          break;
          default:
            callException("toChar: op",1,5);
            printf("MISMATCHED OP: %d\n",op);
            return NULL;
          break;
        }

    break;
    case 2:

        switch (reg) {
          case $sp:
            strcpy(resultString,"$sp");
          break;
          case $fp:
            strcpy(resultString,"$fp");
          break;
          case $ra:
            strcpy(resultString,"$ra");
          break;
          case $gp:
            strcpy(resultString,"$gp");
          break;
          case $zero:
            strcpy(resultString,"$zr");
          break;
          case $acc:
            strcpy(resultString,"$acc");
          break;
          case $paramp:
            strcpy(resultString,"$ap");
          break;
          case $t1:
            strcpy(resultString,"$t1");
          break;
          case $t2:
            strcpy(resultString,"$t2");
          break;
          case $rv:
            strcpy(resultString,"$rv");
          break;
          case $none:
            strcpy(resultString,"");
          break;
          default:
            callException("toChar: reg",1,5);
            return NULL;
          break;
        }

    break;
  }

  return resultString;

}

void printASM(int printMode){

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

      switch (printMode) {
        case 0:
            switch (tempAsm->type) {
              case RTYPE:
                  printf("%d: %s %s, %s, %s \n",tempAsm->asmNumber,toChar(tempAsm->rtype.cpu_operation,$none),toChar(NONE,tempAsm->rtype.rd),toChar(NONE,tempAsm->rtype.r1),toChar(NONE,tempAsm->rtype.r2));
              break;
              case ITYPE:
                  printf("%d: %s %s, %s, %d \n",tempAsm->asmNumber,toChar(tempAsm->itype.cpu_operation,$none),toChar(NONE,tempAsm->itype.rd),toChar(NONE,tempAsm->itype.r1),tempAsm->itype.immediate);
              break;
              case JTYPE:
                  printf("%d: %s %d \n",tempAsm->asmNumber,toChar(tempAsm->jtype.cpu_operation,$none),tempAsm->jtype.address);
              break;
              case LTYPE:
                  printf("%d: %s %d ",tempAsm->asmNumber,tempAsm->ltype.functionName,tempAsm->ltype.asmAddress);
              break;
              default:
                  callException("printASM",15,5);
              break;
            }
        break;
        case 1:
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
        break;
        default:
            callException("printASM: printMode",1,5);
            return;
        break;
      }
      tempAsm = tempAsm->next;
	}
}


void initializeASMList(){
  NUMBER_OF_ASM = -2;
  NUMBER_OF_POSITIONS = 0;
  ALIGNED_MEMORY = FALSE;
  asmList = createRTYPE(NONE,$none,$none,$none);
  tempAsm = createRTYPE(NONE,$none,$none,$none);
  positionList = createPosition(-2,0);
  tempPos = createPosition(-1,0);
}
