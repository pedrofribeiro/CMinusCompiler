#include "assemblyCode.h"

void asmCode (triple* instruction) {

  if (instruction == NULL) { callException("asmCode",4,5); return; }

  Operation operation = NONE;
  operation = getOperation(instruction);
  if (operation == NONE) { callException("asmCode",13,5); return; }

  int i;
  int t1, t2, t3;

  switch (operation) {
    case FNDECL:

    break;
    case EQL:
    case DIFE:
    case GRT:
    case LST:
    case GTE:
    case LTE:
    case ADD:
    case SUB:
    case MUL:
    case DIV:
        if ((instruction->firstOperandType == ConstantNoAddress) && (instruction->secondOperandType == ConstantNoAddress)) {

            int packegedConstants;
            switch (operation) {
              case ADD:
                  packegedConstants = instruction->firstOperand + instruction->secondOperand;
              break;
              case SUB:
                packegedConstants = instruction->firstOperand - instruction->secondOperand;
              break;
              case MUL:
                packegedConstants = instruction->firstOperand * instruction->secondOperand;
              break;
              case DIV:
                packegedConstants = instruction->firstOperand / instruction->secondOperand;
              break;
              case EQL:
                packegedConstants = instruction->firstOperand == instruction->secondOperand;
              break;
              case DIFE:
                packegedConstants = instruction->firstOperand != instruction->secondOperand;
              break;
              case GRT:
                packegedConstants = instruction->firstOperand > instruction->secondOperand;
              break;
              case LST:
                packegedConstants = instruction->firstOperand < instruction->secondOperand;
              break;
              case GTE:
                packegedConstants = instruction->firstOperand >= instruction->secondOperand;
              break;
              case LTE:
                packegedConstants = instruction->firstOperand <= instruction->secondOperand;
              break;
              default:
                callException("asmCode: const packaging",1,5);
              break;
            }

            addASM( createITYPE( LI, $acc, $zero, packegedConstants) );
            setRegister(2,packegedConstants);

        } else if ((instruction->firstOperandType == ConstantNoAddress) && (instruction->secondOperandType == SymboltableAddress)) {

            addASM( createITYPE( LI, $acc, $zero, instruction->firstOperand ) );
            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            addASM( createRTYPE( operation, $acc, $acc, $t1 ) );
            setRegister(2,instruction->firstOperand);
            setRegister(9,getVarPosition(instruction->secondOperand));

        } else if ((instruction->firstOperandType == ConstantNoAddress) && (instruction->secondOperandType == TripleAddress)) {

            addASM( createITYPE( LI, $t1, $zero, instruction->firstOperand ) );
            addASM( createRTYPE( operation, $acc, $t1, $acc ) );
            setRegister(9,instruction->firstOperand);

        } else if ((instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == ConstantNoAddress)) {

            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->firstOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            addASM( createITYPE( LI, $acc, $zero, instruction->secondOperand ) );
            addASM( createRTYPE( operation, $acc, $t1, $acc ) );
            setRegister(9,getVarPosition(instruction->firstOperand));
            setRegister(2,instruction->secondOperand);

        } else if ((instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == SymboltableAddress)) {

            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->firstOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            addASM( createITYPE( LI, $acc, $zero, getVarPosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $acc, $acc, 0 ) );
            addASM( createRTYPE( operation, $acc, $t1, $acc ) );
            setRegister(9,getVarPosition(instruction->firstOperand));
            setRegister(2,getVarPosition(instruction->secondOperand));

        } else if ((instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == TripleAddress)) {

            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->firstOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            addASM( createRTYPE( operation, $acc, $t1, $acc ) );
            setRegister(9,getVarPosition(instruction->firstOperand));
            setRegister(2,instruction->secondOperand);

        } else if ((instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == ConstantNoAddress)) {

            addASM( createITYPE( LI, $t1, $zero, instruction->secondOperand ) );
            addASM( createRTYPE( operation, $acc, $acc, $t1 ) );
            setRegister($t1,instruction->secondOperand);

        } else if ((instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == SymboltableAddress)) {

            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            addASM( createRTYPE( operation, $acc, $acc, $t1 ) );
            setRegister($t1,getVarPosition(instruction->secondOperand));

        } else if ((instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == TripleAddress)) {
            callException("asmCode: tr op tr",25,5);
        } else {
            callException("asmCode: arithmetic ops",1,5);
        }
    break;
    case RET:
        /*
        lw $ra -1($sp)
        addiu $sp $sp Z, Z = #args + #vars + #PositionsUsedByVectors + old $fp + ret. addrs
        lw $fp 0($sp)
        jr $ra
       */
       addASM ( createRTYPE ( SUB, $t1, $sp, $one ) );
       addASM ( createITYPE ( LW, $ra, $t1, 0 ) );
       addASM ( createITYPE ( ADDIU, $sp, $sp, 99999 ) );
       addASM ( createITYPE ( LW, $fp, $sp, 0 ) );
       addASM ( createJTYPE ( JR, $ra) );

    break;
    case PARAM:
        if (instruction->firstOperandType == ConstantNoAddress) {
            /* sw $acc 0($sp)
               addiu $sp $sp 1 */
            addASM ( createITYPE ( LI, $t1, $zero, instruction->firstOperandType ) );
            addASM ( createITYPE ( SW, $sp, $t1, 0 ) );
            addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
        } else if (instruction->firstOperandType == SymboltableAddress) {
            /* lw $acc var_position_on_memory
               sw $acc 0($sp)
               addiu $sp $sp 1
            */
            addASM ( createITYPE ( LI, $t1, $zero, getVarPosition(instruction->firstOperand) ) );
            addASM ( createITYPE ( LW, $t1, $t1, 0 ) );
            addASM ( createITYPE ( SW, $sp, $t1, 0 ) );
            addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );

        } else if (instruction->firstOperandType == TripleAddress) {
          /* sw $acc 0($sp)
             addiu $sp $sp 1 */
            addASM ( createITYPE ( SW, $sp, $acc, 0 ) );
            addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
        } else {
            callException("asmCode: PARAM",1,5);
        }
    break;
    case CALL:
        addASM ( createJTYPE ( JAL, 5555) );
    break;
    case ATR:

        if ( (instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == ConstantNoAddress) ) {

            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->firstOperand) ) );
            addASM( createITYPE( LI, $acc, $zero, instruction->secondOperand ) );
            addASM( createITYPE( SW, $t1, $acc, 0) );

        } else if ( (instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == SymboltableAddress) ) {

            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->firstOperand) ) );
            addASM( createITYPE( LI, $acc, $zero, getVarPosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $acc, $acc, 0) );
            addASM( createITYPE( SW, $t1, $acc, 0) );

        } else if ( (instruction->firstOperandType == SymboltableAddress) && (instruction->secondOperandType == TripleAddress) ) {

            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->firstOperand) ) );
            addASM( createITYPE( SW, $t1, $acc, 0) );

        } else if ( (instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == ConstantNoAddress) ) {

            addASM( createITYPE( LI, $t1, $zero, instruction->secondOperand ) );
            addASM( createITYPE( SW, $acc, $t1, 0) );

        } else if ( (instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == SymboltableAddress) ) {

            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0) );
            addASM( createITYPE( SW, $acc, $t1, 0) );

        } else if ( (instruction->firstOperandType == TripleAddress) && (instruction->secondOperandType == TripleAddress) ) {
            /*this only happens if a vector is receiving its value from an operation
              vectors have a SPECIAL dedicated $reg for its parameter return, its $rv
            */
            addASM( createITYPE( SW, $rv, $acc, $zero ) );

        } else {
            callException("asmCode: ATR",26,5);
            return;
        }

    break;
    case IF_F:
        addASM( createRTYPE( BEQ, $acc, $zero, 222 ) );
    break;
    case GOTO:
        addASM( createJTYPE( JUMP, 111 ) );
    break;
    case V_IN:
        if (instruction->secondOperandType == ConstantNoAddress) {

            addASM( createITYPE( LI, $t1, $zero, instruction->secondOperand ) );
            addASM( createITYPE( LI, $acc, $zero, getVarPosition(instruction->firstOperand) ) );
            addASM( createRTYPE( ADD, $acc, $acc, $t1 ) );
            addASM( createRTYPE( MOVE, $rv, $acc, $zero ) );

        } else if (instruction->secondOperandType == SymboltableAddress) {

            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->secondOperand) ) );
            addASM( createITYPE( LW, $t1, $t1, 0 ) );
            addASM( createITYPE( LI, $acc, $zero, getVarPosition(instruction->firstOperand) ) );
            addASM( createRTYPE( ADD, $acc, $acc, $t1 ) );
            addASM( createRTYPE( MOVE, $rv, $acc, $zero ) );

        } else if (instruction->secondOperandType == TripleAddress) {

            addASM( createITYPE( LI, $t1, $zero, getVarPosition(instruction->firstOperand) ) );
            addASM( createRTYPE( ADD, $acc, $t1, $acc ) );
            addASM( createRTYPE( MOVE, $rv, $acc, $zero ) );

        } else {
            callException("asmCode: V_IN",1,5);
        }
    break;
    case G_VAR:
        /*
        sw $zero 0($sp)
        addiu $sp $sp 1
        */
        addASM ( createITYPE ( SW, $sp, $zero, 0 ) );
        addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
        int gvarCreation;
        gvarCreation = setVarPosition(instruction->firstOperand,1);
        if (gvarCreation == -999) { callException("asmCode: G_VAR",20,5); printf(" st id(%d)\n",instruction->firstOperand); return; }

    break;
    case G_VET:
      /*
      loop n, n = number of parameters
        sw $zero 0($sp)
        addiu $sp $sp 1*/
        for (i = 0; i < instruction->secondOperand; i++) {
            addASM ( createITYPE ( SW, $sp, $zero, 0 ) );
            addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
        }
      int gvetCreation;
      gvetCreation = setVarPosition(instruction->firstOperand,instruction->secondOperand);
      if (gvetCreation == -999) { callException("asmCode: G_VET",20,5); printf(" st id(%d)\n",instruction->firstOperand); return; }

    break;
    case VET:
        /*
        loop n, n = number of parameters
          sw $zero 0($sp)
          addiu $sp $sp 1
        */

        for (i = 0; i < instruction->secondOperand; i++) {
            addASM ( createITYPE ( SW, $sp, $zero, 0 ) );
            addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
        }
        int vetCreation;
        vetCreation = setVarPosition(instruction->firstOperand,instruction->secondOperand);
        if (vetCreation == -999) { callException("asmCode: VET",20,5); printf(" st id(%d)\n",instruction->firstOperand); return; }

    break;
    case VAR:
        /*
        sw $zero 0($sp)
        addiu $sp $sp 1
        */
        addASM ( createITYPE ( SW, $sp, $zero, 0 ) );
        addASM ( createRTYPE ( ADD, $sp, $sp, $one ) );
        int varCreation;
        varCreation = setVarPosition(instruction->firstOperand,1);
        if (varCreation == -999) { callException("asmCode: VAR",20,5); printf("TN:%d, st id(%d)\n",instruction->tripleNumber,instruction->firstOperand); return; }

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

  TRACE_ASM_GEN = FALSE;

  initializeRegisterBank();
  initializeMemory();
  initializeVariables();
  initializeASMList();
  asmCode(List);
  //adjustASM();
  printASM(0);
  printVars();
  //printRegisterBank();
  printMemory();

  /*cleaning up*/
  cleanTriples();
  cleanRuntimeEnvironment();
  cleanASM();

}
