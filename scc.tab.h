/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SCC_TAB_H_INCLUDED
# define YY_YY_SCC_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SEMI = 258,
    COMMA = 259,
    LC = 260,
    RC = 261,
    STRUCT = 262,
    RETURN = 263,
    IF = 264,
    BREAK = 265,
    CONT = 266,
    FOR = 267,
    TYPE = 268,
    ID = 269,
    READ = 270,
    WRITE = 271,
    INT = 272,
    ASSIGNOP = 273,
    ADDASSIGN = 274,
    SUBASSIGN = 275,
    MULASSIGN = 276,
    DIVASSIGN = 277,
    ANDASSIGN = 278,
    XORASSIGN = 279,
    ORASSIGN = 280,
    LSASSIGN = 281,
    RSASSIGN = 282,
    LOR = 283,
    LAND = 284,
    BOR = 285,
    BXOR = 286,
    BAND = 287,
    EQ = 288,
    NEQ = 289,
    GT = 290,
    LT = 291,
    NGT = 292,
    NLT = 293,
    SL = 294,
    SR = 295,
    ADD = 296,
    SUB = 297,
    MUL = 298,
    DIV = 299,
    MOD = 300,
    LNOT = 301,
    PIN = 302,
    PDE = 303,
    UMIN = 304,
    BNOT = 305,
    DOT = 306,
    LP = 307,
    RP = 308,
    LB = 309,
    RB = 310,
    ELSE = 311
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
#line 83 "scc.y" /* yacc.c:1915  */
union semrec
{
#line 84 "scc.y" /* yacc.c:1915  */

    int     intval; /* Integer values                              */
    char    *id;  /* Identifiers                                 */
    struct lbs *lbls; /* For backpatching                            */
    struct /*records type*/
    {
        Type type;
        int width;
        char *lexeme; 
        symrec *structST;
    } STypeVal;

    struct /*records for expression*/
    {
        symrec *address;
        Type type;
        symrec *array;  
        symrec *offset;
        list * truelist;
        list * falselist;
        list * nextlist;
        int isLeft;
        int isInt;
    } ExprType;
    struct /*records for args*/
    {
        symrec* array[100];
        int len;
        symrec *address;
    } ArgExpr;
    struct /*records for statements*/
    {
        list * nextlist;
        list * skiplist;
        list * breaklist;
    } ListExpr;

    Quad * instr;
  //}

#line 152 "scc.tab.h" /* yacc.c:1915  */
};
#line 83 "scc.y" /* yacc.c:1915  */
typedef union semrec YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SCC_TAB_H_INCLUDED  */
