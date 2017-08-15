#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{
	switch (token)
  	{
		case INT:
		case VOID:
		case IF:
		case ELSE:
		case WHILE:
		case RETURN: fprintf(listing,"reserved word: %s\n",tokenString); break;
		case ASSIGN: 	printf("= \n"); break;
		case LT: 	printf("< \n"); break;
		case LET: 	printf("<= \n"); break;
		case GT: 	printf("> \n"); break;
		case GET: 	printf(">= \n"); break;
		case EQ: 	printf("== \n"); break;
		case DIF: 	printf("!= \n"); break;
		case PLUS: 	printf("+ \n"); break;
		case MINUS: 	printf("- \n"); break;
		case TIMES: 	printf("* \n"); break;
		case LPAREN: 	printf("( \n"); break;
		case RPAREN: 	printf(") \n"); break;
		case LCH: 	printf("[ \n"); break;
		case RCH: 	printf("] \n"); break;
		case SEMI: 	printf("; \n"); break;
		case OVER: 	printf("/ \n"); break;
		case LBKT: 	printf("{ \n"); break;
		case RBKT: 	printf("} \n"); break;
		case COMMA: 	printf(", \n"); break;
		case ENDFILE: 	printf("EOF \n"); break;
		case NUM:	printf("NUM, val = %s \n",tokenString); break;
		case ID: 	printf("ID, name = %s \n", tokenString); break;
		case ERROR: 	printf("ERRO: %s \n", tokenString); break;
	    	default: 	printf("Token Desconhecido: %d\n",token);
  	}
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Erro de falta de memoria na linha %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
		strcpy(t->tacode,"");
		strcpy(t->tacodeopk,"");
		strcpy(t->cummulativeTacode,"");
		t->paramIndex = 0;
		t->numberOfParameters = 0;
  }
  return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Erro de falta de memoria na linha %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++)
		t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
  	t->type = Integer;
		strcpy(t->tacode,"exp");
		strcpy(t->tacodeopk,"");
		strcpy(t->cummulativeTacode,"");
		t->paramIndex = 0;
		t->numberOfParameters = 0;
  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Erro de falta de memoria na linha %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
	case RepeatK:
	  fprintf(listing, "While\n");
		break;
	case IfK:
	  fprintf(listing, "If\n");
		break;
	case AtribK:
		  fprintf(listing, "Assign to:\n");
		break;
	case ReturnK:
		  fprintf(listing, "Return\n");
		break;
	case FunK:
	  fprintf(listing, "Function Declaration: %s\n",tree->attr.name);
		break;
	case VarK:
		fprintf(listing, "Variable Declaration: %s\n",tree->attr.name);
		break;
	case VetK:
		  fprintf(listing, "Vector Declaration: %s\n",tree->attr.name);
		break;
	case DadoK:
		  fprintf(listing, "Data Type: %s\n",tree->attr.name);
		break;
        default:
          fprintf(listing,"No de Tipo de Declaracao com StmtKind desconhecido\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
	    case OpK:
				  fprintf(listing, "Operation: %s\n",tree->attr.name);
		     break;
	    case IdK:
				  fprintf(listing, "Identifier (Variable): %s\n",tree->attr.name);
		    break;
	    case IdVetK:
				  fprintf(listing, "Identifier (Vector): %s\n",tree->attr.name);
		    break;
	    case IdFunK:
				  fprintf(listing, "Function call: %s\n",tree->attr.name);
		    break;
	    case ConstK:
			  fprintf(listing, "Const: %d\n",tree->attr.val);
		    break;
      default:
  	    fprintf(listing,"No de Tipo de Expressao com ExpKind desconhecido\n");
  	    break;
    }
  }
  else fprintf(listing,"No com tipo desconhecido.\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
}
  UNINDENT;
}
