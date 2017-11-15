/***************************************************************************
							TOKEN GENERATION
							
This flex file is used to transform the source program into tokens.
We give the regular expression to each token.                   
***************************************************************************/
%{
/*=========================================================================
                      C-libraries and Token  definitions
=========================================================================*/
#include <string.h>     
/*#include <stdlib.h> */    /* for atoi */
#ifndef TP
#define TP
#include "type.h"
#endif
#include "smallc.tab.h" /* for token definitions and yylval */

%}
/*=========================================================================
                             TOKEN Definitions
=========================================================================*/
DIGIT [0-9]
NONDIGIT [_A-Za-z]
INTEGER (({DIGIT}+)|(0x|0X|0)[0-9a-fA-F]+)
IDENTIFIER 	({NONDIGIT}({NONDIGIT}|{DIGIT})*)
BLANK	([ \f\r\t\v]+)
/*=========================================================================
      				REGULAR EXPRESSIONS FOR THE TOKENS
=========================================================================*/
%%
int                    	{
							yylval.STypeVal.type = Integer;
							yylval.STypeVal.width = 4;
							return TYPE;
						}                      	                                                                                             
struct                  {
                         	return STRUCT; 
                        }  
return                  { 
	                        return RETURN; 
	                    }  
if                      {
                        	return IF ; 
                        }  
else                    {
                        	return ELSE ;
                        }  
break                   {
                          	return BREAK;
                        }  
continue                {
                          	return CONT; 
                      	}  
for                     {
                          	return FOR;  
                        }
read					{							
							yylval.id = (char *) malloc (strlen(yytext)+1);
  							strcpy (yylval.id,yytext);
  							return READ;
						}
write					{
							yylval.id = (char *) malloc (strlen(yytext)+1);
  							strcpy (yylval.id,yytext);
  							return WRITE;
						}
[;]						{return SEMI;}

[,]						{return COMMA;}

{INTEGER}				{
							yylval.intval = strtol (yytext, NULL, 0);
							return INT;
						}

[-]                 	{return SUB;}

(\*)					{
							return MUL;
						}
(\/)					{
							return DIV;
						}
(%)						{
							return MOD;
						}												
[+]                    	{
							return ADD;
						}   						
(<<)					{
							return SL;
						}
(>>)					{
							return SR;
						}
(>)						{
							return GT;
						}
(<)						{
							return LT;
						}
(<=)						{
							return NGT;
						}
(>=)						{
							return NLT;
						}																		
(==)	             	{
							return EQ;
						}
(!=)	             	{
							return NEQ;
						}
[&]                   	{
							return BAND;
						}  
"^"                   	{
							return BXOR;
						}    
(\|)                    {
							return BOR;
						}    
(&&)                    {
							return LAND;
						}    
(\|\|)                  {
							return LOR;
						}    
(\+=)                   {
							return ADDASSIGN;
						}
(\-=)                   {
							return SUBASSIGN;
						}
(\*=)                   {
							return MULASSIGN;
						}
(\/=)                   {
							return DIVASSIGN;
						}			
(\&=)                   {
							return ANDASSIGN;
						}
(\^=)                   {
							return XORASSIGN;
						}
(\|=)                   {
							return ORASSIGN;
						}
(<<=)                   {
							return LSASSIGN;
						}			
(>>=)                   {
							return RSASSIGN;
						}
[=]                    	{
                        	return ASSIGNOP;
                    	}  
(!)						{
							return LNOT;
						}
(\+\+)					{
							return PIN;
						}
(\-\-)					{
							return PDE;
						}
(~)						{
							return BNOT;
						}						
[(]						{
							return LP;
						}
[)]						{
							return RP;
						}                      	
"[" 		                { 
                          	return LB;  
                        }  
"]" 		                { 
                          	return RB;  
                        }  
[{] 		            { 
                          	return LC;  
                        }
"}" 		            { 
                          	return RC;  
                        }       

{IDENTIFIER}			{
							yylval.id = (char *) malloc (strlen(yytext)+1);
  							strcpy (yylval.id,yytext);							 
							return ID;							
						}
[ \t\n]+ /* eat up whitespace */ 
"."						{
							return(DOT);
						}
.        				{ 
							return(yytext[0]);
						}
%%
int yywrap(void){}
/************************** End File *****************************/