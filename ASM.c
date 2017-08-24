#include "ASM.h"

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

      //NEED TO MAKE A DECENT TRANSLATION FUNCTION TO USE WITH ENUMS

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
  asmList = createRTYPE(NONE,$none,$none,$none);
  tempAsm = createRTYPE(NONE,$none,$none,$none);
}
