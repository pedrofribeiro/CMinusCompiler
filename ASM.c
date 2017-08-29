#include "ASM.h"


ASM_INSTR* createRTYPE(Operation cop, Register rd, Register r1, Register r2){
  ASM_INSTR* newInstruction = malloc(sizeof(ASM_INSTR)*1);
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
  ASM_INSTR* newInstruction = malloc(sizeof(ASM_INSTR)*1);
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
  ASM_INSTR* newInstruction = malloc(sizeof(ASM_INSTR)*1);
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
  ASM_INSTR* newInstruction = malloc(sizeof(ASM_INSTR)*1);
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
      asmList->next = malloc(sizeof(ASM_INSTR)*1);
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
  char* resultString = malloc(sizeof(char)*6);

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
  asmList = createRTYPE(NONE,$none,$none,$none);
  tempAsm = createRTYPE(NONE,$none,$none,$none);
}

void cleanASM(){
  free(asmList);
  asmList = NULL;
  free(tempAsm);
  tempAsm = NULL;
}
