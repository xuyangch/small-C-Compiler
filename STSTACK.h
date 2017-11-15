/*========================================================================
                       A STACK THAT HOLDS ALL SIMBLE TABLE
========================================================================*/
#define STACK_MAX 100
struct ST_Stack {
    symrec* data[STACK_MAX];
    int     size;
};
typedef struct ST_Stack ST_Stack;

ST_Stack StStack;

//check whether the stack has symbol record d
int ST_Stack_has(ST_Stack *S, symrec* d)
{
    for (int i = 0; i < S->size; i++)
    {
        if (S->data[i] == d)
        {
            return 1;
        }
    }
    return 0;
}

//initialize symbol record stack
void ST_Stack_Init(ST_Stack *S)
{
    S->size = 0;
}

//check whether the stack is empty
int ST_Stack_isEmpty(ST_Stack *S)
{
    return (S->size == 0);
}

//return the top element
symrec* ST_Stack_Top(ST_Stack *S)
{
    if (S->size == 0) {
        fprintf(stderr, "Error: stack empty\n");
        return NULL;
    } 

    return S->data[S->size-1];
}

//push an element in stack
void ST_Stack_Push(ST_Stack *S, symrec* d)
{
    if (S->size < STACK_MAX)
        S->data[S->size++] = d;
    else
        fprintf(stderr, "Error: stack full\n");
}
//pop an element in stack
symrec* ST_Stack_Pop(ST_Stack *S)
{
    if (S->size == 0)
    {
        fprintf(stderr, "Error: stack empty\n");
        return NULL;
    }
    else
    {
        symrec *tmp = ST_Stack_Top(S);
        S->size--;
        return tmp;
    }
}

