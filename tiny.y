%{

#define YYPARSER 		/* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *

static TreeNode * savedTree; 	/* stores syntax tree for later return */
static int yylex(void);

%}

%token INT VOID
%token IF THEN ELSE END WHILE RETURN
%token ID NUM
%token EQ DIF LT LET GT GET
%token PLUS MINUS TIMES OVER ASSIGN
%token LPAREN RPAREN LCH RCH LBKT RBKT COMMA SEMI
%token ERROR

/*Eliminacao de ambiguidade do ELSE PENDENTE usando precedencia*/
%nonassoc RPAREN
%nonassoc ELSE

%% /* Gramatica para a linguagem C Menos */

programa		: declaracao_lista
						{
							savedTree = $1;
						}
						;
declaracao_lista	: declaracao_lista declaracao
									{
										YYSTYPE t = $1;
                   		  if (t != NULL)
                   		  {
													while (t->sibling != NULL)
													 		t = t->sibling;
				    							t->sibling = $2;
				    							$$ = $1;
												}
												else $$ = $2;
                 	}
									| declaracao
									{
										$$ = $1;
									}
									;
declaracao		: var_declaracao
							{
				  			$$ = $1;
							}
							| fun_declaracao
							{
				   			$$ = $1;
							}
							;
var_declaracao		: tipo_especificador id SEMI
									{
										$$ = $1;
				  					$$->child[0] = newStmtNode(VarK);
				  					$$->child[0]->type = $1->type;
				  					$$->child[0]->attr.name = $2->attr.name;

				  					$2->type = $1->type;
									}
									| tipo_especificador id LCH num RCH SEMI
									{
										$$ = $1;
				  					$$->child[0] = newStmtNode(VetK);
				  					$$->child[0]->type = $1->type;
				  					$$->child[0]->child[0] = $4;
				  					$$->child[0]->attr.name = $2->attr.name;

				  					$2->type = $4->type;
									}
									;
tipo_especificador	: INT
                  		{
												$$ = newStmtNode(DadoK);
                   		  $$->attr.name = "int";
				  							$$->type = Integer;
                      }
											| VOID
                  		{
												$$ = newStmtNode(DadoK);
                   		  $$->attr.name = "Void";
				  							$$->type = Void;
                      }
											;
fun_declaracao		: tipo_especificador id LPAREN params RPAREN composto_decl
									{
										$$ = $1;
				  					$$->child[0] = newStmtNode(FunK);
										$$->child[0]->attr.name = $2->attr.name;
										$$->child[0]->type = $1->type;
                    $$->child[0]->child[0] = $4;
                    $$->child[0]->child[1] = $6;

				  					$2->type = $1->type;
									}
									;
params			: param_lista
						{
							$$ = $1;
						}
						| VOID
						{
							$$ = NULL;
						}
						;
param_lista		: param_lista COMMA param
							{
								YYSTYPE t = $1;
                   	if (t != NULL)
                   	{
											while (t->sibling != NULL)
													t = t->sibling;
				    					t->sibling = $3;
				    					$$ = $1;
										}else
											$$ = $3;
										}
							| param
							{
								$$ = $1;
							}
							;
param			: tipo_especificador id
					{
						$$ = $1;
				  	$$->child[0] = newStmtNode(VarK);
				  	$$->child[0]->type = $1->type;
				  	$$->child[0]->attr.name = $2->attr.name;

				  	$2->type = $1->type;
					}
					| tipo_especificador id LCH RCH
					{
						$$ = $1;
				  	$$->child[0] = newStmtNode(VarK);
				  	$$->child[0]->type = $1->type;
				  	$$->child[0]->attr.name = $2->attr.name;

				  	$2->type = $1->type;
					}
					;
composto_decl		: LBKT local_declaracoes statement_lista RBKT
								{
									YYSTYPE t = $2;
                  	  if (t != NULL)
                  	  {
												while (t->sibling != NULL)
														t = t->sibling;
				    						t->sibling = $3;
				    						$$ = $2;
											} else
												$$ = $3;
								}
								;
local_declaracoes	: local_declaracoes var_declaracao
									{
										YYSTYPE t = $1;
                   	  if (t != NULL)
                   	  {
												while (t->sibling != NULL)
													t = t->sibling;
				    						t->sibling = $2;
				    						$$ = $1;
											} else
												$$ = $2;
									}
									|
									{
										$$ = NULL;
									}
									;
statement_lista		: statement_lista statement
									{
										YYSTYPE t = $1;
                   		  if (t != NULL)
                   		  {
													while (t->sibling != NULL)
															t = t->sibling;
				    							t->sibling = $2;
				    							$$ = $1;
												} else
													$$ = $2;
									}
									|
									{
										$$ = NULL;
									}
									;
statement		: expressao_decl
						{
							$$ = $1;
						}
						| composto_decl
						{
							$$ = $1;
						}
						| selecao_decl
						{
							$$ = $1;
						}
						| iteracao_decl
						{
							$$ = $1;
						}
						| retorno_decl
						{
							$$ = $1;
						}
						;
expressao_decl		: expressao SEMI
						{
				  		$$ = $1;
						}
						| SEMI
						{
							$$ = NULL;
						}
						;
selecao_decl		: IF LPAREN expressao RPAREN statement
								{
									$$ = newStmtNode(IfK);
				  				$$->child[0] = $3;
				  				$$->child[1] = $5;
								}
								| IF LPAREN expressao RPAREN statement ELSE statement
								{
									$$ = newStmtNode(IfK);
				  				$$->child[0] = $3;
				  				$$->child[1] = $5;
				  				$$->child[2] = $7;
								}
								;
iteracao_decl		: WHILE LPAREN expressao RPAREN statement
								{
									$$ = newStmtNode(RepeatK);
				  				$$->child[0] = $3;
				  				$$->child[1] = $5;
								}
								;
retorno_decl		: RETURN SEMI
								{
									$$ = newStmtNode(ReturnK);
									$$->attr.name = "return";
								}
								| RETURN expressao SEMI
								{
									$$ = newStmtNode(ReturnK);
				  				$$->child[0] = $2;
									$$->attr.name = "return";
								}
								;
expressao		: var ASSIGN expressao
						{
							$1->type = $$->type;

				  		$$ = newStmtNode(AtribK);
				  		$$->child[0] = $1;
				  		$$->child[1] = $3;
							$$->attr.name = "=";
						}
						| simples_expressao
						{
				  		$$ = $1;
						}
						;
var			: id
        {
					$1->type = $$->type;
				  $$ = $1;
				}
				| id LCH expressao RCH
      	{
					$1->type = $$->type;

					$$ = newExpNode(IdVetK);
					$$->child[0] = $3;
        	$$->attr.name = $1->attr.name;
      	}
				;
simples_expressao	: soma_expressao relacional soma_expressao
									{
										$$ = $2;
				  					$$->child[0] = $1;
				  					$$->child[1] = $3;
									}
									| soma_expressao
									{
										$$ = $1;
									}
									;
relacional		: LET
							{
								$$ = newExpNode(OpK);
                $$->attr.op = LET;
                $$->attr.name = "<=";
								$$->type = Boolean;
							}
							| LT
							{
								$$ = newExpNode(OpK);
                $$->attr.op = LT;
                $$->attr.name = "<";
								$$->type = Boolean;
							}
							| GT
							{
								$$ = newExpNode(OpK);
                $$->attr.op = GT;
                $$->attr.name = ">";
								$$->type = Boolean;
							}
							| GET
							{
								$$ = newExpNode(OpK);
                $$->attr.op = GET;
                $$->attr.name = ">=";
								$$->type = Boolean;
							}
							| EQ
							{
								$$ = newExpNode(OpK);
                $$->attr.op = EQ;
                $$->attr.name = "==";
								$$->type = Boolean;
							}
							| DIF
							{
								$$ = newExpNode(OpK);
                $$->attr.op = DIF;
                $$->attr.name = "!=";
								$$->type = Boolean;
							}
							;
soma_expressao		: soma_expressao soma termo
									{
										$$ = $2;
				  					$$->child[0] = $1;
				  					$$->child[1] = $3;
									}
									| termo
									{
										$$ = $1;
									}
									;
soma			: PLUS
					{
						$$ = newExpNode(OpK);
            $$->attr.op = PLUS;
            $$->attr.name = "+";
					}
					| MINUS
					{
						$$ = newExpNode(OpK);
            $$->attr.op = MINUS;
            $$->attr.name = "-";
					}
					;
termo			: termo mult fator
					{
						$$ = $2;
				  	$$->attr.op = $2->attr.op;
				  	$$->child[0] = $1;
				  	$$->child[1] = $3;
				}
				| fator
				{
					 $$ = $1;
				}
				;
mult		: TIMES
				{
					$$ = newExpNode(OpK);
          $$->attr.op = TIMES;
          $$->attr.name = "*";
				}
				| OVER
				{
					$$ = newExpNode(OpK);
          $$->attr.op = OVER;
          $$->attr.name = "/";
				}
				;
fator 	: LPAREN expressao RPAREN
				{
					$$ = $2;
				}
				| var
				{
					$1->type = $$->type;
				  $$ = $1;
				}
				| ativacao
				{
					$$ = $1;
				}
				| num
				{
					$$ = $1;
				}
				;
ativacao: id LPAREN args RPAREN
        {
					$1->type = $$->type;

				  $$ = newExpNode(IdFunK);
				  $$->child[0] = $3;
          $$->attr.name = $1->attr.name;
				}
				;
args		: arg_lista
				{
					$$ = $1;
				}
				|
				{
				 $$ = NULL;
			 	}
				;
arg_lista		: arg_lista COMMA expressao
						{
							YYSTYPE t = $1;
              if (t != NULL)
              {
								while (t->sibling != NULL)
										t = t->sibling;
				    		t->sibling = $3;
				    		$$ = $1;
							} else
								$$ = $3;
						}
						| expressao
						{
							$$ = $1;
						}
id 	: ID
		{
			$$ = newExpNode(IdK);
			$$->attr.name = copyString(tokenString);
		}
		;
num : NUM
		{
			$$ = newExpNode(ConstK);
			$$->attr.val = atoi(copyString(tokenString));
			$$->type = Integer;
		}
		;
%%

int yyerror(char * message)
{ fprintf(listing,"\n\tSyntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"\n\tCurrent token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with earlier versions of the TINY scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}
