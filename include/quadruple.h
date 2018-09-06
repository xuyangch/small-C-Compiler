/*************************************************************************
                                QUADRUPLE

This file is used to define all the operations we need to the three-address-code
***************************************************************************/

Quad * codeHead = NULL;  //indicate the head of IR
Quad * jumpmain = NULL;  //indicate the quadruple that holds the beginning of main function

/*=========================================================================
                     Operations for quadruple
=========================================================================*/
//push a quadruple into the IR
Quad *quad_push(char *sOp, symrec *arg1, symrec *arg2, symrec *dest, Quad *target) { //corresponding argument
	static int id = 0;
    char * Op;	
    Op = malloc(sizeof(sOp)+1); 
    strcpy(Op, sOp);
	if (codeHead == NULL)
	{
		codeHead = malloc(sizeof(Quad)); 
		codeHead->next = NULL; 
	}
    Quad * current = codeHead;
    while (current->next != NULL) { //get to the last empty quadruple
        current = current->next;
    }

    //OPTIMIZATION: Arithmetic Identities and Constant folding
    if (strcmp(Op, "+") == 0)
    {
        
        if ((arg1->type == Constant) && (arg1->ConstVal == 0))  // x=0+a -> x=a
        {                        
            Op[0] = '=';
            arg1 = arg2;
            arg2 = NULL;
        }

        else if ((arg2->type == Constant) && (arg2->ConstVal == 0))  // x=a+0 -> x=a
        {
            Op[0] = '=';
            arg2 = NULL;
        }
        else if ((arg1->type == Constant) && (arg2->type == Constant))  //x=a+b -> x =c
        {            
            Op[0] = '=';            
            arg1 = putconst(arg1->ConstVal + arg2->ConstVal);
        }        

    }
    else if (strcmp(Op, "-") == 0)
    {
        if ((arg2->type == Constant) && (arg2->ConstVal == 0))  //x=a-0 -> x=a
        {
            Op[0] = '=';            
        }
        else if (arg1 == arg2)  //x=a-a -> x=0
        {
            Op[0] = '=';
            arg1 = putconst(0);
        }
        else if ((arg1->type == Constant) && (arg2->type == Constant))  //x = a-b -> x=c
        {
            Op[0] = '=';
            arg1 = putconst(arg1->ConstVal - arg2->ConstVal);
        }
    }
    else if (strcmp(Op, "*") == 0)
    {
        if ((arg1->type == Constant) && (arg1->ConstVal == 1))  // x=1*a -> x=a
        {
            Op[0] = '=';
            arg1 = arg2;
            arg2 = NULL;
        }
        else if ((arg2->type == Constant) && (arg2->ConstVal == 1))  // x=a*1 -> x=a
        {
            Op[0] = '=';
            arg2 = NULL;
        }
        else if ((arg1->type == Constant) && (arg1->ConstVal == 2))  // x=2*a -> x=a+a
        {
            Op[0] = '+';
            arg1 = arg2;
        }
        else if ((arg2->type == Constant) && (arg2->ConstVal == 2))  // x=a*2 -> x=a+a
        {
            Op[0] = '+';
            arg2 = arg1;
        }
        else if ((arg1->type == Constant) && (arg2->type == Constant))  //x=a*b -> x=c
        {
            Op[0] = '=';
            arg1 = putconst(arg1->ConstVal * arg2->ConstVal);
        }
    }
    else if (strcmp(Op, "/") == 0)
    {       
        if ((arg2->type == Constant) &&  (arg2->ConstVal == 0))  //check error
        {
            errors++;
            printf("Error: Divided by zero.\n");
        }
        else if ((arg2->type == Constant) && (arg2->ConstVal == 1))  //x=a*1 -> x=a
        {
            Op[0] = '=';
            arg2 = NULL;
        }
        else if ((arg1->type == Constant) && (arg1->ConstVal == 0))  //x=0/a -> x=0
        {
            Op[0] = '=';
        }
        else if (arg1 == arg2)
        {
            Op[0] = '=';
            arg1 = putconst(1);
        }
        else if ((arg1->type == Constant) && (arg2->type == Constant))  //x=a/b -> x=c
        {
            Op[0] = '=';
            arg1 = putconst(arg1->ConstVal / arg2->ConstVal);
        }
    }    
    else if (strcmp(Op, "%") == 0)
    {       
        if ((arg2->type == Constant) &&  (arg2->ConstVal == 0))//check error
        {
            errors++;
            printf("Error: Divided by zero.\n");
        }
        else if ((arg1->type == Constant) && (arg2->type == Constant))  //x=a%b -> x=c
        {
            Op[0] = '=';
            arg1 = putconst(arg1->ConstVal % arg2->ConstVal);
        }
    }
    //assign corresponding arguments
	current->Op = (char *) malloc (strlen(Op)+1);
  	strcpy (current->Op,Op);
  	current->arg1 = arg1;
  	current->arg2 = arg2;
  	current->dest = dest;
  	current->target = target;
  	current->isBlock = 0;
    current->generateLabel = 0;
  	id += 1;
  	current->id = id;
    //generate the place for the next instruction    
    current->next = malloc(sizeof(Quad));
    current->next->next = NULL;    
    return (current);
}

//get the last instruction in IR list, which is the next instruction
Quad *get_nextinst()
{
	Quad * current = codeHead;
	if (codeHead == NULL) return NULL;	
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}

//mark the start of a basic block in IR list
void markblock(Quad * codeHead)
{
    Quad * current = codeHead;
    while (current != NULL)
    {
        if (current == codeHead) current->isBlock = 1;
        //if current quadruple is a jump-like instruction
        if ((strcmp(current->Op, "jump") == 0) || (strcmp(current->Op, ">") == 0) || (strcmp(current->Op, "<") == 0)
        || (strcmp(current->Op, "<=") == 0) || (strcmp(current->Op, ">=") == 0) || (strcmp(current->Op, "==") == 0)
        || (strcmp(current->Op, "call") == 0) || (strcmp(current->Op, "return") == 0))
        {
            //mark the next instructon as the beginning of a block
            current->next->isBlock = 1;
            if ((strcmp(current->Op, "jump") == 0) || (strcmp(current->Op, ">") == 0) || (strcmp(current->Op, "<") == 0)
            || (strcmp(current->Op, "<=") == 0) || (strcmp(current->Op, ">=") == 0) || (strcmp(current->Op, "==") == 0))
            {
            //mark the jump target as the beginning of a block
                current->target->isBlock = 1;
                current->target->generateLabel = 1;
            }
        }
        //if the instruction is a lable, it must be the beginning of a block
        if (strcmp(current->Op, "label") == 0)
        {
            current->isBlock = 1;
        }
        
        current = current->next;
        if (current->id == 0) break;
    }    
}

//OPTIMIZATON: const propagation
void const_prop_in_block(Quad *codeHead)
{
    Quad * current = codeHead;
    symrec * var;
    symrec * con;
    int prop = 0;
    while (current != NULL)
    {        

        prop = 0;
        if ((strcmp(current->Op, "=") == 0) && (current->arg1->type == Constant)) //if a register has const type
        {
            prop = 1;
            var = current->dest;
            con = current->arg1;
        }
        else if (strcmp(current->Op, "+") == 0) 
        {
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant)) //if two operands are both const
            {
                current->Op[0] = '=';
                current->arg1 = putconst(current->arg1->ConstVal + current->arg2->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }
        else if (strcmp(current->Op, "-") == 0) 
        {
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant)) //if two operands are both const
            {
                current->Op[0] = '=';
                current->arg1 = putconst(current->arg1->ConstVal - current->arg2->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }
        else if (strcmp(current->Op, "*") == 0)
        { 
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant)) //if two operands are both const
            {
                current->Op[0] = '=';
                current->arg1 = putconst(current->arg1->ConstVal * current->arg2->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }
        else if (strcmp(current->Op, "/") == 0)
        {       
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant)) //if two operands are both const
            {
                current->Op[0] = '=';
                current->arg1 = putconst(current->arg1->ConstVal / current->arg2->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }
        else if (strcmp(current->Op, "%") == 0)
        {       
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant)) //if two operands are both const
            {
                current->Op[0] = '=';
                current->arg1 = putconst(current->arg1->ConstVal % current->arg2->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }
        else if (strcmp(current->Op, "<<") == 0)
        {       
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant)) //if two operands are both const 
            {
                current->Op[0] = '=';
                current->arg1 = putconst(current->arg1->ConstVal << current->arg2->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }
        else if (strcmp(current->Op, ">>") == 0)
        {       
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant)) //if two operands are both const
            {
                current->Op[0] = '=';
                current->arg1 = putconst(current->arg1->ConstVal >> current->arg2->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }
        else if (strcmp(current->Op, "&") == 0)
        {       
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant)) //if two operands are both const
            {
                current->Op[0] = '=';
                current->arg1 = putconst(current->arg1->ConstVal & current->arg2->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }
        else if (strcmp(current->Op, "|") == 0)
        {       
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant)) //if two operands are both const
            {
                current->Op[0] = '=';
                current->arg1 = putconst(current->arg1->ConstVal | current->arg2->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }
        else if (strcmp(current->Op, "^") == 0)
        {       
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant)) //if two operands are both const
            {
                current->Op[0] = '=';
                current->arg1 = putconst(current->arg1->ConstVal ^ current->arg2->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }
        else if (strcmp(current->Op, "~") == 0)
        {       
            if (current->arg1->type == Constant)  //if first operands is const
            {
                current->Op[0] = '=';
                current->arg1 = putconst(~current->arg1->ConstVal);
                prop = 1;
                var = current->dest;
                con = current->arg1;
            }
        }                                
        if (prop == 1) //need to propagate
        {            
            Quad * temp = current->next;            

            while (temp != NULL)
            {

                if (temp->isBlock == 1)
                {
                    break;
                }
                if ((strcmp(temp->Op, "+") == 0) || (strcmp(temp->Op, "-") == 0) || (strcmp(temp->Op, "*") == 0)
                    || (strcmp(temp->Op, "/") == 0) || (strcmp(temp->Op, "%") == 0) || (strcmp(temp->Op, "=") == 0))
                {
                    //if read the dest variable
                    if (temp->arg1 == var) temp->arg1 = con;
                    if (temp->arg2 == var) temp->arg2 = con;
                }
                if (temp->dest == var)
                {
                    break;
                }
                temp = temp->next;
            }
        }
        current = current->next;
        if (current != NULL)
        {
            //TODO if isBlock, store all registers
            if (current->isBlock == 1) 
            {                
                break;
            }
            if (current->id == 0) break;
        }            
    }
}
void ConstProp(Quad * codeHead)/* propagate const      */
{
    Quad * current = codeHead;
    while (current != NULL)
    {
        if (current->isBlock == 1)
        {
            const_prop_in_block(current);
        }
        current = current->next;
        if (current->id == 0) break;
    }
}

//eliminate assign in a block
void assign_eliminate_in_block(Quad *codeHead)
{
    Quad * current = codeHead;
    symrec * var;
    symrec * con;
    int elim = 0;
    int need = 0;
    while (current != NULL)
    {
        need = 0;

        elim = 0;
        if ((strcmp(current->Op, "=") == 0)) //assign found
        {
            elim = 1;
            var = current->dest;
        }

        if (elim == 1)
        {            
            Quad * temp = current->next;            

            while (temp != NULL)
            {

                if (temp->isBlock == 1)
                {
                    if (var->type != Temp) need = 1;
                    break;
                }                                        
                if (temp->arg1 == var) need = 1;
                if (temp->arg2 == var) need = 1;    
                if (strcmp(temp->Op, "[]=") == 0)
                {
                    if (temp->dest == var) need = 1;
                }
                else if (temp->dest == var)
                {
                    //printf("ahhhh %s\n", temp->Op);
                    break;                                 
                }
                temp = temp->next;                
            }
        }

        if (elim == 1) current->need = need;
        current = current->next;
        if (current != NULL)
        {            
            if (current->isBlock == 1) 
            {                
                break;
            }
            if (current->id == 0) break;
        }            
    }
}

void AssignEliminate(Quad * codeHead)/* eliminate assign      */
{
    Quad * current = codeHead;
    while (current != NULL)
    {
        if (current->isBlock == 1)
        {
            assign_eliminate_in_block(current);
        }
        current = current->next;
        if (current->id == 0) break;
    }
}

//eliminate unused jump
void eliminateJump(Quad * codeHead)
{
    Quad * current = codeHead;
    while (current != NULL)
    {        
        if (strcmp(current->Op, "call") == 0)
        {
            current->arg1->isCalled = 1;
        }
        current = current->next;
        if (current->id == 0) break;
    }
}

//print the IR
void printIR()
{	
	Quad * current = codeHead;
    while (current != NULL) {        
        int dd;
        if (current->target == NULL)
        {
        	dd = 0;
        }
        else dd = current->target->id;    
        if (!(((strcmp(current->Op, "=")==0) && current->need == 0)))
        fprintf(irfp, "%d:  %s %s %s %s %d \n", current->id, current->Op, getName(current->arg1), 
        	getName(current->arg2), getName(current->dest), dd);
        current = current->next;
        if (current->id == 0) break;
    }
}