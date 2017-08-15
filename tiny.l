%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}

digito 		[0-9]
letra 		[a-zA-Z]
id 		{letra}+
num 		{digito}+
newline     	\n
whitespace  	[ \t]+

%%
"int" 		  {return  INT;}
"void" 		{return  VOID;}
"if" 		  {return  IF;}
"then" 		{return  THEN;}
"else" 		{return  ELSE;}
"end" 		{return  END;}
"while" 	{return  WHILE;}
"return" 	{return  RETURN;}
"=" 	 	  {return  ASSIGN;}
"==" 		  {return  EQ;}
"!=" 		  {return  DIF;}
"<" 		  {return  LT;}
"<=" 		  {return  LET;}
">" 		  {return  GT;}
">=" 		  {return  GET;}
"+" 		  {return  PLUS;}
"-" 		  {return  MINUS;}
"*" 		  {return  TIMES;}
"/" 		  {return  OVER;}
"(" 		  {return  LPAREN;}
")" 		  {return  RPAREN;}
"[" 		  {return  LCH;}
"]" 		  {return  RCH;}
"{" 		  {return  LBKT;}
"}" 		  {return  RBKT;}
"," 		  {return  COMMA;}
";" 		  {return  SEMI;}
"/*"            {char c1, c2;
                 c1 = input();
                 do
                 { if (c1 == EOF)  break;
                   c2 = input();
                   if (c1 == '\n') lineno++;
                   if (c1 == '*' && c2 == '/') break;
                   c1 = c2;
                 } while (1);
                }
{num}	         {return  NUM;}
{id} 		       {return  ID;}
{newline}      {lineno++;}
{whitespace}   {/* ignora espaços em branco no codigo */}
.              {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (1) {
    printf("\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}
