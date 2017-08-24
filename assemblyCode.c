#include "assemblyCode.h"
#include "triple.h"

Operation getOperation(triple *tr){

  if (tr == NULL) {
      callException("getOperation",3,5);
      return NONE;
  }

  char inputOperation[6];
  sprintf(inputOperation,"%s",tr->operation);

  if (strcmp(inputOperation,tr->operation) != 0) {
    callException("getOperation",12,5);
    return NONE;
  }

  /* finding out the operation*/
  if (strcmp(inputOperation,"+") == 0) {
    return ADD;
  } else if (strcmp(inputOperation,"-") == 0) {
    return SUB;
  } else if (strcmp(inputOperation,"*") == 0) {
    return MUL;
  } else if (strcmp(inputOperation,"/") == 0) {
    return DIV;
  } else if (strcmp(inputOperation,"RETURN") == 0) {
    return RET;
  } else if (strcmp(inputOperation,"FNCALL") == 0) {
    return CALL;
  } else if (strcmp(inputOperation,"ATRIB") == 0) {
    return ATR;
  } else if (strcmp(inputOperation,"IF_F") == 0) {
    return IF_F;
  } else if (strcmp(inputOperation,"GOTO") == 0) {
    return GOTO;
  } else if (strcmp(inputOperation,"V_INDEX") == 0) {
    return V_IN;
  } else if (strcmp(inputOperation,"==") == 0) {
    return EQL;
  } else if (strcmp(inputOperation,"!=") == 0) {
    return DIFE;
  } else if (strcmp(inputOperation,">") == 0) {
    return GRT;
  } else if (strcmp(inputOperation,"<") == 0) {
    return LST;
  } else if (strcmp(inputOperation,">=") == 0) {
    return GTE;
  } else if (strcmp(inputOperation,"<=") == 0) {
    return LTE;
  } else if (strcmp(inputOperation,"G_VAR") == 0) {
    return G_VAR;
  } else if (strcmp(inputOperation,"G_VET") == 0) {
    return G_VET;
  } else {
    return FNDECL;
  }

  return NONE;
}

void asmCode (triple* instruction) {

  if (instruction == NULL) {
    callException("asmCode",4,5);
    return;
  }

  Operation operation = NONE;
  operation = getOperation(instruction);

  if (operation == NONE) {
    callException("asmCode",13,5);
    return;
  }

  int x,y,z;

  switch (operation) {
    case FNDECL:
        NUMBER_OF_POSITIONS = instruction->firstOperand;
        /*
        move $fp $sp
        sw $ra 0($sp)
        addiu $sp $sp 1
        >> ADENDOS NECESSARIOS:
        addiu $paramp $zero y
        */
        y = NUMBER_OF_POSITIONS - instruction->secondOperand;

        addASM ( createRTYPE ( MOVE, $fp, $sp, $zero ) );
        addASM ( createITYPE ( SW, $ra, $sp, 0 ) );
        addASM ( createITYPE ( ADDIU, $sp, $sp, 1 ) );
        addASM ( createITYPE ( ADDIU, $paramp, $zero, y ) );

        asmCode(instruction->next);

    break;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
    break;
    case RET:
        /*
        lw $ra -1($sp)
        addiu $sp $sp Z, Z = #args + #vars + #PositionsUsedByVectors + old $fp + ret. addrs
        lw $fp 0($sp)
        jr $ra
       */
       z = NUMBER_OF_POSITIONS;
       addASM ( createITYPE ( LW, $ra, $sp, -1 ) );
       addASM ( createITYPE ( ADDIU, $sp, $sp, z ) );
       addASM ( createITYPE ( LW, $fp, $sp, 0 ) );
       addASM ( createJTYPE ( JR, $ra) );

       asmCode(instruction->next);

    break;
    case CALL:
    break;
    case ATR:
    break;
    case IF_F:
    break;
    case GOTO:
    break;
    case V_IN:
    break;
    case EQL:
    break;
    case DIFE:
    break;
    case GRT:
    break;
    case LST:
    break;
    case GTE:
    break;
    case LTE:
    break;
    case G_VAR:
        /*
        addiu $globalsp $zero x
        */
        x = NUMBER_OF_GLOBALS;
        addASM ( createITYPE ( ADDIU, $globalsp, $zero, x ) );
    break;
    case G_VET:
    break;
    default:
        callException("asmCode",1,5);
    break;
  }

return;
}


void generateAssembly(triple* List){

  if(List == NULL){
    callException("generateAssembly",4,5);
    return;
  }

  initializeASMList();
  asmCode(List);
  //adjustASM();
  //printASM(asmList);

}
