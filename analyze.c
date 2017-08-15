#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"
#include "scan.h"

/* counter for variable memory locations */
static int location = 0;

/*
Function to control the variable's and function's ESCOPO
*/
char* ESCOPO = "global";
char* GUARDAESCOPO = "global";

void ESCOPOControllerUP(TreeNode * node){
        GUARDAESCOPO = ESCOPO;
        ESCOPO = node->attr.name;
        if (node->child[0] != NULL && node->child[0]->kind.exp == FunK){
          ESCOPO = node->child[0]->attr.name;
        }
}

void ESCOPOControllerDOWN(){
        ESCOPO = GUARDAESCOPO;
}


/* Procedure traverse is a generic recursive
* syntax tree traversal routine:
* it applies preProc in preorder and postProc
* in postorder to tree pointed to by t
*/
static void traverse( TreeNode * t,
  void (* preProc) (TreeNode *),
  void (* postProc) (TreeNode *) )
  { if (t != NULL)
    { preProc(t);
      { /*pre-order traversal*/
        int i;
        for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
      }
      postProc(t);
      traverse(t->sibling,preProc,postProc);
    }
  }

  /* nullProc is a do-nothing procedure to
  * generate preorder-only or postorder-only
  * traversals from traverse
  */
  static void nullProc(TreeNode * t)
  { if (t==NULL) return;
    else return;
  }

  static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"LINHA :  %d: %s\n",t->lineno,message);
  Error = TRUE;
}

/* Procedure insertNode inserts
* identifiers stored in t into
* the symbol table
*/
static void insertNode( TreeNode * t)
{
  switch (t->nodekind)
  {
    case StmtK:
      switch (t->kind.stmt)
      {
        case VarK:
          if (t->type == Void){
            typeError(t, "ERRO ENCONTRADO. VARIAVEL COM TIPO VOID.\n");
            t->child[0] = NULL;
            t->child[1] = NULL;
            t->child[2] = NULL;
          } else {
                if(st_lookup(t->attr.name) == -1){
                  if(st_lookup3(t->attr.name) == -1){
                    st_insert(t->attr.name, location++, t->lineno, t->type, t->kind.stmt, ESCOPO);
                  } else{
                    typeError(t, "ERRO ENCONTRADO. RE-DECLARACAO DE UMA VARIAVEL EXISTENTE. OU VAR COM MESMO NOME DE FUNCAO.");
                  }
                }else{
                  typeError(t, "ERRO ENCONTRADO. RE-DECLARACAO DE UMA VARIAVEL EXISTENTE. OU VAR COM MESMO NOME DE FUNCAO.");
                }
          }
        break; // do case VARK
        case VetK:
          if(st_lookup3(t->attr.name) == -1){
            st_insert(t->attr.name,location++,t->lineno,t->type,t->kind.stmt, ESCOPO);
          } else{
            typeError(t,"ERRO ENCONTRADO. RE-DECLARACAO DE UM VETOR EXISTENTE, OU COM MESMO NOME DE FUNCAO.\n");
          }
        break;
        case FunK:
          if(st_lookup(t->attr.name) == -1) {
            ESCOPOControllerUP(t);
            st_insert(t->attr.name, location++, t->lineno, t->type, t->kind.stmt, ESCOPO);
          }else {
            typeError(t, "ERRO ENCONTRADO. IDENTIFICADOR DE FUNCAO DUPLICADO.");
          }
        break;
        case AtribK:
          if(st_lookup(t->child[0]->attr.name) == -1){ //Verifica apenas o nome, para nao gerar erro com globais
            typeError(t, "ERRO ENCONTRADO. ATRIBUICAO FEITA A VAR NAO DECLARADA.");
          }
        break;
        default:
        break;
      }
    break;
    case ExpK:
      switch (t->kind.exp)
      {
        case IdK:
          if(st_lookup2(t->attr.name) == -1) { //Verifica apenas o nome, para nao gerar erro com globais
            typeError(t, "ERRO ENCONTRADO. USO DE VARIAVEL NAO DECLARADA.");
          } else {
            st_insert(t->attr.name,location++, t->lineno, t->type, t->kind.exp, ESCOPO);
          }
        break;
        case IdVetK:
          if(st_lookup2(t->attr.name) == -1) { //Verifica apenas o nome, para nao gerar erro com globais
            typeError(t, "ERRO ENCONTRADO. USO DE VETOR NAO DECLARADO.");
            t->child[0] = NULL;
            t->child[1] = NULL;
            t->child[2] = NULL;
          } else {
            st_insert(t->attr.name, location++, t->lineno, t->type, t->kind.exp, ESCOPO);
          }
        break;
        case IdFunK:
          if(st_lookup2(t->attr.name) == -1) { //Verifica apenas o nome, para nao gerar erro com globais
            typeError(t, "ERRO ENCONTRADO. USO DE FUNCAO NAO DECLARADA.");
          }else{
            st_insert(t->attr.name, location++, t->lineno, t->type, t->kind.exp, ESCOPO);
            //counting the number of parameters
              if(t->child[0]->type == Void) t->numberOfParameters = 0;
              else{
                int parametersOfFn = 1;
                TreeNode *specialNode = (TreeNode*) malloc(sizeof(TreeNode)*1);
                if(specialNode == NULL){
                   callException("insertNode FunK",3,3);
                   break;
                }
                specialNode = t->child[0]->sibling;
                while (specialNode != NULL) {
                  parametersOfFn++;
                  specialNode = specialNode->sibling;
                }
                t->numberOfParameters = parametersOfFn;
              }
          }
        break;
        default:
        break;
        }
      break;

      default:
      break;
    } // SWITCH MAIS EXTERNO
  }

/*Function to verify the existence of main*/
  int mainVerify(){
    if(st_lookup("main") == -1){
      return -1;
    }else {
      return 0;
    }
  }

  /* Function buildSymtab constructs the symbol
  * table by preorder traversal of the syntax tree
  */
  void buildSymtab(TreeNode * syntaxTree)
  {
    traverse(syntaxTree,insertNode,nullProc);
    if (TraceAnalyze)
    { fprintf(listing,"\n SYMBOL TABLE DO COMPILADOR: \n\n");
    printSymTab(listing);
  }
}

/* Procedure checkNode performs
* type checking at a single tree node
*/
TreeNode *tree;
static void checkNode(TreeNode * t){

  switch (t->nodekind)
  { case ExpK:
        switch (t->kind.exp) {
          case OpK:
            if((t->child[0]->type != Integer) || (t->child[1]->type != Integer)){
                typeError(t,"ERRO ENCONTRADO. PELO MENOS UM OPERANDO INVALIDO.\n");
            }
          break;
          case ConstK:
            if(t->type != Integer){
              typeError(t, "ERRO ENCONTRADO. CONSTANTE DE TIPO NAO-INTEIRO.\n");
            }
          break;
          case IdK:
          break;
          case IdVetK:
          break;
          case IdFunK:
          break;
          default:
          break;
        }
      break;

    case StmtK:
        switch (t->kind.stmt) {
          case IfK:
            if(t->child[0]->type != Boolean){
               typeError(t, "ERRO ENCONTRADO. TESTE NAO BOOLEANO NO IF.\n");
             }
          break;
          case RepeatK:
            if(t->child[0]->type != Boolean){
              typeError(t->child[0], "ERRO ENCONTRADO. TESTE NAO BOOLEANO NO WHILE.\n");
            }
          break;
          case ReturnK:
          break;
          case FunK:
              if(t->child[0] != NULL){
                if(t->child[0]->type != Integer){
                  typeError(t, "ERRO ENCONTRADO. PARAMETRO (DE FUNCAO) COM TIPO NAO-INTEIRO.\n");
                }
              }
          break;
          case VarK:
            if(t->type != Integer){
              typeError(t, "ERRO ENCONTRADO. VARIAVEL DE TIPO NAO-INTEIRO.\n");
            }
          break;
          case VetK:
            if(t->type != Integer){
              typeError(t, "ERRO ENCONTRADO. VETOR DE TIPO NAO-INTEIRO.\n");
            }
            if(t->child[0]->type != Integer){
              typeError(t, "ERRO ENCONTRADO. TAMANHO DO VETOR DE TIPO NAO-INTEIRO.\n");
            }
          break;
          case DadoK:
            if((t->type != Integer) && (t->type != Void)){
              typeError(t,"ERRO ENCONTRADO. TIPO DE DADO INVALIDO.");
            }
          break;
          case AtribK:
            if((t->child[0]->type == Integer) && (t->child[1]->kind.stmt == IdFunK)){
              if((st_lookup4(t->child[1]->attr.name) == 1) ){
                typeError(t,"ERRO ENCONTRADO. ATRIBUICAO VOID FEITA A VARIAVEL INTEIRA.");
              }
            }
          break;

          default:
          break;
        }
    break;
    default:
    break;
  }
}

/* Procedure typeCheck performs type checking
* by a postorder syntax tree traversal
*/
void typeCheck(TreeNode * syntaxTree)
{ traverse(syntaxTree,nullProc,checkNode);
}
