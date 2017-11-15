%{/*************************************************************************
                        PARSER AND IR GENERATOR

The bison file is used to parse the token lists and generate the three-address-code.
The semantic check is also carried out during the pasring.
***************************************************************************/
/*=========================================================================
                    Headfiles and Libraries needed
=========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef TP
#define TP
#include "type.h"
#endif

#include "STSTACK.h" //symbol record stack
#include "INTSTACK.h" //integer stack
#include "ST.h" // Symbol Table
#include "quadruple.h" //three-address-code
#include "backpatch.h" //list for backpatch
#include "Reg.h" //register definition and allocation
#include "CG.h" //functions for code generation
#include "tools.h" //function for checking valid identifier
#define YYDEBUG 1 //for debugging
int yydebug=0;
int inloop;


/*=========================================================================
       Install the previously defined symbol into the symbol table
=========================================================================*/
symrec *install (char *sym_name, int offset, Type t, symrec *strField, int row, int col, int width)
{
    symrec *s;

    if (checkValid(sym_name) == 0)//if already installed, then it is semantic error
    {
        errors++;
        fprintf(stderr, "Variable %s is reserved.\n", sym_name);
    }
    s = currsymtablecheck(sym_name);
    if (s == 0)
    s = putsym(sym_name, offset, t, strField, 0, 0, row, col, width);
    else 
    { 
        errors++;
        fprintf(stderr, "Error: %s is already defined\n", sym_name );
    } 
    return s;
}

/*=========================================================================
                          SEMANTIC RECORDS
=========================================================================*/
%}
%union semrec /* The Semantic Records                        */
{
    int     intval; /* Integer values                              */
    char    *id;  /* Identifiers                                 */
    struct lbs *lbls; /* For backpatching                            */
    struct //attributes for non-terminal that holds the information of variables
    {
        Type type;  
        int width;  
        char *lexeme;  //the name of that variable
        symrec *structST;   //the unique symbol table for that variable (for function and structure)
    } STypeVal;

    struct //attribute for expression-related non-terminal
    {
        symrec *address;  //symbol record that holds the value of this expression
        Type type;  //type of the result of this expression

        symrec *array;  //if the expression is to access an array, 
                        //we need to store the symbol record for the array symbol

        symrec *offset; //if the expression is to access an array, 
                        //we need to store the symbol record for the accessing offset
        list * truelist; //truelist for backpatching
        list * falselist; //falselist for backpatching
        list * nextlist; //nextlist for backpatching
        int isLeft; //whether this expression can be left value
        int isInt; //whether the expression value is integer
        int val;
    } ExprType;
    struct //attribute for arguments-related non-terminal
    {
        symrec* array[100]; //holds all the arguments
        int len;  //holds the length of argument list
        symrec *address;
    } ArgExpr;

    struct //attributes for statement
    {
        list * nextlist; //nextlist for backpatching
        list * skiplist; //skiplist for backpatching
        list * breaklist; //breaklist for backpatching
    } ListExpr;

    Quad * instr; //attribute for empty production for backpatching
  //}
}

/*=========================================================================
                               TOKENS
=========================================================================*/
%start program
%token SEMI COMMA LC RC STRUCT RETURN IF BREAK CONT FOR
%token <STypeVal> TYPE
%token <id> ID READ WRITE
%token <intval> INT

/*=========================================================================
                          OPERATOR PRECEDENCE
=========================================================================*/
%right ASSIGNOP ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN ANDASSIGN XORASSIGN ORASSIGN LSASSIGN RSASSIGN
%left LOR
%left LAND
%left BOR
%left BXOR
%left BAND
%left EQ NEQ
%left GT LT NGT NLT
%left SL SR
%left ADD SUB
%left MUL DIV MOD
%right LNOT PIN PDE UMIN BNOT
%left DOT LP RP LB RB
%nonassoc ELSE
/*=========================================================================
                               NON-TERMINALS
=========================================================================*/
%type <STypeVal> extvars var stspec sdecs sdefs func
%type <ExprType> exps exp
%type <ArgExpr> args init
%type <instr> m
%type <ListExpr> n stmt stmts stmtblock
%type <intval> paras

/*=========================================================================
                   GRAMMAR RULES for the SmallC language
=========================================================================*/
%%
program : 
    {
        offset = 0; 
        inloop = 0;
        ST_Stack_Init(&StStack); INT_Stack_Init(&OffsetStack); 
        codeHead = malloc(sizeof(Quad)); codeHead->next = NULL; 
        putsym("read", 0, Function, NULL, 0, 0, 0, 0, 0); 
        putsym("write", 0, Function, NULL, 0, 0, 0, 0, 0); 
    } extdefs
;

extdefs : /*empty*/
    {
        
    }
    | extdef extdefs
    {
        
    }
;

extdef : 
    TYPE extvars SEMI
    {

    }
    | stspec sextvars SEMI
    {
        
    }
    
    | TYPE func stmtblock
    {
        symrec * pt;
        symrec * temp = sym_table;
        pt = getsym($<STypeVal.lexeme>2);
        pt->strField = temp;    //set the unique symbol table for function
        sym_table = ST_Stack_Pop(&StStack); //restore symbol table
        pt = getsym($<STypeVal.lexeme>2);
        pt->strField = temp;  //set the unique symbol table for function        
        offset = INT_Stack_Pop(&OffsetStack);   //restore offset
    }
;

sextvars : /*empty*/
    {
        
    }
    | ID 
    {
        if (getsym($1) != 0)    //ID not defined
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s Redeclaration!\n", $1);
        }
        //sextvars should also holds information for this structure
        $<STypeVal.type>$ = $<STypeVal.type>0;
        $<STypeVal.width>$ = $<STypeVal.width>0;
        $<STypeVal.lexeme>$ = $1;
        $<STypeVal.structST>$ = $<STypeVal.structST>0;
        //install ID
        sym_table = install($<STypeVal.lexeme>$, offset, Struct, $<STypeVal.structST>$, 0, 0, $<STypeVal.width>$);

        //install all of ID' s attributes    
        symrec * ori = sym_table;
        symrec * str = $<STypeVal.structST>$;
        char * s1;
        char *s2, *s;
        for (int i = 0; i < $<STypeVal.width>$; i = i + 4)
        {
            sym_table = str;
            symrec *pt = getsym_by_offset(i);
            s2 = pt->name;
            s = malloc(sizeof(char) * (strlen($1) + strlen(s2) + 3));
            strcpy(s, $1);
            strcat(s, ".");
            strcat(s, s2);
            sym_table = ori;            
            ori = install(s, offset + i, Integer, NULL, 0, 0, 4);
        }
        sym_table = ori;
        //move the offset ahead
        offset += $<STypeVal.width>$;
    }
    | sextvars COMMA ID //the same as previous one
    {
        if (getsym($3) != 0)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable Redeclaration!\n");
        }
        $<STypeVal.type>$ = $<STypeVal.type>1;
        $<STypeVal.width>$ = $<STypeVal.width>1;
        $<STypeVal.lexeme>$ = $3;
        $<STypeVal.structST>$ = $<STypeVal.structST>1;
        sym_table = install($<STypeVal.lexeme>$, offset, Struct, $<STypeVal.structST>$, 0, 0, $<STypeVal.width>$);

        symrec * ori = sym_table;
        symrec * str = $<STypeVal.structST>$;
        char * s1;
        char *s2;
        for (int i = 0; i < $<STypeVal.width>$; i = i + 4)
        {
            char *s;
            sym_table = str;
            symrec *pt = getsym_by_offset(i);
            s2 = pt->name;
            s = malloc(sizeof(char) * (strlen($3) + strlen(s2) + 3));
            strcpy(s, $3);
            strcat(s, ".");
            strcat(s, s2);
            sym_table = ori;            
            ori = install(s, offset + i, Integer, NULL, 0, 0, 4);
        }
        sym_table = ori;
        offset += $<STypeVal.width>1;
    }
;
extvars : /*empty*/
    {
        
    }
    |var
    {
        if ($<STypeVal.type>1 == Integer)
        {
            //quad_push("=", $<ExprType.address>3, NULL, getsym($<STypeVal.lexeme>1), NULL);
            getsym($<STypeVal.lexeme>1)->ConstVal = 0;
        }   
        else if ($<STypeVal.type>1 == Array)    //set initial value to zero
        {
            symrec *ptr = getsym($<STypeVal.lexeme>1);
            int length = ptr->row * ptr->col;
            ptr->arrayInit = (int *) malloc ((length+1) * sizeof(int));
            for (int i = 0; i < length; i++) ptr->arrayInit[i] = 0;
        }
    }    
    | var ASSIGNOP init
    {
        if (($<STypeVal.type>1 == Integer) && ($<ArgExpr.len>3 == 0))
        {
            //ConstVal is used to hold initial value
            getsym($<STypeVal.lexeme>1)->ConstVal = $<ExprType.address>3->ConstVal;  
        }
        else if (($<STypeVal.type>1 == Array) && ($<ArgExpr.len>3 > 0)) //set initial value
        {
            symrec *ptr = getsym($<STypeVal.lexeme>1), *pt, *zero;
            zero = putconst(0);
            int length = ptr->row * ptr->col;
            int base = ptr->offset;
            ptr->arrayInit = (int *) malloc ((length+1) * sizeof(int));
            for (int i = 0; i < length; i++)
            {
                //pt = putconst(4*i);
                if (i < $<ArgExpr.len>3)
                {
                    //init has attributes holding initial value
                    ptr->arrayInit[i] = $<ArgExpr.array>3[i]->ConstVal;     
                    //quad_push("[]=", ptr, pt, $<ArgExpr.array>3[i], NULL);
                }
                else ptr->arrayInit[i] = 0;
                //quad_push("[]=", ptr, pt, zero, NULL);
            }
        }
    }
    | extvars COMMA var //the same as previous
    {
        if ($<STypeVal.type>3 == Integer)
        {
            //quad_push("=", $<ExprType.address>3, NULL, getsym($<STypeVal.lexeme>1), NULL);
            getsym($<STypeVal.lexeme>3)->ConstVal = 0;
        }   
        else if ($<STypeVal.type>3 == Array)
        {
            symrec *ptr = getsym($<STypeVal.lexeme>3);
            int length = ptr->row * ptr->col;
            ptr->arrayInit = (int *) malloc ((length+1) * sizeof(int));
            for (int i = 0; i < length; i++) ptr->arrayInit[i] = 0;
        }
    }
    | extvars COMMA var ASSIGNOP init //the same as previous
    {
        if (($<STypeVal.type>3 == Integer) && ($<ArgExpr.len>5 == 0))
        {
            //quad_push("=", $<ExprType.address>5, NULL, getsym($<STypeVal.lexeme>3), NULL);
            getsym($<STypeVal.lexeme>3)->ConstVal = $<ExprType.address>5->ConstVal;
        }
        else if (($<STypeVal.type>3 == Array) && ($<ArgExpr.len>5 > 0))
        {
            symrec *ptr = getsym($<STypeVal.lexeme>3), *pt, *zero;
            zero = putconst(0);
            int length = ptr->row * ptr->col;
            int base = ptr->offset;
            ptr->arrayInit = (int *) malloc ((length+1) * sizeof(int));
            for (int i = 0; i < length; i++)
            {
                //pt = putconst(4*i);
                if (i < $<ArgExpr.len>5)
                {
                    //quad_push("[]=", ptr, pt, $<ArgExpr.array>5[i], NULL);
                    ptr->arrayInit[i] = $<ArgExpr.array>5[i]->ConstVal;
                }
                else ptr->arrayInit[i] = 0;
                //else quad_push("[]=", ptr, pt, zero, NULL);
            }
        }        
    }
;
var : ID
    {
        //set information
        $<STypeVal.type>$ = Integer;
        $<STypeVal.width>$ = 4;
        $<STypeVal.lexeme>$ = $1;
        //install ID
        sym_table = install($<STypeVal.lexeme>$, offset, Integer, NULL, 0, 0, 4);
        offset += $<STypeVal.width>$;
    }
    | ID LB INT RB
    {    
        //set information
        $<STypeVal.type>$ = Array;
        $<STypeVal.width>$ = $3 * 4;
        $<STypeVal.lexeme>$ = $1;
        //install ID
        sym_table = install($1, offset, Array, NULL, 1, $3, $<STypeVal.width>$);
        offset += $<STypeVal.width>$;
    }
    | ID LB INT RB LB INT RB
    {
        //the same
        $<STypeVal.type>$ = Array;
        $<STypeVal.width>$ = $3 * 4 * $6;
        $<STypeVal.lexeme>$ = $1;
        sym_table = install($1, offset, Array, NULL, $3, $6, $<STypeVal.width>$);
        offset += $<STypeVal.width>$;   
    }    
;

stspec : 
    STRUCT ID LC
    {
        ST_Stack_Push(&StStack, sym_table);//store symbol table        
        sym_table = (symrec *)0;
        INT_Stack_Push(&OffsetStack, offset);//store offset
        offset = 0;
    }
    sdefs RC
    {
        //set attributes of stspec
        $<STypeVal.structST>$ = sym_table;
        $<STypeVal.type>$ = Struct;
        $<STypeVal.width>$ = offset;
        //restore environment
        sym_table = ST_Stack_Pop(&StStack);
        offset = INT_Stack_Pop(&OffsetStack);
        sym_table = install($2, 0, StructID, $<STypeVal.structST>$, 0, 0, $<STypeVal.width>$);                
    }
    | STRUCT LC  //the same as previous
    {
        ST_Stack_Push(&StStack, sym_table);
        //sym_table = (symrec *) malloc (sizeof(symrec));
        sym_table = (symrec *)0;
        INT_Stack_Push(&OffsetStack, offset);        
        offset = 0;
    }
    sdefs RC
    {
        $<STypeVal.structST>$ = sym_table;
        $<STypeVal.type>$ = Struct;
        $<STypeVal.width>$ = offset;
        sym_table = ST_Stack_Pop(&StStack);    
        offset = INT_Stack_Pop(&OffsetStack);
    }
    | STRUCT ID
    {
        //search the symbol table
        symrec *pt = getsym($2);        
        if (pt == NULL)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", $2);
        }
        //set the attribute of stspec                    
        $<STypeVal.structST>$ = pt->strField;        
        $<STypeVal.type>$ = Struct;
        $<STypeVal.width>$ = pt->width;
    }
;

defs : /*empty*/
    | TYPE decs SEMI defs
    | stspec sdecs SEMI defs
;

sdefs : /*empty*/
    {
        $<STypeVal.width>$ = 0;        
    }
    | TYPE sdecs SEMI sdefs
    {
        $$ = $2;        
        $<STypeVal.width>$ = $<STypeVal.width>2 + $<STypeVal.width>4;
        
    }
;

sdecs : sdecs COMMA ID //almost the same as extvars
        {
            $<STypeVal.type>$ = $<STypeVal.type>1;
            if ($<STypeVal.type>$ == Struct)//actually type cannot be structure
            {
                if (getsym($3) != 0)
                {
                    errors++;
                    fprintf(stderr, "Semantic Error!\n");
                    fprintf(stderr, "Variable %s Redeclaration!\n", $3);
                }
                $<STypeVal.type>$ = $<STypeVal.type>1;
                $<STypeVal.width>$ = $<STypeVal.width>1;
                $<STypeVal.lexeme>$ = $3;
                $<STypeVal.structST>$ = $<STypeVal.structST>1;
                sym_table = install($<STypeVal.lexeme>$, offset, Struct, $<STypeVal.structST>$, 0, 0, $<STypeVal.width>$);
    
                symrec * ori = sym_table;
                symrec * str = $<STypeVal.structST>$;
                char * s1;
                char *s2;
                for (int i = 0; i < $<STypeVal.width>$; i = i + 4)
                {
                    char *s;
                    sym_table = str;
                    symrec *pt = getsym_by_offset(i);
                    s2 = pt->name;
                    s = malloc(sizeof(char) * (strlen($3) + strlen(s2) + 3));
                    strcpy(s, $3);
                    strcat(s, ".");
                    strcat(s, s2);
                    sym_table = ori;                    
                    ori = install(s, offset + i, Integer, NULL, 0, 0, 4);
                }
                sym_table = ori;
                offset += $<STypeVal.width>1;
            }
            else
            {//install the symbol and move offset ahead
                $<STypeVal.width>$ = $<STypeVal.width>1;
                sym_table = install($3, offset, $<STypeVal.type>1, NULL, 0, 0, $<STypeVal.width>$);
                $<STypeVal.lexeme>$ = $3;            
                offset += $<STypeVal.width>$;
            }
        }
    | ID
    {//the same as extvars
        $<STypeVal.type>$ = $<STypeVal.type>0;            
        if ($<STypeVal.type>$ == Struct)
        {            
            if (getsym($1) != 0)
            {
                errors++;
                fprintf(stderr, "Semantic Error!\n");
                fprintf(stderr, "Variable %s Redeclaration!\n", $1);
            }
            $<STypeVal.type>$ = $<STypeVal.type>0;
            $<STypeVal.width>$ = $<STypeVal.width>0;
            $<STypeVal.lexeme>$ = $1;
            $<STypeVal.structST>$ = $<STypeVal.structST>0;
            sym_table = install($<STypeVal.lexeme>$, offset, Struct, $<STypeVal.structST>$, 0, 0, $<STypeVal.width>$);

            symrec * ori = sym_table;
            symrec * str = $<STypeVal.structST>$;            
            char * s1;
            char *s2, *s;
            for (int i = 0; i < $<STypeVal.width>$; i = i + 4)
            {                
                sym_table = str;                
                symrec *pt = getsym_by_offset(i);                
                s2 = pt->name;                
                s = malloc(sizeof(char) * (strlen($1) + strlen(s2) + 3));                
                strcpy(s, $1);
                strcat(s, ".");                
                strcat(s, s2);
                sym_table = ori;                                
                ori = install(s, offset + i, Integer, NULL, 0, 0, 4);
                ori = sym_table;
            }
            sym_table = ori;
            offset += $<STypeVal.width>$;            
        }

        else
        {
            $<STypeVal.width>$ = $<STypeVal.width>0;
            sym_table = install($1, offset, Integer, NULL, 0, 0, $<STypeVal.width>$);
            $<STypeVal.lexeme>$ = $1;        
            offset += $<STypeVal.width>$;
        }
    }
;

decs : 
    var
    {        
    }
    | decs COMMA var
    {        
    }
    |  decs COMMA var ASSIGNOP init
    {
        symrec *pt = getsym($<STypeVal.lexeme>3);
        if (pt == 0)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", $<STypeVal.lexeme>3);
        }
        if (($<STypeVal.type>3 == Integer) && ($<ArgExpr.len>5 == 0))
        {            
            quad_push("=", $<ExprType.address>5, NULL, getsym($<STypeVal.lexeme>3), NULL);
        }
        else if (($<STypeVal.type>3 == Array) && ($<ArgExpr.len>5 > 0))
        {
            symrec *ptr = getsym($<STypeVal.lexeme>3), *pt, *zero;
            zero = putconst(0);
            int length = ptr->row * ptr->col;
            int base = ptr->offset;
            for (int i = 0; i < length; i++)
            {
                pt = putconst(4*i);
                if (i < $<ArgExpr.len>5)
                {
                    quad_push("[]=", ptr, pt, $<ArgExpr.array>5[i], NULL);
                }
                else quad_push("[]=", ptr, pt, zero, NULL);
            }
        }
    }
    | var ASSIGNOP init
    {
        symrec *pt = getsym($<STypeVal.lexeme>1);        
        if (pt == 0)  //if var not defined, error
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", $<STypeVal.lexeme>1);
        }
        if (($<STypeVal.type>1 == Integer) && ($<ArgExpr.len>3 == 0)) //integer, derect assign
        {

            quad_push("=", $<ExprType.address>3, NULL, getsym($<STypeVal.lexeme>1), NULL);
        }
        else if (($<STypeVal.type>1 == Array) && ($<ArgExpr.len>3 > 0))  //array, use a loop to assign
        {
            symrec *ptr = getsym($<STypeVal.lexeme>1), *pt, *zero;//get the symbol record for array
            zero = putconst(0);
            int length = ptr->row * ptr->col;
            int base = ptr->offset;
            for (int i = 0; i < length; i++)
            {
                pt = putconst(4*i); //1 integer is 4 bytes
                if (i < $<ArgExpr.len>3)
                {
                    quad_push("[]=", ptr, pt, $<ArgExpr.array>3[i], NULL);
                }
                else quad_push("[]=", ptr, pt, zero, NULL);
            }
        }
    }
;

exp : /*empty*/
    {
        $<ExprType.address>$ = 0;        
    }
    | exps
    {//passing attributes
        $<ExprType.address>$ = $<ExprType.address>1;
        $<ExprType.type>$ = $<ExprType.type>1;
        $<ExprType.array>$ = $<ExprType.array>1;
        $<ExprType.offset>$ = $<ExprType.offset>1;
        $<ExprType.truelist>$ = $<ExprType.truelist>1;
        $<ExprType.falselist>$ = $<ExprType.falselist>1;
        $<ExprType.nextlist>$ = $<ExprType.nextlist>1;        
        $<ExprType.isInt>$ = $<ExprType.isInt>1;        
        $<ExprType.isLeft>$ = $<ExprType.isLeft>1;        
    }
;

init : exp   
    {        
        $<ArgExpr.address>$ = $<ExprType.address>1;
        $<ArgExpr.len>$ = 0;//indicate it is init for integer var
    } 
    | LC args RC
    {
        $<ArgExpr.address>$ = 0;
        $<ArgExpr.len>$ = $<ArgExpr.len>2;
        for (int i = 0; i < $<ArgExpr.len>2; i++)
        {
            $<ArgExpr.array>$[i] = $<ArgExpr.array>2[i];
        }
    }
;

exps : 
    READ LP ID RP
    {
        symrec *pt = getsym($3);
        if (pt == 0)
        {//whether ID is defined
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", $3);
        }
        else if ((pt->type != Integer) && (pt->type != Constant) && (pt->type != Temp))
        {
            errors++;
            fprintf(stderr, "Function argument type not fit.\n");   
        }
        else //load argument and call function
        {
            quad_push("param", getsym($3), NULL, NULL, NULL);
            symrec * pt = putconst(1);
            quad_push("call", getsym($1), pt, getsym($3), NULL);   
        }
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 0;
    }
    | WRITE LP exps RP //the same as read
    {   
        if ($<ExprType.isInt>3 != 1)
        {
            errors++;
            fprintf(stderr, "Function argument type not fit.\n");
        }        
        quad_push("param", $<ExprType.address>3, NULL, NULL, NULL);
        symrec * pt = putconst(1);
        quad_push("call", getsym($1), pt, NULL, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 0;
    }
/*==========================================================
                    BINARY OPERATION
==========================================================*/
    | exps ADD exps
    {
        tempCnt ++;
        symrec *tp;
        tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);//generate the temporal variable
        $<ExprType.address>$ = tp;
        //if both are constant, we can calculate the value
        if (($<ExprType.type>1 == Constant) && ($<ExprType.type>3 == Constant))
        {
            $<ExprType.type>$ = Constant;
            $<ExprType.address>$ = putconst($<ExprType.address>1->ConstVal + $<ExprType.address>3->ConstVal);
        } else //otherwise generate the quadruple to compute the result
        quad_push("+", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
        
    }
    | exps SUB exps
    {//below are almost the same as ADD operation
        tempCnt ++;
        $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (($<ExprType.type>1 == Constant) && ($<ExprType.type>3 == Constant))
        {
            $<ExprType.type>$ = Constant;
            $<ExprType.address>$ = putconst($<ExprType.address>1->ConstVal - $<ExprType.address>3->ConstVal);
        } else
        quad_push("-", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
        
    }    
    | exps MUL exps
    {
        tempCnt ++;
        $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (($<ExprType.type>1 == Constant) && ($<ExprType.type>3 == Constant))
        {
            $<ExprType.type>$ = Constant;
            $<ExprType.address>$ = putconst($<ExprType.address>1->ConstVal * $<ExprType.address>3->ConstVal);
        } else
        quad_push("*", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
        
    }
    | exps DIV exps
    {
        tempCnt ++;
        $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (($<ExprType.type>1 == Constant) && ($<ExprType.type>3 == Constant))
        {
            $<ExprType.type>$ = Constant;
            $<ExprType.address>$ = putconst($<ExprType.address>1->ConstVal / $<ExprType.address>3->ConstVal);
        } else
        quad_push("/", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
        
    }
    | exps MOD exps
    {
        tempCnt ++;
        $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (($<ExprType.type>1 == Constant) && ($<ExprType.type>3 == Constant))
        {
            $<ExprType.type>$ = Constant;
            $<ExprType.address>$ = putconst($<ExprType.address>1->ConstVal % $<ExprType.address>3->ConstVal);
        } else
        quad_push("%", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
        
    }
    | exps SL exps
    {
        tempCnt ++;
        $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (($<ExprType.type>1 == Constant) && ($<ExprType.type>3 == Constant))
        {
            $<ExprType.type>$ = Constant;
            $<ExprType.address>$ = putconst($<ExprType.address>1->ConstVal << $<ExprType.address>3->ConstVal);
        } else
        quad_push("<<", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
        
    }
    | exps SR exps
    {
        tempCnt ++;
        $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (($<ExprType.type>1 == Constant) && ($<ExprType.type>3 == Constant))
        {
            $<ExprType.type>$ = Constant;
            $<ExprType.address>$ = putconst($<ExprType.address>1->ConstVal >> $<ExprType.address>3->ConstVal);
        } else
        quad_push(">>", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
        
    }
    | exps BAND exps
    {
        tempCnt ++;
        $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (($<ExprType.type>1 == Constant) && ($<ExprType.type>3 == Constant))
        {
            $<ExprType.type>$ = Constant;
            $<ExprType.address>$ = putconst($<ExprType.address>1->ConstVal & $<ExprType.address>3->ConstVal);
        } else
        quad_push("&", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
        
    }
    | exps BOR exps
    {
        tempCnt ++;
        $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (($<ExprType.type>1 == Constant) && ($<ExprType.type>3 == Constant))
        {
            $<ExprType.type>$ = Constant;
            $<ExprType.address>$ = putconst($<ExprType.address>1->ConstVal | $<ExprType.address>3->ConstVal);
        } else
        quad_push("|", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
        
    }
    | exps BXOR exps
    {
        tempCnt ++;
        $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (($<ExprType.type>1 == Constant) && ($<ExprType.type>3 == Constant))
        {
            $<ExprType.type>$ = Constant;
            $<ExprType.address>$ = putconst($<ExprType.address>1->ConstVal ^ $<ExprType.address>3->ConstVal);
        } else
        quad_push("^", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
        
    }
/*==========================================================
                    ASSIGN OPERATION
==========================================================*/
    | exps ASSIGNOP exps
    {        
        if ($<ExprType.isLeft>1 == 0)//left value check 
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ($<ExprType.type>1 == Array) 
        {
            symrec *zero = putconst(0);
            //assign value to an array
            quad_push("[]=", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>3, NULL);
        }
        else quad_push("=", $<ExprType.address>3, NULL, $<ExprType.address>1, NULL);//assign integer value
        $<ExprType.address>$ = $<ExprType.address>1;
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps ADDASSIGN exps
    {
        if ($<ExprType.isLeft>1 == 0)// left value check
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ($<ExprType.type>1 == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            //t get the value of left expression
            quad_push("=[]", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
            //do the addition
            quad_push("+", $<ExprType.address>$, $<ExprType.address>3, $<ExprType.address>$, NULL);
            //store t to the left expression
            quad_push("[]=", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
        }
        else 
        {
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            //add left and right, store to left
            quad_push("+", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>1, NULL);
        }
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps SUBASSIGN exps
    {//below are the same as ADDASSIGN
        if ($<ExprType.isLeft>1 == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ($<ExprType.type>1 == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
            quad_push("-", $<ExprType.address>$, $<ExprType.address>3, $<ExprType.address>$, NULL);
            quad_push("[]=", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
        }
        else 
        {
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("-", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>1, NULL);
        }
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps MULASSIGN exps
    {
        if ($<ExprType.isLeft>1 == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ($<ExprType.type>1 == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
            quad_push("*", $<ExprType.address>$, $<ExprType.address>3, $<ExprType.address>$, NULL);
            quad_push("[]=", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
        }
        else 
        {
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("*", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>1, NULL);
        }
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps DIVASSIGN exps
    {
        if ($<ExprType.isLeft>1 == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ($<ExprType.type>1 == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
            quad_push("/", $<ExprType.address>$, $<ExprType.address>3, $<ExprType.address>$, NULL);
            quad_push("[]=", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
        }
        else 
        {
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("/", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>1, NULL);
        }

        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }    
    | exps ANDASSIGN exps
    {
        if ($<ExprType.isLeft>1 == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ($<ExprType.type>1 == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
            quad_push("&", $<ExprType.address>$, $<ExprType.address>3, $<ExprType.address>$, NULL);
            quad_push("[]=", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
        }
        else 
        {
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("&", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>1, NULL);
        }
        
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps ORASSIGN exps
    {
        if ($<ExprType.isLeft>1 == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ($<ExprType.type>1 == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
            quad_push("|", $<ExprType.address>$, $<ExprType.address>3, $<ExprType.address>$, NULL);
            quad_push("[]=", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
        }
        else 
        {
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("|", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>1, NULL);
        }
        
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }    
    | exps XORASSIGN exps
    {
        if ($<ExprType.isLeft>1 == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ($<ExprType.type>1 == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
            quad_push("^", $<ExprType.address>$, $<ExprType.address>3, $<ExprType.address>$, NULL);
            quad_push("[]=", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
        }
        else 
        {
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("^", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>1, NULL);
        }
        
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps LSASSIGN exps
    {
        if ($<ExprType.isLeft>1 == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ($<ExprType.type>1 == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
            quad_push("<<", $<ExprType.address>$, $<ExprType.address>3, $<ExprType.address>$, NULL);
            quad_push("[]=", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
        }
        else 
        {
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("<<", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>1, NULL);
        }
        
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps RSASSIGN exps
    {
        if ($<ExprType.isLeft>1 == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ($<ExprType.type>1 == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
            quad_push(">>", $<ExprType.address>$, $<ExprType.address>3, $<ExprType.address>$, NULL);
            quad_push("[]=", $<ExprType.array>1, $<ExprType.offset>1, $<ExprType.address>$, NULL);
        }
        else 
        {
            tempCnt ++;
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push(">>", $<ExprType.address>1, $<ExprType.address>3, $<ExprType.address>1, NULL);
        }
        
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }            
    //TODO: more
/*==========================================================    
                BEGINNING OF BOOLEAN PART
==========================================================*/                    
//All of the Boolean part is the same as the Dragon Book
    | exps LOR m exps
    {
        backpatch($<ExprType.falselist>1, $<instr>3);
        $<ExprType.truelist>$ = merge($<ExprType.truelist>1, $<ExprType.truelist>4);
        $<ExprType.falselist>$ = $<ExprType.falselist>4;
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }

    | exps LAND m exps
    {
        backpatch($<ExprType.truelist>1, $<instr>3);
        $<ExprType.truelist>$ = $<ExprType.truelist>4;
        $<ExprType.falselist>$ = merge($<ExprType.falselist>1, $<ExprType.falselist>4);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }

    | LNOT exps
    {
        $<ExprType.truelist>$ = $<ExprType.falselist>2;
        $<ExprType.falselist>$ = $<ExprType.truelist>2;
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps GT exps
    {
        $<ExprType.truelist>$ = makelist(get_nextinst());
        quad_push(">", $<ExprType.address>1, $<ExprType.address>3, NULL, NULL);
        $<ExprType.falselist>$ = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps NLT exps
    {
        $<ExprType.truelist>$ = makelist(get_nextinst());
        quad_push(">=", $<ExprType.address>1, $<ExprType.address>3, NULL, NULL);
        $<ExprType.falselist>$ = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps LT exps
    {
        $<ExprType.truelist>$ = makelist(get_nextinst());
        quad_push("<", $<ExprType.address>1, $<ExprType.address>3, NULL, NULL);
        $<ExprType.falselist>$ = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps NGT exps
    {
        $<ExprType.truelist>$ = makelist(get_nextinst());
        quad_push("<=", $<ExprType.address>1, $<ExprType.address>3, NULL, NULL);
        $<ExprType.falselist>$ = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps EQ exps
    {
        $<ExprType.truelist>$ = makelist(get_nextinst());
        quad_push("==", $<ExprType.address>1, $<ExprType.address>3, NULL, NULL);
        $<ExprType.falselist>$ = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);   
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | exps NEQ exps
    {
        $<ExprType.truelist>$ = makelist(get_nextinst());
        quad_push("!=", $<ExprType.address>1, $<ExprType.address>3, NULL, NULL);
        $<ExprType.falselist>$ = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
/*==========================================================    
                UNARY OPERATION
==========================================================*/                    
    | SUB exp %prec UMIN
    {
        symrec *zero = putconst(0);
        symrec *tp;
        //if exp is constant, we can just get the negative value of the constant as result
        if ($<ExprType.address>2->type == Constant)
        {
            $<ExprType.address>2 = putconst($<ExprType.address>2->ConstVal * -1);
            $<ExprType.address>$ = $<ExprType.address>2;
        }
        else//other wise generate a quadruple to compute the resule
        {
            tempCnt ++;    
            tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            $<ExprType.address>$ = tp;
            quad_push("-", zero, $<ExprType.address>2, $<ExprType.address>$, NULL);
        }
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | BNOT exp//almost the same as SUb exp
    {        
        if ($<ExprType.address>2->type == Constant)
        {
            $<ExprType.address>2 = putconst(~($<ExprType.address>2->ConstVal));
            $<ExprType.address>$ = $<ExprType.address>2;
        }
        else
        {
            tempCnt ++;        
            symrec *tp;
            tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            $<ExprType.address>$ = tp;
            quad_push("~", $<ExprType.address>2, NULL, $<ExprType.address>$, NULL);
        }
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | PIN exp
    {
        tempCnt ++;        
        symrec *tp;
        tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
        $<ExprType.address>$ = tp;
        symrec *one = putconst(1);
        quad_push("+", $<ExprType.address>2, one, $<ExprType.address>2, NULL); //increase exp by 1
        quad_push("=", $<ExprType.address>2, NULL, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | PDE exp
    {
        tempCnt ++;        
        symrec *tp;
        tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
        $<ExprType.address>$ = tp;
        symrec *one = putconst(1);
        quad_push("-", $<ExprType.address>2, one, $<ExprType.address>2, NULL); //decrease exp by 1
        quad_push("=", $<ExprType.address>2, NULL, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }

    | LP exps RP
    {//just assign all attributes of exps to exp
        $<ExprType.address>$ = $<ExprType.address>2;
        $<ExprType.truelist>$ = $<ExprType.truelist>2;
        $<ExprType.falselist>$ = $<ExprType.falselist>2;
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = $<ExprType.isInt>1;
    }    
    | ID LP args RP
    {        
        if ($<ArgExpr.len>3 != getsym($1)->paranum)//semantic check: whether have the same length as defined 
        {
            errors++;
            fprintf(stderr, "Function argument number not fit.\n");
        }
        for (int i = 0; i < $<ArgExpr.len>3; i++)
        {
            //load parameters
            quad_push("param", $<ArgExpr.array>3[i], NULL, NULL,  NULL);
            //parameter length not fit
            if (($<ArgExpr.array>3[i]->type != Integer) && ($<ArgExpr.array>3[i]->type != Constant) && ($<ArgExpr.array>3[i]->type != Temp))
            {
                errors++;
                fprintf(stderr, "Function argument type not fit.\n");
            }
        }
        tempCnt++;
        $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
        symrec *l = putconst($<ArgExpr.len>3);

        symrec *pt = getsym($1);
        if (pt == 0)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", $1);
        }
        //call function, store value in temporal variable
        quad_push("call", getsym($1), l, $<ExprType.address>$, NULL);
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
    | ID arrs
    {
        if (($<ExprType.address>2) == 0)//it is an integer variable
        {
            symrec *pt = getsym($<id>1);
            if (pt == 0) //semantic check: whether has been defined
            {
                errors++;
                fprintf(stderr, "Semantic Error!\n");
                fprintf(stderr, "Variable %s not declared!\n", $<id>1);
            }
            else if (pt->type != Integer) //whether it is an integer, not structure
            {
                errors++;
                fprintf(stderr, "Semantic Error!\n");
                fprintf(stderr, "Variable %s is not Integer!\n", $<id>1);   
            }
            $<ExprType.address>$ = getsym($<id>1);
        }
        else //it is an array element
        {
            if (getsym($1)->type != Array)
            {
                errors++;
                fprintf(stderr, "Variable %s is not an array.\n", $1);
            }
            symrec *zero = putconst(0);
            //set exps' attribute
            $<ExprType.type>$ = Array;
            $<ExprType.offset>$ = $<ExprType.address>2;
            $<ExprType.array>$ = getsym($1);            
            tempCnt ++;        
            $<ExprType.address>$ = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", $<ExprType.array>$, $<ExprType.offset>$, $<ExprType.address>$, NULL);
        }
        $<ExprType.isLeft>$ = 1;
        $<ExprType.isInt>$ = 1;
    }
    | ID DOT ID
    {
        if (getsym($1)->type != Struct)//semantic check
        {
            errors++;
            fprintf(stderr, "Varible %s is not struct type.\n", $1);
        }
        char *s;
        symrec *pt = getsym($<id>1);
        if (pt == 0)//semantic check
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", $<id>1);
        }
        //get the name of structure attribute in symbol table
        $<ExprType.address>$ = getsym($<id>1);
        s = malloc(sizeof(char) * (strlen($1) + strlen($3) + 3));
        strcpy(s, $1);
        strcat(s, ".");
        strcat(s, $3);
        //find the structure attribute
        pt = getsym(s);
        if (pt == 0) //semantic check
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", s);
        }
        //pass to reduced element
        $<ExprType.address>$ = getsym(s);
        $<ExprType.type>$ = Integer;
        $<ExprType.isLeft>$ = 1;
        $<ExprType.isInt>$ = 1;
    }
    | INT
    {
        symrec *tp;
        tp = putconst($<intval>1);
        $<ExprType.address>$ = tp;
        $<ExprType.type>$ = Constant;
        $<ExprType.isLeft>$ = 0;
        $<ExprType.isInt>$ = 1;
    }
;

m : /*empty*/
    {
        $<instr>$ = get_nextinst();
    }
;

arrs : /*empty*/
    {    
        $<ExprType.address>$ = 0;
    }    
    | LB exp RB
    {
        tempCnt ++;        
        symrec *tp;
        //t holds the offset of the array
        tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);        
        $<ExprType.address>$ = tp;
        tp = putconst(4);
        quad_push("*", $<ExprType.address>2, tp, $<ExprType.address>$, NULL);
    }
    | LB exp RB LB exp RB
    {// almost the same as previous one
        tempCnt ++;        
        symrec *tp;

        tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);

        $<ExprType.address>$ = tp;
        symrec *pt;
        tp = getsym($<id>0);
        if (tp == 0)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", $<id>0);
        }
        else if (tp->type != Array)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s does not have type Array!\n", $<id>0);
        }
        tp = putconst(tp->col);        
        //offset =exp1*col+exp2
        quad_push("*", $<ExprType.address>2, tp, $<ExprType.address>$, NULL);        
        quad_push("+", $<ExprType.address>$, $<ExprType.address>5, $<ExprType.address>$, NULL);
        tp = putconst(4);
        quad_push("*", $<ExprType.address>$, tp, $<ExprType.address>$, NULL);
    }
;

args : args COMMA exp
        {
            //increase length
            $<ArgExpr.len>$ = $<ArgExpr.len>1 + 1;
            //add exp.address to args.array
            for (int i = 0; i < $<ArgExpr.len>1; i++)
            {
                $<ArgExpr.array>$[i] = $<ArgExpr.array>1[i];
            }
            $<ArgExpr.array>$[$<ArgExpr.len>$ - 1] = $<ExprType.address>3;
        }
    | exp
    {
        //one element
        $<ArgExpr.len>$ = 1;
        $<ArgExpr.array>$[$<ArgExpr.len>$ - 1] = $<ExprType.address>1;
    }
;

func : 
    ID LP 
    {
        if (jumpmain == NULL)//record the palce of main function
        {   
            //jump main should be the first quadruple
            jumpmain = quad_push("jump", NULL, NULL, NULL, NULL);
            jumpmain->target = 0;
        }        
        sym_table = install($1, 0, Function, NULL, 0, 0, 0);         
        symrec * tempt = sym_table;
        //store symbol table and offset
        ST_Stack_Push(&StStack, sym_table);
        sym_table = 0;        
        //generate label to represent the function
        Quad * pt = quad_push("label", NULL, NULL, tempt, NULL);
        if (strcmp($<id>1, "main") == 0)//if the current  function is main, update jump main
        {            
            jumpmain->target = pt;
            tempt->isCalled = 1;
        }
        INT_Stack_Push(&OffsetStack, offset);
        offset = 0;

    }
    paras RP
    {
        symrec *pt = getsym($1);
        //record parameter number 
        pt->paranum = $4;
        //pass attributes to func
        $<STypeVal.structST>$ = sym_table;
        $<STypeVal.type>$ = Function;
        $<STypeVal.width>$ = offset;
        $<STypeVal.lexeme>$ = $1;        
    }
;

paras : /*empty*/
    {
    }
    | paras COMMA TYPE ID
    {
        install($4, offset, Integer, NULL, 0, 0, 4);
        offset += 4;   
        $$ += 1;
    }
    | TYPE ID
    {
        install($2, offset, Integer, NULL, 0, 0, 4);
        offset += 4;
        $$ = 1;
    }
;
/*==========================================================    
                    STATEMENTS
==========================================================*/                    
//statement implementation is almost the same as the Dragon Book
//
stmtblock : 
    LC defs stmts RC
    {
        $<ListExpr.nextlist>$ = $<ListExpr.nextlist>3;
        $<ListExpr.skiplist>$ = $<ListExpr.skiplist>3;
        $<ListExpr.breaklist>$ = $<ListExpr.breaklist>3;
    }
;

stmts : /*empty*/
    {        
        $<ListExpr.nextlist>$ = NULL;
        $<ListExpr.skiplist>$ = NULL;
        $<ListExpr.breaklist>$ = NULL;
    }
    | stmt m stmts
    {
        backpatch($<ListExpr.nextlist>1, $<instr>2);
        $<ListExpr.nextlist>$ = $<ListExpr.nextlist>3;
        $<ListExpr.breaklist>$ = merge($<ListExpr.breaklist>1, $<ListExpr.breaklist>3);
        $<ListExpr.skiplist>$ = merge($<ListExpr.skiplist>1, $<ListExpr.skiplist>3);        
    }
;

n : /*empty*/
    {
        $<ListExpr.nextlist>$ = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
    }
;

stmt : 
    exp SEMI
    {        
        $<ListExpr.nextlist>$ = NULL;
        $<ListExpr.skiplist>$ = NULL;
        $<ListExpr.breaklist>$ = NULL;
    }
    | stmtblock
    {
        $<ListExpr.nextlist>$ = $<ListExpr.nextlist>1;
        $<ListExpr.skiplist>$ = $<ListExpr.skiplist>1;
        $<ListExpr.breaklist>$ = $<ListExpr.breaklist>1;

    }
    | RETURN exp SEMI
    {
        quad_push("return", $<ExprType.address>2, NULL, NULL, NULL);        
        $<ListExpr.nextlist>$ = NULL;
        $<ListExpr.skiplist>$ = NULL;
        $<ListExpr.breaklist>$ = NULL;
    }
    | IF LP exp RP m stmt
    {
        if ($<ExprType.isInt>3 == 0)
        {
            errors++;
            fprintf(stderr, "expression not int type.\n");
        }
        backpatch($<ExprType.truelist>3, $<instr>5);
        $<ListExpr.nextlist>$ = merge($<ExprType.falselist>3, $<ListExpr.nextlist>6);
        $<ListExpr.skiplist>$ = $<ListExpr.skiplist>6;
        $<ListExpr.breaklist>$ = $<ListExpr.breaklist>6;
    }
    | IF LP exp RP m stmt n ELSE m stmt
    {        
        backpatch($<ExprType.truelist>3, $<instr>5);
        backpatch($<ExprType.falselist>3, $<instr>9);        
        list *temp1 = merge($<ListExpr.nextlist>6, $<ListExpr.nextlist>7);
        //fprintf(stderr, "here!\n");
        $<ListExpr.nextlist>$ = merge(temp1, $<ListExpr.nextlist>10);
        $<ListExpr.skiplist>$ = merge($<ListExpr.skiplist>6, $<ListExpr.skiplist>10);
        $<ListExpr.breaklist>$ = merge($<ListExpr.breaklist>6, $<ListExpr.breaklist>10);
    }
    | FOR LP exp SEMI m exp SEMI m exp n RP m {inloop += 1;} stmt
    {
        if ($<ExprType.isInt>6 == 0)
        {
            errors++;
            fprintf(stderr, "Expression not int type.\n");
        }
        inloop -= 1;        
        backpatch($<ExprType.truelist>6, $<instr>12);
        $<ListExpr.nextlist>$ = merge($<ExprType.falselist>6, $<ListExpr.breaklist>14);
        quad_push("jump", NULL, NULL, NULL, $<instr>8);
        backpatch($<ListExpr.nextlist>10, $<instr>5);
        backpatch($<ListExpr.nextlist>14, $<instr>8);
        backpatch($<ListExpr.skiplist>14, $<instr>8);        
        $<ListExpr.breaklist>$ = NULL;
    }
    | CONT SEMI
    {
        if (inloop == 0)
        {
            errors++;
            fprintf(stderr, "Continue not in loop.\n");
        }
        $<ListExpr.skiplist>$ = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        $<ListExpr.nextlist>$ = NULL;
        $<ListExpr.breaklist>$ = NULL;
    }
    | BREAK SEMI
    {        
        if (inloop == 0)
        {
            errors++;
            fprintf(stderr, "Break not in loop.\n");
        }
        $<ListExpr.breaklist>$ = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        $<ListExpr.nextlist>$ = NULL;
        $<ListExpr.skiplist>$ = NULL;
    }
;

%%
/*=========================================================================
                                  MAIN
=========================================================================*/
main( int argc, char *argv[] )
{ 
    extern FILE *yyin;
    ++argv; --argc;
    yyin = fopen( argv[0], "r" );

    /*yydebug = 1;*/
    errors = 0;
    yyparse ();
    irfp=fopen("InterCode","w");
    outfp=fopen(argv[1],"w");
    printf ( "Parse Completed\n" );
    if (jumpmain->target == 0)
    {
        errors++;
        fprintf(stderr, "Main function not defined.\n");
    }    
    if (errors > 0)
    {
        fprintf(stderr, "%d Errors, please check!\n", errors);
        fprintf(outfp, "Error.\n");
    }
    else
    {
        /*
        symrec *pt;
                for (pt = sym_table; pt != 0; pt = pt->next)
        {
            if (pt->strField != 0) 
            {
                fprintf(stderr, "=======sym_table_for_function=======\n");        
                printST(pt->strField);
            }
        }        */
        markblock(codeHead);
        //printIR();
        //OPTIMIZATION
        eliminateJump(codeHead);
        jumpmain->target->dest->isCalled = 1;        
        ConstProp(codeHead);
        //fprintf(stderr, "------start eliminate assign--------");
        AssignEliminate(codeHead);
        printIR();
        gen_code(codeHead);
        //printST(sym_table);
        
        /*for (pt = sym_table; pt != 0; pt = pt->next)
        {
            if (pt->strField != 0) 
            {
                fprintf(stderr, "=======sym_table_for_function=======\n");        
                printST(pt->strField);
            }
        }*/
    }
    fclose(yyin);
    fclose(outfp);
    fclose(irfp);
}

yyerror ( char *s )  /* Called by yyparse on error */
{
    errors++;
    printf ("%s\n", s);
}

