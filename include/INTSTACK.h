/*========================================================================
                       A STACK THAT HOLDS ALL Integers(Offset)
========================================================================*/
#define STACK_MAX 100
struct INT_Stack {
    int     data[STACK_MAX];
    int     size;
};
typedef struct INT_Stack INT_Stack;

INT_Stack OffsetStack;

//initialize int stack
void INT_Stack_Init(INT_Stack *S)
{
    S->size = 0;
}

//get the top element of int stack
int INT_Stack_Top(INT_Stack *S)
{
    if (S->size == 0) {
        fprintf(stderr, "Error: stack empty\n");
        return 0;
    } 

    return S->data[S->size-1];
}
//push an element into int stack
void INT_Stack_Push(INT_Stack *S, int d)
{
    if (S->size < STACK_MAX)
        S->data[S->size++] = d;
    else
        fprintf(stderr, "Error: stack full\n");
}
//pop an element into int stack
int INT_Stack_Pop(INT_Stack *S)
{
    if (S->size == 0)
    {
        fprintf(stderr, "Error: stack empty\n");
        return 999;
    }
    else
    {
        int tmp = INT_Stack_Top(S);
        S->size--;
        return tmp;
    }
}

