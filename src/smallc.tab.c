/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "smallc.y" /* yacc.c:339  */
/*************************************************************************
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

#line 124 "smallc.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "smallc.tab.h".  */
#ifndef YY_YY_SMALLC_TAB_H_INCLUDED
# define YY_YY_SMALLC_TAB_H_INCLUDED
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
#line 58 "smallc.y" /* yacc.c:355  */
union semrec
{
#line 59 "smallc.y" /* yacc.c:355  */

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

#line 268 "smallc.tab.c" /* yacc.c:355  */
};
#line 58 "smallc.y" /* yacc.c:355  */
typedef union semrec YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SMALLC_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 285 "smallc.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   401

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  103
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  216

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   311

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   147,   147,   147,   158,   161,   168,   172,   177,   191,
     194,   231,   267,   270,   285,   313,   328,   356,   366,   376,
     389,   388,   407,   406,   422,   439,   440,   441,   445,   448,
     456,   501,   550,   553,   556,   586,   620,   623,   637,   642,
     654,   677,   693,   710,   724,   738,   752,   766,   780,   794,
     808,   822,   839,   857,   886,   911,   936,   962,   988,  1014,
    1040,  1066,  1097,  1106,  1115,  1122,  1131,  1140,  1149,  1158,
    1167,  1179,  1199,  1217,  1229,  1242,  1250,  1284,  1322,  1357,
    1369,  1375,  1378,  1388,  1419,  1430,  1440,  1439,  1477,  1479,
    1485,  1498,  1507,  1512,  1522,  1529,  1535,  1542,  1549,  1561,
    1571,  1571,  1587,  1599
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEMI", "COMMA", "LC", "RC", "STRUCT",
  "RETURN", "IF", "BREAK", "CONT", "FOR", "TYPE", "ID", "READ", "WRITE",
  "INT", "ASSIGNOP", "ADDASSIGN", "SUBASSIGN", "MULASSIGN", "DIVASSIGN",
  "ANDASSIGN", "XORASSIGN", "ORASSIGN", "LSASSIGN", "RSASSIGN", "LOR",
  "LAND", "BOR", "BXOR", "BAND", "EQ", "NEQ", "GT", "LT", "NGT", "NLT",
  "SL", "SR", "ADD", "SUB", "MUL", "DIV", "MOD", "LNOT", "PIN", "PDE",
  "UMIN", "BNOT", "DOT", "LP", "RP", "LB", "RB", "ELSE", "$accept",
  "program", "$@1", "extdefs", "extdef", "sextvars", "extvars", "var",
  "stspec", "$@2", "$@3", "defs", "sdefs", "sdecs", "decs", "exp", "init",
  "exps", "m", "arrs", "args", "func", "$@4", "paras", "stmtblock",
  "stmts", "n", "stmt", "$@5", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311
};
# endif

#define YYPACT_NINF -186

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-186)))

#define YYTABLE_NINF -95

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -186,    15,    11,  -186,     2,    16,  -186,    11,    25,  -186,
      48,   -44,    76,    63,    85,  -186,  -186,    99,    84,  -186,
    -186,    82,  -186,    94,    77,    19,  -186,  -186,   102,   104,
     109,    84,   107,    62,    72,   112,   147,    44,    79,    80,
    -186,   147,   147,   147,   147,   147,   147,  -186,  -186,   270,
      94,   104,   140,  -186,  -186,   101,  -186,   127,   120,     1,
      83,    77,  -186,    21,   124,   147,   147,  -186,   125,   147,
    -186,  -186,  -186,  -186,  -186,   198,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,  -186,  -186,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   129,   103,   110,   147,    90,   150,
     155,    91,   156,  -186,   154,  -186,    84,   151,  -186,  -186,
     153,  -186,   164,  -186,   147,  -186,  -186,    18,   130,   114,
     234,  -186,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,   147,   147,   318,   332,   345,   356,   356,    33,
      33,    33,    33,    43,    43,   -10,   -10,  -186,  -186,  -186,
      77,    19,    94,    19,   181,   147,  -186,  -186,   147,  -186,
    -186,   140,  -186,  -186,   177,   141,  -186,  -186,   144,  -186,
    -186,   287,   303,  -186,  -186,   182,  -186,  -186,   148,   199,
    -186,  -186,  -186,   147,    77,  -186,  -186,   149,  -186,   163,
     147,  -186,   152,   200,   158,  -186,  -186,   147,   163,  -186,
    -186,   159,  -186,  -186,   163,  -186
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     4,     1,     0,    12,     3,     4,     9,    22,
      24,    17,     0,    13,     0,     5,    10,     0,    28,    20,
      86,     0,     6,     0,    36,    25,     8,     7,     0,     0,
       0,    28,    88,     0,    17,    15,    36,    81,     0,     0,
      79,    36,     0,    36,    36,    36,     0,    38,    14,    37,
       0,     0,    36,    11,    31,     0,    23,     0,     0,     0,
      18,    36,    85,     0,     0,    36,    36,    77,     0,     0,
      71,    64,    73,    74,    72,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    80,    80,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,     0,     0,    36,     0,     0,
       0,     0,     0,    96,     0,    80,    28,     0,    21,    90,
       0,    87,     0,    16,    36,    39,    78,     0,     0,     0,
       0,    75,    52,    53,    54,    55,    56,    57,    59,    58,
      60,    61,     0,     0,    50,    51,    49,    69,    70,    65,
      67,    68,    66,    47,    48,    42,    43,    44,    45,    46,
      36,    25,     0,    25,     0,    36,   103,   102,    36,    95,
      91,    36,    29,    30,     0,     0,    84,    76,    82,    40,
      41,    62,    63,    35,    26,    33,    27,    97,     0,     0,
      93,    89,    19,    36,    36,    80,    80,     0,    34,    36,
      36,    83,    98,     0,     0,    80,    80,    36,    36,    94,
      99,     0,    80,   100,    36,   101
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -186,  -186,  -186,   237,  -186,  -186,  -186,   -22,     4,  -186,
    -186,  -123,   -27,   194,  -186,   -24,   -59,   -33,   -84,  -186,
     142,  -186,  -186,  -186,   192,    75,    38,  -185,  -186
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     6,     7,    17,    12,    13,    51,    31,
      18,    52,    30,    55,   105,   112,    48,    49,   142,    67,
      63,    14,    32,    59,   113,   114,   204,   115,   214
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      47,    35,   123,   143,    57,   120,     8,     9,    20,    71,
      21,     8,    62,    75,   202,     3,    10,    70,     4,    72,
      73,    74,   124,   210,     5,   124,     4,   125,   104,   215,
      11,   171,    50,   101,   102,   103,   130,    47,   184,    16,
     186,    62,   128,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    19,   121,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   177,    97,    98,    99,   100,   101,   102,   103,    22,
      23,    24,    36,   164,    99,   100,   101,   102,   103,   172,
      25,    37,    38,    39,    40,    64,    65,    29,    66,    33,
     176,   183,    27,    28,   116,   117,   161,   162,    34,   181,
     182,   199,   200,   163,   117,    56,    53,    60,    54,    41,
      58,   207,   208,    42,    43,    44,    21,    45,   213,    46,
      61,    68,    69,   118,   119,   198,    47,   122,   126,   129,
     185,   188,   165,   168,   189,    25,   -92,   160,   107,   108,
     109,   110,   111,   166,    37,    38,    39,    40,   167,   169,
     170,    37,    38,    39,    40,   173,   174,   179,    25,   197,
      47,   107,   108,   109,   110,   111,   203,    37,    38,    39,
      40,   175,    41,   209,   187,   178,    42,    43,    44,    41,
      45,   191,    46,    42,    43,    44,   192,    45,   193,    46,
     194,   195,   196,   205,   201,    41,    26,   127,   -94,    42,
      43,    44,   212,    45,   206,    46,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    15,   106,   190,   211,     0,     0,
       0,   131,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
       0,     0,     0,     0,     0,     0,     0,   180,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103
};

static const yytype_int16 yycheck[] =
{
      24,    23,    61,    87,    31,     4,     2,     5,    52,    42,
      54,     7,    36,    46,   199,     0,    14,    41,     7,    43,
      44,    45,     4,   208,    13,     4,     7,     6,    50,   214,
      14,   115,    13,    43,    44,    45,    69,    61,   161,    14,
     163,    65,    66,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,     5,    53,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,    53,    39,    40,    41,    42,    43,    44,    45,     3,
       4,    18,     5,   107,    41,    42,    43,    44,    45,   116,
       5,    14,    15,    16,    17,    51,    52,    13,    54,    17,
     124,   160,     3,     4,     3,     4,     3,     4,    14,   142,
     143,   195,   196,     3,     4,     6,    14,    55,    14,    42,
      13,   205,   206,    46,    47,    48,    54,    50,   212,    52,
      18,    52,    52,     6,    14,   194,   160,    54,    14,    14,
     162,   165,    52,    52,   168,     5,     6,    18,     8,     9,
      10,    11,    12,     3,    14,    15,    16,    17,     3,     3,
       6,    14,    15,    16,    17,    14,    13,    53,     5,   193,
     194,     8,     9,    10,    11,    12,   200,    14,    15,    16,
      17,    17,    42,   207,     3,    55,    46,    47,    48,    42,
      50,    14,    52,    46,    47,    48,    55,    50,    54,    52,
      18,    53,     3,     3,    55,    42,    14,    65,    56,    46,
      47,    48,    53,    50,    56,    52,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,     7,    51,   171,   209,    -1,    -1,
      -1,    53,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    58,    59,     0,     7,    13,    60,    61,    65,     5,
      14,    14,    63,    64,    78,    60,    14,    62,    67,     5,
      52,    54,     3,     4,    18,     5,    81,     3,     4,    13,
      69,    66,    79,    17,    14,    64,     5,    14,    15,    16,
      17,    42,    46,    47,    48,    50,    52,    72,    73,    74,
      13,    65,    68,    14,    14,    70,     6,    69,    13,    80,
      55,    18,    72,    77,    51,    52,    54,    76,    52,    52,
      72,    74,    72,    72,    72,    74,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    64,    71,    70,     8,     9,    10,
      11,    12,    72,    81,    82,    84,     3,     4,     6,    14,
       4,    53,    54,    73,     4,     6,    14,    77,    72,    14,
      74,    53,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    75,    75,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      18,     3,     4,     3,    72,    52,     3,     3,    52,     3,
       6,    75,    69,    14,    13,    17,    72,    53,    55,    53,
      53,    74,    74,    73,    68,    64,    68,     3,    72,    72,
      82,    14,    55,    54,    18,    53,     3,    72,    73,    75,
      75,    55,    84,    72,    83,     3,    56,    75,    75,    72,
      84,    83,    53,    75,    85,    84
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    57,    59,    58,    60,    60,    61,    61,    61,    62,
      62,    62,    63,    63,    63,    63,    63,    64,    64,    64,
      66,    65,    67,    65,    65,    68,    68,    68,    69,    69,
      70,    70,    71,    71,    71,    71,    72,    72,    73,    73,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      75,    76,    76,    76,    77,    77,    79,    78,    80,    80,
      80,    81,    82,    82,    83,    84,    84,    84,    84,    84,
      85,    84,    84,    84
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     2,     3,     3,     3,     0,
       1,     3,     0,     1,     3,     3,     5,     1,     4,     7,
       0,     6,     0,     5,     2,     0,     4,     4,     0,     4,
       3,     1,     1,     3,     5,     3,     0,     1,     1,     3,
       4,     4,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     2,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     3,     4,     2,     3,     1,
       0,     0,     3,     6,     3,     1,     0,     5,     0,     4,
       2,     4,     0,     3,     0,     2,     1,     3,     6,    10,
       0,    14,     2,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 147 "smallc.y" /* yacc.c:1661  */
    {
        offset = 0; 
        inloop = 0;
        ST_Stack_Init(&StStack); INT_Stack_Init(&OffsetStack); 
        codeHead = malloc(sizeof(Quad)); codeHead->next = NULL; 
        putsym("read", 0, Function, NULL, 0, 0, 0, 0, 0); 
        putsym("write", 0, Function, NULL, 0, 0, 0, 0, 0); 
    }
#line 1554 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 4:
#line 158 "smallc.y" /* yacc.c:1661  */
    {
        
    }
#line 1562 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 5:
#line 162 "smallc.y" /* yacc.c:1661  */
    {
        
    }
#line 1570 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 6:
#line 169 "smallc.y" /* yacc.c:1661  */
    {

    }
#line 1578 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 7:
#line 173 "smallc.y" /* yacc.c:1661  */
    {
        
    }
#line 1586 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 8:
#line 178 "smallc.y" /* yacc.c:1661  */
    {
        symrec * pt;
        symrec * temp = sym_table;
        pt = getsym((yyvsp[-1].STypeVal.lexeme));
        pt->strField = temp;    //set the unique symbol table for function
        sym_table = ST_Stack_Pop(&StStack); //restore symbol table
        pt = getsym((yyvsp[-1].STypeVal.lexeme));
        pt->strField = temp;  //set the unique symbol table for function        
        offset = INT_Stack_Pop(&OffsetStack);   //restore offset
    }
#line 1601 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 9:
#line 191 "smallc.y" /* yacc.c:1661  */
    {
        
    }
#line 1609 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 10:
#line 195 "smallc.y" /* yacc.c:1661  */
    {
        if (getsym((yyvsp[0].id)) != 0)    //ID not defined
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s Redeclaration!\n", (yyvsp[0].id));
        }
        //sextvars should also holds information for this structure
        (yyval.STypeVal.type) = (yyvsp[-1].STypeVal.type);
        (yyval.STypeVal.width) = (yyvsp[-1].STypeVal.width);
        (yyval.STypeVal.lexeme) = (yyvsp[0].id);
        (yyval.STypeVal.structST) = (yyvsp[-1].STypeVal.structST);
        //install ID
        sym_table = install((yyval.STypeVal.lexeme), offset, Struct, (yyval.STypeVal.structST), 0, 0, (yyval.STypeVal.width));

        //install all of ID' s attributes    
        symrec * ori = sym_table;
        symrec * str = (yyval.STypeVal.structST);
        char * s1;
        char *s2, *s;
        for (int i = 0; i < (yyval.STypeVal.width); i = i + 4)
        {
            sym_table = str;
            symrec *pt = getsym_by_offset(i);
            s2 = pt->name;
            s = malloc(sizeof(char) * (strlen((yyvsp[0].id)) + strlen(s2) + 3));
            strcpy(s, (yyvsp[0].id));
            strcat(s, ".");
            strcat(s, s2);
            sym_table = ori;            
            ori = install(s, offset + i, Integer, NULL, 0, 0, 4);
        }
        sym_table = ori;
        //move the offset ahead
        offset += (yyval.STypeVal.width);
    }
#line 1650 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 11:
#line 232 "smallc.y" /* yacc.c:1661  */
    {
        if (getsym((yyvsp[0].id)) != 0)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable Redeclaration!\n");
        }
        (yyval.STypeVal.type) = (yyvsp[-2].STypeVal.type);
        (yyval.STypeVal.width) = (yyvsp[-2].STypeVal.width);
        (yyval.STypeVal.lexeme) = (yyvsp[0].id);
        (yyval.STypeVal.structST) = (yyvsp[-2].STypeVal.structST);
        sym_table = install((yyval.STypeVal.lexeme), offset, Struct, (yyval.STypeVal.structST), 0, 0, (yyval.STypeVal.width));

        symrec * ori = sym_table;
        symrec * str = (yyval.STypeVal.structST);
        char * s1;
        char *s2;
        for (int i = 0; i < (yyval.STypeVal.width); i = i + 4)
        {
            char *s;
            sym_table = str;
            symrec *pt = getsym_by_offset(i);
            s2 = pt->name;
            s = malloc(sizeof(char) * (strlen((yyvsp[0].id)) + strlen(s2) + 3));
            strcpy(s, (yyvsp[0].id));
            strcat(s, ".");
            strcat(s, s2);
            sym_table = ori;            
            ori = install(s, offset + i, Integer, NULL, 0, 0, 4);
        }
        sym_table = ori;
        offset += (yyvsp[-2].STypeVal.width);
    }
#line 1688 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 12:
#line 267 "smallc.y" /* yacc.c:1661  */
    {
        
    }
#line 1696 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 13:
#line 271 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[0].STypeVal.type) == Integer)
        {
            //quad_push("=", $<ExprType.address>3, NULL, getsym($<STypeVal.lexeme>1), NULL);
            getsym((yyvsp[0].STypeVal.lexeme))->ConstVal = 0;
        }   
        else if ((yyvsp[0].STypeVal.type) == Array)    //set initial value to zero
        {
            symrec *ptr = getsym((yyvsp[0].STypeVal.lexeme));
            int length = ptr->row * ptr->col;
            ptr->arrayInit = (int *) malloc ((length+1) * sizeof(int));
            for (int i = 0; i < length; i++) ptr->arrayInit[i] = 0;
        }
    }
#line 1715 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 14:
#line 286 "smallc.y" /* yacc.c:1661  */
    {
        if (((yyvsp[-2].STypeVal.type) == Integer) && ((yyvsp[0].ArgExpr.len) == 0))
        {
            //ConstVal is used to hold initial value
            getsym((yyvsp[-2].STypeVal.lexeme))->ConstVal = (yyvsp[0].ExprType.address)->ConstVal;  
        }
        else if (((yyvsp[-2].STypeVal.type) == Array) && ((yyvsp[0].ArgExpr.len) > 0)) //set initial value
        {
            symrec *ptr = getsym((yyvsp[-2].STypeVal.lexeme)), *pt, *zero;
            zero = putconst(0);
            int length = ptr->row * ptr->col;
            int base = ptr->offset;
            ptr->arrayInit = (int *) malloc ((length+1) * sizeof(int));
            for (int i = 0; i < length; i++)
            {
                //pt = putconst(4*i);
                if (i < (yyvsp[0].ArgExpr.len))
                {
                    //init has attributes holding initial value
                    ptr->arrayInit[i] = (yyvsp[0].ArgExpr.array)[i]->ConstVal;     
                    //quad_push("[]=", ptr, pt, $<ArgExpr.array>3[i], NULL);
                }
                else ptr->arrayInit[i] = 0;
                //quad_push("[]=", ptr, pt, zero, NULL);
            }
        }
    }
#line 1747 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 15:
#line 314 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[0].STypeVal.type) == Integer)
        {
            //quad_push("=", $<ExprType.address>3, NULL, getsym($<STypeVal.lexeme>1), NULL);
            getsym((yyvsp[0].STypeVal.lexeme))->ConstVal = 0;
        }   
        else if ((yyvsp[0].STypeVal.type) == Array)
        {
            symrec *ptr = getsym((yyvsp[0].STypeVal.lexeme));
            int length = ptr->row * ptr->col;
            ptr->arrayInit = (int *) malloc ((length+1) * sizeof(int));
            for (int i = 0; i < length; i++) ptr->arrayInit[i] = 0;
        }
    }
#line 1766 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 16:
#line 329 "smallc.y" /* yacc.c:1661  */
    {
        if (((yyvsp[-2].STypeVal.type) == Integer) && ((yyvsp[0].ArgExpr.len) == 0))
        {
            //quad_push("=", $<ExprType.address>5, NULL, getsym($<STypeVal.lexeme>3), NULL);
            getsym((yyvsp[-2].STypeVal.lexeme))->ConstVal = (yyvsp[0].ExprType.address)->ConstVal;
        }
        else if (((yyvsp[-2].STypeVal.type) == Array) && ((yyvsp[0].ArgExpr.len) > 0))
        {
            symrec *ptr = getsym((yyvsp[-2].STypeVal.lexeme)), *pt, *zero;
            zero = putconst(0);
            int length = ptr->row * ptr->col;
            int base = ptr->offset;
            ptr->arrayInit = (int *) malloc ((length+1) * sizeof(int));
            for (int i = 0; i < length; i++)
            {
                //pt = putconst(4*i);
                if (i < (yyvsp[0].ArgExpr.len))
                {
                    //quad_push("[]=", ptr, pt, $<ArgExpr.array>5[i], NULL);
                    ptr->arrayInit[i] = (yyvsp[0].ArgExpr.array)[i]->ConstVal;
                }
                else ptr->arrayInit[i] = 0;
                //else quad_push("[]=", ptr, pt, zero, NULL);
            }
        }        
    }
#line 1797 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 17:
#line 357 "smallc.y" /* yacc.c:1661  */
    {
        //set information
        (yyval.STypeVal.type) = Integer;
        (yyval.STypeVal.width) = 4;
        (yyval.STypeVal.lexeme) = (yyvsp[0].id);
        //install ID
        sym_table = install((yyval.STypeVal.lexeme), offset, Integer, NULL, 0, 0, 4);
        offset += (yyval.STypeVal.width);
    }
#line 1811 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 18:
#line 367 "smallc.y" /* yacc.c:1661  */
    {    
        //set information
        (yyval.STypeVal.type) = Array;
        (yyval.STypeVal.width) = (yyvsp[-1].intval) * 4;
        (yyval.STypeVal.lexeme) = (yyvsp[-3].id);
        //install ID
        sym_table = install((yyvsp[-3].id), offset, Array, NULL, 1, (yyvsp[-1].intval), (yyval.STypeVal.width));
        offset += (yyval.STypeVal.width);
    }
#line 1825 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 19:
#line 377 "smallc.y" /* yacc.c:1661  */
    {
        //the same
        (yyval.STypeVal.type) = Array;
        (yyval.STypeVal.width) = (yyvsp[-4].intval) * 4 * (yyvsp[-1].intval);
        (yyval.STypeVal.lexeme) = (yyvsp[-6].id);
        sym_table = install((yyvsp[-6].id), offset, Array, NULL, (yyvsp[-4].intval), (yyvsp[-1].intval), (yyval.STypeVal.width));
        offset += (yyval.STypeVal.width);   
    }
#line 1838 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 20:
#line 389 "smallc.y" /* yacc.c:1661  */
    {
        ST_Stack_Push(&StStack, sym_table);//store symbol table        
        sym_table = (symrec *)0;
        INT_Stack_Push(&OffsetStack, offset);//store offset
        offset = 0;
    }
#line 1849 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 21:
#line 396 "smallc.y" /* yacc.c:1661  */
    {
        //set attributes of stspec
        (yyval.STypeVal.structST) = sym_table;
        (yyval.STypeVal.type) = Struct;
        (yyval.STypeVal.width) = offset;
        //restore environment
        sym_table = ST_Stack_Pop(&StStack);
        offset = INT_Stack_Pop(&OffsetStack);
        sym_table = install((yyvsp[-4].id), 0, StructID, (yyval.STypeVal.structST), 0, 0, (yyval.STypeVal.width));                
    }
#line 1864 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 22:
#line 407 "smallc.y" /* yacc.c:1661  */
    {
        ST_Stack_Push(&StStack, sym_table);
        //sym_table = (symrec *) malloc (sizeof(symrec));
        sym_table = (symrec *)0;
        INT_Stack_Push(&OffsetStack, offset);        
        offset = 0;
    }
#line 1876 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 23:
#line 415 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.STypeVal.structST) = sym_table;
        (yyval.STypeVal.type) = Struct;
        (yyval.STypeVal.width) = offset;
        sym_table = ST_Stack_Pop(&StStack);    
        offset = INT_Stack_Pop(&OffsetStack);
    }
#line 1888 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 24:
#line 423 "smallc.y" /* yacc.c:1661  */
    {
        //search the symbol table
        symrec *pt = getsym((yyvsp[0].id));        
        if (pt == NULL)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", (yyvsp[0].id));
        }
        //set the attribute of stspec                    
        (yyval.STypeVal.structST) = pt->strField;        
        (yyval.STypeVal.type) = Struct;
        (yyval.STypeVal.width) = pt->width;
    }
#line 1907 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 28:
#line 445 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.STypeVal.width) = 0;        
    }
#line 1915 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 29:
#line 449 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.STypeVal) = (yyvsp[-2].STypeVal);        
        (yyval.STypeVal.width) = (yyvsp[-2].STypeVal.width) + (yyvsp[0].STypeVal.width);
        
    }
#line 1925 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 30:
#line 457 "smallc.y" /* yacc.c:1661  */
    {
            (yyval.STypeVal.type) = (yyvsp[-2].STypeVal.type);
            if ((yyval.STypeVal.type) == Struct)//actually type cannot be structure
            {
                if (getsym((yyvsp[0].id)) != 0)
                {
                    errors++;
                    fprintf(stderr, "Semantic Error!\n");
                    fprintf(stderr, "Variable %s Redeclaration!\n", (yyvsp[0].id));
                }
                (yyval.STypeVal.type) = (yyvsp[-2].STypeVal.type);
                (yyval.STypeVal.width) = (yyvsp[-2].STypeVal.width);
                (yyval.STypeVal.lexeme) = (yyvsp[0].id);
                (yyval.STypeVal.structST) = (yyvsp[-2].STypeVal.structST);
                sym_table = install((yyval.STypeVal.lexeme), offset, Struct, (yyval.STypeVal.structST), 0, 0, (yyval.STypeVal.width));
    
                symrec * ori = sym_table;
                symrec * str = (yyval.STypeVal.structST);
                char * s1;
                char *s2;
                for (int i = 0; i < (yyval.STypeVal.width); i = i + 4)
                {
                    char *s;
                    sym_table = str;
                    symrec *pt = getsym_by_offset(i);
                    s2 = pt->name;
                    s = malloc(sizeof(char) * (strlen((yyvsp[0].id)) + strlen(s2) + 3));
                    strcpy(s, (yyvsp[0].id));
                    strcat(s, ".");
                    strcat(s, s2);
                    sym_table = ori;                    
                    ori = install(s, offset + i, Integer, NULL, 0, 0, 4);
                }
                sym_table = ori;
                offset += (yyvsp[-2].STypeVal.width);
            }
            else
            {//install the symbol and move offset ahead
                (yyval.STypeVal.width) = (yyvsp[-2].STypeVal.width);
                sym_table = install((yyvsp[0].id), offset, (yyvsp[-2].STypeVal.type), NULL, 0, 0, (yyval.STypeVal.width));
                (yyval.STypeVal.lexeme) = (yyvsp[0].id);            
                offset += (yyval.STypeVal.width);
            }
        }
#line 1974 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 31:
#line 502 "smallc.y" /* yacc.c:1661  */
    {//the same as extvars
        (yyval.STypeVal.type) = (yyvsp[-1].STypeVal.type);            
        if ((yyval.STypeVal.type) == Struct)
        {            
            if (getsym((yyvsp[0].id)) != 0)
            {
                errors++;
                fprintf(stderr, "Semantic Error!\n");
                fprintf(stderr, "Variable %s Redeclaration!\n", (yyvsp[0].id));
            }
            (yyval.STypeVal.type) = (yyvsp[-1].STypeVal.type);
            (yyval.STypeVal.width) = (yyvsp[-1].STypeVal.width);
            (yyval.STypeVal.lexeme) = (yyvsp[0].id);
            (yyval.STypeVal.structST) = (yyvsp[-1].STypeVal.structST);
            sym_table = install((yyval.STypeVal.lexeme), offset, Struct, (yyval.STypeVal.structST), 0, 0, (yyval.STypeVal.width));

            symrec * ori = sym_table;
            symrec * str = (yyval.STypeVal.structST);            
            char * s1;
            char *s2, *s;
            for (int i = 0; i < (yyval.STypeVal.width); i = i + 4)
            {                
                sym_table = str;                
                symrec *pt = getsym_by_offset(i);                
                s2 = pt->name;                
                s = malloc(sizeof(char) * (strlen((yyvsp[0].id)) + strlen(s2) + 3));                
                strcpy(s, (yyvsp[0].id));
                strcat(s, ".");                
                strcat(s, s2);
                sym_table = ori;                                
                ori = install(s, offset + i, Integer, NULL, 0, 0, 4);
                ori = sym_table;
            }
            sym_table = ori;
            offset += (yyval.STypeVal.width);            
        }

        else
        {
            (yyval.STypeVal.width) = (yyvsp[-1].STypeVal.width);
            sym_table = install((yyvsp[0].id), offset, Integer, NULL, 0, 0, (yyval.STypeVal.width));
            (yyval.STypeVal.lexeme) = (yyvsp[0].id);        
            offset += (yyval.STypeVal.width);
        }
    }
#line 2024 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 32:
#line 551 "smallc.y" /* yacc.c:1661  */
    {        
    }
#line 2031 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 33:
#line 554 "smallc.y" /* yacc.c:1661  */
    {        
    }
#line 2038 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 34:
#line 557 "smallc.y" /* yacc.c:1661  */
    {
        symrec *pt = getsym((yyvsp[-2].STypeVal.lexeme));
        if (pt == 0)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", (yyvsp[-2].STypeVal.lexeme));
        }
        if (((yyvsp[-2].STypeVal.type) == Integer) && ((yyvsp[0].ArgExpr.len) == 0))
        {            
            quad_push("=", (yyvsp[0].ExprType.address), NULL, getsym((yyvsp[-2].STypeVal.lexeme)), NULL);
        }
        else if (((yyvsp[-2].STypeVal.type) == Array) && ((yyvsp[0].ArgExpr.len) > 0))
        {
            symrec *ptr = getsym((yyvsp[-2].STypeVal.lexeme)), *pt, *zero;
            zero = putconst(0);
            int length = ptr->row * ptr->col;
            int base = ptr->offset;
            for (int i = 0; i < length; i++)
            {
                pt = putconst(4*i);
                if (i < (yyvsp[0].ArgExpr.len))
                {
                    quad_push("[]=", ptr, pt, (yyvsp[0].ArgExpr.array)[i], NULL);
                }
                else quad_push("[]=", ptr, pt, zero, NULL);
            }
        }
    }
#line 2072 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 35:
#line 587 "smallc.y" /* yacc.c:1661  */
    {
        symrec *pt = getsym((yyvsp[-2].STypeVal.lexeme));        
        if (pt == 0)  //if var not defined, error
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", (yyvsp[-2].STypeVal.lexeme));
        }
        if (((yyvsp[-2].STypeVal.type) == Integer) && ((yyvsp[0].ArgExpr.len) == 0)) //integer, derect assign
        {

            quad_push("=", (yyvsp[0].ExprType.address), NULL, getsym((yyvsp[-2].STypeVal.lexeme)), NULL);
        }
        else if (((yyvsp[-2].STypeVal.type) == Array) && ((yyvsp[0].ArgExpr.len) > 0))  //array, use a loop to assign
        {
            symrec *ptr = getsym((yyvsp[-2].STypeVal.lexeme)), *pt, *zero;//get the symbol record for array
            zero = putconst(0);
            int length = ptr->row * ptr->col;
            int base = ptr->offset;
            for (int i = 0; i < length; i++)
            {
                pt = putconst(4*i); //1 integer is 4 bytes
                if (i < (yyvsp[0].ArgExpr.len))
                {
                    quad_push("[]=", ptr, pt, (yyvsp[0].ArgExpr.array)[i], NULL);
                }
                else quad_push("[]=", ptr, pt, zero, NULL);
            }
        }
    }
#line 2107 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 36:
#line 620 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ExprType.address) = 0;        
    }
#line 2115 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 37:
#line 624 "smallc.y" /* yacc.c:1661  */
    {//passing attributes
        (yyval.ExprType.address) = (yyvsp[0].ExprType.address);
        (yyval.ExprType.type) = (yyvsp[0].ExprType.type);
        (yyval.ExprType.array) = (yyvsp[0].ExprType.array);
        (yyval.ExprType.offset) = (yyvsp[0].ExprType.offset);
        (yyval.ExprType.truelist) = (yyvsp[0].ExprType.truelist);
        (yyval.ExprType.falselist) = (yyvsp[0].ExprType.falselist);
        (yyval.ExprType.nextlist) = (yyvsp[0].ExprType.nextlist);        
        (yyval.ExprType.isInt) = (yyvsp[0].ExprType.isInt);        
        (yyval.ExprType.isLeft) = (yyvsp[0].ExprType.isLeft);        
    }
#line 2131 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 38:
#line 638 "smallc.y" /* yacc.c:1661  */
    {        
        (yyval.ArgExpr.address) = (yyvsp[0].ExprType.address);
        (yyval.ArgExpr.len) = 0;//indicate it is init for integer var
    }
#line 2140 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 39:
#line 643 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ArgExpr.address) = 0;
        (yyval.ArgExpr.len) = (yyvsp[-1].ArgExpr.len);
        for (int i = 0; i < (yyvsp[-1].ArgExpr.len); i++)
        {
            (yyval.ArgExpr.array)[i] = (yyvsp[-1].ArgExpr.array)[i];
        }
    }
#line 2153 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 40:
#line 655 "smallc.y" /* yacc.c:1661  */
    {
        symrec *pt = getsym((yyvsp[-1].id));
        if (pt == 0)
        {//whether ID is defined
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", (yyvsp[-1].id));
        }
        else if ((pt->type != Integer) && (pt->type != Constant) && (pt->type != Temp))
        {
            errors++;
            fprintf(stderr, "Function argument type not fit.\n");   
        }
        else //load argument and call function
        {
            quad_push("param", getsym((yyvsp[-1].id)), NULL, NULL, NULL);
            symrec * pt = putconst(1);
            quad_push("call", getsym((yyvsp[-3].id)), pt, getsym((yyvsp[-1].id)), NULL);   
        }
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 0;
    }
#line 2180 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 41:
#line 678 "smallc.y" /* yacc.c:1661  */
    {   
        if ((yyvsp[-1].ExprType.isInt) != 1)
        {
            errors++;
            fprintf(stderr, "Function argument type not fit.\n");
        }        
        quad_push("param", (yyvsp[-1].ExprType.address), NULL, NULL, NULL);
        symrec * pt = putconst(1);
        quad_push("call", getsym((yyvsp[-3].id)), pt, NULL, NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 0;
    }
#line 2197 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 42:
#line 694 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;
        symrec *tp;
        tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);//generate the temporal variable
        (yyval.ExprType.address) = tp;
        //if both are constant, we can calculate the value
        if (((yyvsp[-2].ExprType.type) == Constant) && ((yyvsp[0].ExprType.type) == Constant))
        {
            (yyval.ExprType.type) = Constant;
            (yyval.ExprType.address) = putconst((yyvsp[-2].ExprType.address)->ConstVal + (yyvsp[0].ExprType.address)->ConstVal);
        } else //otherwise generate the quadruple to compute the result
        quad_push("+", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
        
    }
#line 2218 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 43:
#line 711 "smallc.y" /* yacc.c:1661  */
    {//below are almost the same as ADD operation
        tempCnt ++;
        (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (((yyvsp[-2].ExprType.type) == Constant) && ((yyvsp[0].ExprType.type) == Constant))
        {
            (yyval.ExprType.type) = Constant;
            (yyval.ExprType.address) = putconst((yyvsp[-2].ExprType.address)->ConstVal - (yyvsp[0].ExprType.address)->ConstVal);
        } else
        quad_push("-", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
        
    }
#line 2236 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 44:
#line 725 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;
        (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (((yyvsp[-2].ExprType.type) == Constant) && ((yyvsp[0].ExprType.type) == Constant))
        {
            (yyval.ExprType.type) = Constant;
            (yyval.ExprType.address) = putconst((yyvsp[-2].ExprType.address)->ConstVal * (yyvsp[0].ExprType.address)->ConstVal);
        } else
        quad_push("*", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
        
    }
#line 2254 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 45:
#line 739 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;
        (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (((yyvsp[-2].ExprType.type) == Constant) && ((yyvsp[0].ExprType.type) == Constant))
        {
            (yyval.ExprType.type) = Constant;
            (yyval.ExprType.address) = putconst((yyvsp[-2].ExprType.address)->ConstVal / (yyvsp[0].ExprType.address)->ConstVal);
        } else
        quad_push("/", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
        
    }
#line 2272 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 46:
#line 753 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;
        (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (((yyvsp[-2].ExprType.type) == Constant) && ((yyvsp[0].ExprType.type) == Constant))
        {
            (yyval.ExprType.type) = Constant;
            (yyval.ExprType.address) = putconst((yyvsp[-2].ExprType.address)->ConstVal % (yyvsp[0].ExprType.address)->ConstVal);
        } else
        quad_push("%", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
        
    }
#line 2290 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 47:
#line 767 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;
        (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (((yyvsp[-2].ExprType.type) == Constant) && ((yyvsp[0].ExprType.type) == Constant))
        {
            (yyval.ExprType.type) = Constant;
            (yyval.ExprType.address) = putconst((yyvsp[-2].ExprType.address)->ConstVal << (yyvsp[0].ExprType.address)->ConstVal);
        } else
        quad_push("<<", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
        
    }
#line 2308 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 48:
#line 781 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;
        (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (((yyvsp[-2].ExprType.type) == Constant) && ((yyvsp[0].ExprType.type) == Constant))
        {
            (yyval.ExprType.type) = Constant;
            (yyval.ExprType.address) = putconst((yyvsp[-2].ExprType.address)->ConstVal >> (yyvsp[0].ExprType.address)->ConstVal);
        } else
        quad_push(">>", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
        
    }
#line 2326 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 49:
#line 795 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;
        (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (((yyvsp[-2].ExprType.type) == Constant) && ((yyvsp[0].ExprType.type) == Constant))
        {
            (yyval.ExprType.type) = Constant;
            (yyval.ExprType.address) = putconst((yyvsp[-2].ExprType.address)->ConstVal & (yyvsp[0].ExprType.address)->ConstVal);
        } else
        quad_push("&", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
        
    }
#line 2344 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 50:
#line 809 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;
        (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (((yyvsp[-2].ExprType.type) == Constant) && ((yyvsp[0].ExprType.type) == Constant))
        {
            (yyval.ExprType.type) = Constant;
            (yyval.ExprType.address) = putconst((yyvsp[-2].ExprType.address)->ConstVal | (yyvsp[0].ExprType.address)->ConstVal);
        } else
        quad_push("|", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
        
    }
#line 2362 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 51:
#line 823 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;
        (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);;
        if (((yyvsp[-2].ExprType.type) == Constant) && ((yyvsp[0].ExprType.type) == Constant))
        {
            (yyval.ExprType.type) = Constant;
            (yyval.ExprType.address) = putconst((yyvsp[-2].ExprType.address)->ConstVal ^ (yyvsp[0].ExprType.address)->ConstVal);
        } else
        quad_push("^", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
        
    }
#line 2380 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 52:
#line 840 "smallc.y" /* yacc.c:1661  */
    {        
        if ((yyvsp[-2].ExprType.isLeft) == 0)//left value check 
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ((yyvsp[-2].ExprType.type) == Array) 
        {
            symrec *zero = putconst(0);
            //assign value to an array
            quad_push("[]=", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyvsp[0].ExprType.address), NULL);
        }
        else quad_push("=", (yyvsp[0].ExprType.address), NULL, (yyvsp[-2].ExprType.address), NULL);//assign integer value
        (yyval.ExprType.address) = (yyvsp[-2].ExprType.address);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2402 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 53:
#line 858 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[-2].ExprType.isLeft) == 0)// left value check
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ((yyvsp[-2].ExprType.type) == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            //t get the value of left expression
            quad_push("=[]", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
            //do the addition
            quad_push("+", (yyval.ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
            //store t to the left expression
            quad_push("[]=", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
        }
        else 
        {
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            //add left and right, store to left
            quad_push("+", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyvsp[-2].ExprType.address), NULL);
        }
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2435 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 54:
#line 887 "smallc.y" /* yacc.c:1661  */
    {//below are the same as ADDASSIGN
        if ((yyvsp[-2].ExprType.isLeft) == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ((yyvsp[-2].ExprType.type) == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
            quad_push("-", (yyval.ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
            quad_push("[]=", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
        }
        else 
        {
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("-", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyvsp[-2].ExprType.address), NULL);
        }
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2464 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 55:
#line 912 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[-2].ExprType.isLeft) == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ((yyvsp[-2].ExprType.type) == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
            quad_push("*", (yyval.ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
            quad_push("[]=", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
        }
        else 
        {
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("*", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyvsp[-2].ExprType.address), NULL);
        }
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2493 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 56:
#line 937 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[-2].ExprType.isLeft) == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ((yyvsp[-2].ExprType.type) == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
            quad_push("/", (yyval.ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
            quad_push("[]=", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
        }
        else 
        {
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("/", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyvsp[-2].ExprType.address), NULL);
        }

        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2523 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 57:
#line 963 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[-2].ExprType.isLeft) == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ((yyvsp[-2].ExprType.type) == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
            quad_push("&", (yyval.ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
            quad_push("[]=", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
        }
        else 
        {
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("&", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyvsp[-2].ExprType.address), NULL);
        }
        
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2553 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 58:
#line 989 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[-2].ExprType.isLeft) == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ((yyvsp[-2].ExprType.type) == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
            quad_push("|", (yyval.ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
            quad_push("[]=", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
        }
        else 
        {
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("|", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyvsp[-2].ExprType.address), NULL);
        }
        
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2583 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 59:
#line 1015 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[-2].ExprType.isLeft) == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ((yyvsp[-2].ExprType.type) == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
            quad_push("^", (yyval.ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
            quad_push("[]=", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
        }
        else 
        {
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("^", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyvsp[-2].ExprType.address), NULL);
        }
        
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2613 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 60:
#line 1041 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[-2].ExprType.isLeft) == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ((yyvsp[-2].ExprType.type) == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
            quad_push("<<", (yyval.ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
            quad_push("[]=", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
        }
        else 
        {
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("<<", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyvsp[-2].ExprType.address), NULL);
        }
        
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2643 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 61:
#line 1067 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[-2].ExprType.isLeft) == 0)
        {
            errors++;
            fprintf(stderr, "Not left value.\n");
        }
        if ((yyvsp[-2].ExprType.type) == Array) 
        {
            symrec *zero = putconst(0);
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
            quad_push(">>", (yyval.ExprType.address), (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
            quad_push("[]=", (yyvsp[-2].ExprType.array), (yyvsp[-2].ExprType.offset), (yyval.ExprType.address), NULL);
        }
        else 
        {
            tempCnt ++;
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push(">>", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), (yyvsp[-2].ExprType.address), NULL);
        }
        
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2673 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 62:
#line 1098 "smallc.y" /* yacc.c:1661  */
    {
        backpatch((yyvsp[-3].ExprType.falselist), (yyvsp[-1].instr));
        (yyval.ExprType.truelist) = merge((yyvsp[-3].ExprType.truelist), (yyvsp[0].ExprType.truelist));
        (yyval.ExprType.falselist) = (yyvsp[0].ExprType.falselist);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2685 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 63:
#line 1107 "smallc.y" /* yacc.c:1661  */
    {
        backpatch((yyvsp[-3].ExprType.truelist), (yyvsp[-1].instr));
        (yyval.ExprType.truelist) = (yyvsp[0].ExprType.truelist);
        (yyval.ExprType.falselist) = merge((yyvsp[-3].ExprType.falselist), (yyvsp[0].ExprType.falselist));
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2697 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 64:
#line 1116 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ExprType.truelist) = (yyvsp[0].ExprType.falselist);
        (yyval.ExprType.falselist) = (yyvsp[0].ExprType.truelist);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2708 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 65:
#line 1123 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ExprType.truelist) = makelist(get_nextinst());
        quad_push(">", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), NULL, NULL);
        (yyval.ExprType.falselist) = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2721 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 66:
#line 1132 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ExprType.truelist) = makelist(get_nextinst());
        quad_push(">=", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), NULL, NULL);
        (yyval.ExprType.falselist) = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2734 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 67:
#line 1141 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ExprType.truelist) = makelist(get_nextinst());
        quad_push("<", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), NULL, NULL);
        (yyval.ExprType.falselist) = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2747 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 68:
#line 1150 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ExprType.truelist) = makelist(get_nextinst());
        quad_push("<=", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), NULL, NULL);
        (yyval.ExprType.falselist) = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2760 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 69:
#line 1159 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ExprType.truelist) = makelist(get_nextinst());
        quad_push("==", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), NULL, NULL);
        (yyval.ExprType.falselist) = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);   
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2773 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 70:
#line 1168 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ExprType.truelist) = makelist(get_nextinst());
        quad_push("!=", (yyvsp[-2].ExprType.address), (yyvsp[0].ExprType.address), NULL, NULL);
        (yyval.ExprType.falselist) = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2786 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 71:
#line 1180 "smallc.y" /* yacc.c:1661  */
    {
        symrec *zero = putconst(0);
        symrec *tp;
        //if exp is constant, we can just get the negative value of the constant as result
        if ((yyvsp[0].ExprType.address)->type == Constant)
        {
            (yyvsp[0].ExprType.address) = putconst((yyvsp[0].ExprType.address)->ConstVal * -1);
            (yyval.ExprType.address) = (yyvsp[0].ExprType.address);
        }
        else//other wise generate a quadruple to compute the resule
        {
            tempCnt ++;    
            tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            (yyval.ExprType.address) = tp;
            quad_push("-", zero, (yyvsp[0].ExprType.address), (yyval.ExprType.address), NULL);
        }
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2810 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 72:
#line 1200 "smallc.y" /* yacc.c:1661  */
    {        
        if ((yyvsp[0].ExprType.address)->type == Constant)
        {
            (yyvsp[0].ExprType.address) = putconst(~((yyvsp[0].ExprType.address)->ConstVal));
            (yyval.ExprType.address) = (yyvsp[0].ExprType.address);
        }
        else
        {
            tempCnt ++;        
            symrec *tp;
            tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            (yyval.ExprType.address) = tp;
            quad_push("~", (yyvsp[0].ExprType.address), NULL, (yyval.ExprType.address), NULL);
        }
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2832 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 73:
#line 1218 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;        
        symrec *tp;
        tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
        (yyval.ExprType.address) = tp;
        symrec *one = putconst(1);
        quad_push("+", (yyvsp[0].ExprType.address), one, (yyvsp[0].ExprType.address), NULL); //increase exp by 1
        quad_push("=", (yyvsp[0].ExprType.address), NULL, (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2848 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 74:
#line 1230 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;        
        symrec *tp;
        tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
        (yyval.ExprType.address) = tp;
        symrec *one = putconst(1);
        quad_push("-", (yyvsp[0].ExprType.address), one, (yyvsp[0].ExprType.address), NULL); //decrease exp by 1
        quad_push("=", (yyvsp[0].ExprType.address), NULL, (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2864 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 75:
#line 1243 "smallc.y" /* yacc.c:1661  */
    {//just assign all attributes of exps to exp
        (yyval.ExprType.address) = (yyvsp[-1].ExprType.address);
        (yyval.ExprType.truelist) = (yyvsp[-1].ExprType.truelist);
        (yyval.ExprType.falselist) = (yyvsp[-1].ExprType.falselist);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = (yyvsp[-2].ExprType.isInt);
    }
#line 2876 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 76:
#line 1251 "smallc.y" /* yacc.c:1661  */
    {        
        if ((yyvsp[-1].ArgExpr.len) != getsym((yyvsp[-3].id))->paranum)//semantic check: whether have the same length as defined 
        {
            errors++;
            fprintf(stderr, "Function argument number not fit.\n");
        }
        for (int i = 0; i < (yyvsp[-1].ArgExpr.len); i++)
        {
            //load parameters
            quad_push("param", (yyvsp[-1].ArgExpr.array)[i], NULL, NULL,  NULL);
            //parameter length not fit
            if (((yyvsp[-1].ArgExpr.array)[i]->type != Integer) && ((yyvsp[-1].ArgExpr.array)[i]->type != Constant) && ((yyvsp[-1].ArgExpr.array)[i]->type != Temp))
            {
                errors++;
                fprintf(stderr, "Function argument type not fit.\n");
            }
        }
        tempCnt++;
        (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
        symrec *l = putconst((yyvsp[-1].ArgExpr.len));

        symrec *pt = getsym((yyvsp[-3].id));
        if (pt == 0)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", (yyvsp[-3].id));
        }
        //call function, store value in temporal variable
        quad_push("call", getsym((yyvsp[-3].id)), l, (yyval.ExprType.address), NULL);
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 2914 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 77:
#line 1285 "smallc.y" /* yacc.c:1661  */
    {
        if (((yyvsp[0].ExprType.address)) == 0)//it is an integer variable
        {
            symrec *pt = getsym((yyvsp[-1].id));
            if (pt == 0) //semantic check: whether has been defined
            {
                errors++;
                fprintf(stderr, "Semantic Error!\n");
                fprintf(stderr, "Variable %s not declared!\n", (yyvsp[-1].id));
            }
            else if (pt->type != Integer) //whether it is an integer, not structure
            {
                errors++;
                fprintf(stderr, "Semantic Error!\n");
                fprintf(stderr, "Variable %s is not Integer!\n", (yyvsp[-1].id));   
            }
            (yyval.ExprType.address) = getsym((yyvsp[-1].id));
        }
        else //it is an array element
        {
            if (getsym((yyvsp[-1].id))->type != Array)
            {
                errors++;
                fprintf(stderr, "Variable %s is not an array.\n", (yyvsp[-1].id));
            }
            symrec *zero = putconst(0);
            //set exps' attribute
            (yyval.ExprType.type) = Array;
            (yyval.ExprType.offset) = (yyvsp[0].ExprType.address);
            (yyval.ExprType.array) = getsym((yyvsp[-1].id));            
            tempCnt ++;        
            (yyval.ExprType.address) = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);
            quad_push("=[]", (yyval.ExprType.array), (yyval.ExprType.offset), (yyval.ExprType.address), NULL);
        }
        (yyval.ExprType.isLeft) = 1;
        (yyval.ExprType.isInt) = 1;
    }
#line 2956 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 78:
#line 1323 "smallc.y" /* yacc.c:1661  */
    {
        if (getsym((yyvsp[-2].id))->type != Struct)//semantic check
        {
            errors++;
            fprintf(stderr, "Varible %s is not struct type.\n", (yyvsp[-2].id));
        }
        char *s;
        symrec *pt = getsym((yyvsp[-2].id));
        if (pt == 0)//semantic check
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", (yyvsp[-2].id));
        }
        //get the name of structure attribute in symbol table
        (yyval.ExprType.address) = getsym((yyvsp[-2].id));
        s = malloc(sizeof(char) * (strlen((yyvsp[-2].id)) + strlen((yyvsp[0].id)) + 3));
        strcpy(s, (yyvsp[-2].id));
        strcat(s, ".");
        strcat(s, (yyvsp[0].id));
        //find the structure attribute
        pt = getsym(s);
        if (pt == 0) //semantic check
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", s);
        }
        //pass to reduced element
        (yyval.ExprType.address) = getsym(s);
        (yyval.ExprType.type) = Integer;
        (yyval.ExprType.isLeft) = 1;
        (yyval.ExprType.isInt) = 1;
    }
#line 2995 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 79:
#line 1358 "smallc.y" /* yacc.c:1661  */
    {
        symrec *tp;
        tp = putconst((yyvsp[0].intval));
        (yyval.ExprType.address) = tp;
        (yyval.ExprType.type) = Constant;
        (yyval.ExprType.isLeft) = 0;
        (yyval.ExprType.isInt) = 1;
    }
#line 3008 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 80:
#line 1369 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.instr) = get_nextinst();
    }
#line 3016 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 81:
#line 1375 "smallc.y" /* yacc.c:1661  */
    {    
        (yyval.ExprType.address) = 0;
    }
#line 3024 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 82:
#line 1379 "smallc.y" /* yacc.c:1661  */
    {
        tempCnt ++;        
        symrec *tp;
        //t holds the offset of the array
        tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);        
        (yyval.ExprType.address) = tp;
        tp = putconst(4);
        quad_push("*", (yyvsp[-1].ExprType.address), tp, (yyval.ExprType.address), NULL);
    }
#line 3038 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 83:
#line 1389 "smallc.y" /* yacc.c:1661  */
    {// almost the same as previous one
        tempCnt ++;        
        symrec *tp;

        tp = puttemp("t", 0, Temp, NULL, tempCnt, 0, 0, 0);

        (yyval.ExprType.address) = tp;
        symrec *pt;
        tp = getsym((yyvsp[-6].id));
        if (tp == 0)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s not declared!\n", (yyvsp[-6].id));
        }
        else if (tp->type != Array)
        {
            errors++;
            fprintf(stderr, "Semantic Error!\n");
            fprintf(stderr, "Variable %s does not have type Array!\n", (yyvsp[-6].id));
        }
        tp = putconst(tp->col);        
        //offset =exp1*col+exp2
        quad_push("*", (yyvsp[-4].ExprType.address), tp, (yyval.ExprType.address), NULL);        
        quad_push("+", (yyval.ExprType.address), (yyvsp[-1].ExprType.address), (yyval.ExprType.address), NULL);
        tp = putconst(4);
        quad_push("*", (yyval.ExprType.address), tp, (yyval.ExprType.address), NULL);
    }
#line 3071 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 84:
#line 1420 "smallc.y" /* yacc.c:1661  */
    {
            //increase length
            (yyval.ArgExpr.len) = (yyvsp[-2].ArgExpr.len) + 1;
            //add exp.address to args.array
            for (int i = 0; i < (yyvsp[-2].ArgExpr.len); i++)
            {
                (yyval.ArgExpr.array)[i] = (yyvsp[-2].ArgExpr.array)[i];
            }
            (yyval.ArgExpr.array)[(yyval.ArgExpr.len) - 1] = (yyvsp[0].ExprType.address);
        }
#line 3086 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 85:
#line 1431 "smallc.y" /* yacc.c:1661  */
    {
        //one element
        (yyval.ArgExpr.len) = 1;
        (yyval.ArgExpr.array)[(yyval.ArgExpr.len) - 1] = (yyvsp[0].ExprType.address);
    }
#line 3096 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 86:
#line 1440 "smallc.y" /* yacc.c:1661  */
    {
        if (jumpmain == NULL)//record the palce of main function
        {   
            //jump main should be the first quadruple
            jumpmain = quad_push("jump", NULL, NULL, NULL, NULL);
            jumpmain->target = 0;
        }        
        sym_table = install((yyvsp[-1].id), 0, Function, NULL, 0, 0, 0);         
        symrec * tempt = sym_table;
        //store symbol table and offset
        ST_Stack_Push(&StStack, sym_table);
        sym_table = 0;        
        //generate label to represent the function
        Quad * pt = quad_push("label", NULL, NULL, tempt, NULL);
        if (strcmp((yyvsp[-1].id), "main") == 0)//if the current  function is main, update jump main
        {            
            jumpmain->target = pt;
            tempt->isCalled = 1;
        }
        INT_Stack_Push(&OffsetStack, offset);
        offset = 0;

    }
#line 3124 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 87:
#line 1464 "smallc.y" /* yacc.c:1661  */
    {
        symrec *pt = getsym((yyvsp[-4].id));
        //record parameter number 
        pt->paranum = (yyvsp[-1].intval);
        //pass attributes to func
        (yyval.STypeVal.structST) = sym_table;
        (yyval.STypeVal.type) = Function;
        (yyval.STypeVal.width) = offset;
        (yyval.STypeVal.lexeme) = (yyvsp[-4].id);        
    }
#line 3139 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 88:
#line 1477 "smallc.y" /* yacc.c:1661  */
    {
    }
#line 3146 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 89:
#line 1480 "smallc.y" /* yacc.c:1661  */
    {
        install((yyvsp[0].id), offset, Integer, NULL, 0, 0, 4);
        offset += 4;   
        (yyval.intval) += 1;
    }
#line 3156 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 90:
#line 1486 "smallc.y" /* yacc.c:1661  */
    {
        install((yyvsp[0].id), offset, Integer, NULL, 0, 0, 4);
        offset += 4;
        (yyval.intval) = 1;
    }
#line 3166 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 91:
#line 1499 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ListExpr.nextlist) = (yyvsp[-1].ListExpr.nextlist);
        (yyval.ListExpr.skiplist) = (yyvsp[-1].ListExpr.skiplist);
        (yyval.ListExpr.breaklist) = (yyvsp[-1].ListExpr.breaklist);
    }
#line 3176 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 92:
#line 1507 "smallc.y" /* yacc.c:1661  */
    {        
        (yyval.ListExpr.nextlist) = NULL;
        (yyval.ListExpr.skiplist) = NULL;
        (yyval.ListExpr.breaklist) = NULL;
    }
#line 3186 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 93:
#line 1513 "smallc.y" /* yacc.c:1661  */
    {
        backpatch((yyvsp[-2].ListExpr.nextlist), (yyvsp[-1].instr));
        (yyval.ListExpr.nextlist) = (yyvsp[0].ListExpr.nextlist);
        (yyval.ListExpr.breaklist) = merge((yyvsp[-2].ListExpr.breaklist), (yyvsp[0].ListExpr.breaklist));
        (yyval.ListExpr.skiplist) = merge((yyvsp[-2].ListExpr.skiplist), (yyvsp[0].ListExpr.skiplist));        
    }
#line 3197 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 94:
#line 1522 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ListExpr.nextlist) = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
    }
#line 3206 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 95:
#line 1530 "smallc.y" /* yacc.c:1661  */
    {        
        (yyval.ListExpr.nextlist) = NULL;
        (yyval.ListExpr.skiplist) = NULL;
        (yyval.ListExpr.breaklist) = NULL;
    }
#line 3216 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 96:
#line 1536 "smallc.y" /* yacc.c:1661  */
    {
        (yyval.ListExpr.nextlist) = (yyvsp[0].ListExpr.nextlist);
        (yyval.ListExpr.skiplist) = (yyvsp[0].ListExpr.skiplist);
        (yyval.ListExpr.breaklist) = (yyvsp[0].ListExpr.breaklist);

    }
#line 3227 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 97:
#line 1543 "smallc.y" /* yacc.c:1661  */
    {
        quad_push("return", (yyvsp[-1].ExprType.address), NULL, NULL, NULL);        
        (yyval.ListExpr.nextlist) = NULL;
        (yyval.ListExpr.skiplist) = NULL;
        (yyval.ListExpr.breaklist) = NULL;
    }
#line 3238 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 98:
#line 1550 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[-3].ExprType.isInt) == 0)
        {
            errors++;
            fprintf(stderr, "expression not int type.\n");
        }
        backpatch((yyvsp[-3].ExprType.truelist), (yyvsp[-1].instr));
        (yyval.ListExpr.nextlist) = merge((yyvsp[-3].ExprType.falselist), (yyvsp[0].ListExpr.nextlist));
        (yyval.ListExpr.skiplist) = (yyvsp[0].ListExpr.skiplist);
        (yyval.ListExpr.breaklist) = (yyvsp[0].ListExpr.breaklist);
    }
#line 3254 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 99:
#line 1562 "smallc.y" /* yacc.c:1661  */
    {        
        backpatch((yyvsp[-7].ExprType.truelist), (yyvsp[-5].instr));
        backpatch((yyvsp[-7].ExprType.falselist), (yyvsp[-1].instr));        
        list *temp1 = merge((yyvsp[-4].ListExpr.nextlist), (yyvsp[-3].ListExpr.nextlist));
        //fprintf(stderr, "here!\n");
        (yyval.ListExpr.nextlist) = merge(temp1, (yyvsp[0].ListExpr.nextlist));
        (yyval.ListExpr.skiplist) = merge((yyvsp[-4].ListExpr.skiplist), (yyvsp[0].ListExpr.skiplist));
        (yyval.ListExpr.breaklist) = merge((yyvsp[-4].ListExpr.breaklist), (yyvsp[0].ListExpr.breaklist));
    }
#line 3268 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 100:
#line 1571 "smallc.y" /* yacc.c:1661  */
    {inloop += 1;}
#line 3274 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 101:
#line 1572 "smallc.y" /* yacc.c:1661  */
    {
        if ((yyvsp[-8].ExprType.isInt) == 0)
        {
            errors++;
            fprintf(stderr, "Expression not int type.\n");
        }
        inloop -= 1;        
        backpatch((yyvsp[-8].ExprType.truelist), (yyvsp[-2].instr));
        (yyval.ListExpr.nextlist) = merge((yyvsp[-8].ExprType.falselist), (yyvsp[0].ListExpr.breaklist));
        quad_push("jump", NULL, NULL, NULL, (yyvsp[-6].instr));
        backpatch((yyvsp[-4].ListExpr.nextlist), (yyvsp[-9].instr));
        backpatch((yyvsp[0].ListExpr.nextlist), (yyvsp[-6].instr));
        backpatch((yyvsp[0].ListExpr.skiplist), (yyvsp[-6].instr));        
        (yyval.ListExpr.breaklist) = NULL;
    }
#line 3294 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 102:
#line 1588 "smallc.y" /* yacc.c:1661  */
    {
        if (inloop == 0)
        {
            errors++;
            fprintf(stderr, "Continue not in loop.\n");
        }
        (yyval.ListExpr.skiplist) = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        (yyval.ListExpr.nextlist) = NULL;
        (yyval.ListExpr.breaklist) = NULL;
    }
#line 3310 "smallc.tab.c" /* yacc.c:1661  */
    break;

  case 103:
#line 1600 "smallc.y" /* yacc.c:1661  */
    {        
        if (inloop == 0)
        {
            errors++;
            fprintf(stderr, "Break not in loop.\n");
        }
        (yyval.ListExpr.breaklist) = makelist(get_nextinst());
        quad_push("jump", NULL, NULL, NULL, NULL);
        (yyval.ListExpr.nextlist) = NULL;
        (yyval.ListExpr.skiplist) = NULL;
    }
#line 3326 "smallc.tab.c" /* yacc.c:1661  */
    break;


#line 3330 "smallc.tab.c" /* yacc.c:1661  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1613 "smallc.y" /* yacc.c:1906  */

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

