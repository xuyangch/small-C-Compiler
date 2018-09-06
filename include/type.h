/***************************************************************************
                              TYPE DEFINITION
              
This file defines all the unique type in this project
***************************************************************************/
//type for symbol record
enum EType {Integer = 1, Array, Struct, StructID, Temp, Constant, Function};
typedef enum EType Type;

//type for register
enum RType {V = 1, T, S, A, GP, SP, FP, Z, O, RA};
typedef enum RType RType;

struct symrec  //symbol record
{
  char *name;  //name of the symbol
  int offset;  //memory offset of the symbol
  struct symrec *next; //it is a linked list
  Type type;  //type of the symbol
  struct symrec *strField;  //the name space for this symbol, also a symbol table (only for function and structure)
  int tempCnt;  //the id of each temporal variable
  int ConstVal;  //the constant value if it is a constant or it holds constant
  int row;  //the maximum row number (only for array)
  int col;  //the maximum column number (only for array)
  int width;  //the width of this symbol (4 for int)
  int paranum;  //how many parameter the symbol has (only for function)  
  int *arrayInit;  //initial value for array, stored in an array
  int isGlobal;  //indocate whether this is a global variable
  int isCalled;  //indocate whether this is called (only for function)
};

typedef struct symrec symrec;

struct qnode  //quadruple
{
	char *Op;  //the operation of this quadruple
	symrec *arg1;  //the symbol record for the first argument
	symrec *arg2;  //the symbol record for the second argument
	symrec *dest;  //the symbol record for the destination argument
	struct qnode *target;  //the symbol record for the target quadruple (only for jump or branch)
	struct qnode *next;  //next quadruple, it is a linked list
	int id;  //the unique id of a quadruple
	int isBlock;  //indicate whether this quadruple is the head of a basic block
  int generateLabel;  //indicate whether we need to generate a MIPS code for label 
  int need; //indicate whether this quadruple is needed for assign operation
}; 

typedef struct qnode Quad;


struct lnode
{
    Quad * rec;
    struct lnode *next;
};

typedef struct lnode list;

FILE *outfp; //output file for MIPS code
FILE *irfp; //output file for IR code

int errors; /* Error Count                                 */
