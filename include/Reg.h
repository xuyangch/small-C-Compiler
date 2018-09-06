/***************************************************************************
                             	Register 
***************************************************************************/
struct Register
{
	int id;	//the unique id for each register
	RType type; //register type, e.g. S, T, V, GP, ST, ...
	ST_Stack reg_descriptor; //record the variable stored in the register
} ;

typedef struct Register Register;

Register MipsReg[32];
char * RegName(int x) //get the register name according to its id
{
    char *str;
    if (x == 0)
    {
    	str = malloc(6 * sizeof(char*));
    	strcpy(str, "$zero");
    	return str;
    }
    str = malloc(5 * sizeof(char*));
    if (x == 1)
    {
    	strcpy(str, "$at");
    	return str;
    }
    if ((x >= 2) && (x <= 3))
    {
    	str[0] = '$';
    	str[1] = 'v';
    	str[2] = '0' + (x - 2);
    	str[3] = '\0';
    }
    if ((x >= 4) && (x <= 7))
    {
    	str[0] = '$';
    	str[1] = 'a';
    	str[2] = '0' + (x - 4);
    	str[3] = '\0';
    }
    if ((x >= 8) && (x <= 15))
    {
    	str[0] = '$';
    	str[1] = 't';
    	str[2] = '0' + (x - 8);
    	str[3] = '\0';
    }
    if ((x >= 16) && (x <= 23))
    {
    	str[0] = '$';
    	str[1] = 's';
    	str[2] = '0' + (x - 16);
    	str[3] = '\0';
    }
    if ((x >= 24) && (x <= 25))
    {
    	str[0] = '$';
    	str[1] = 't';
    	str[2] = '0' + (x - 16);
    	str[3] = '\0';
    }
    if ((x >= 26) && (x <= 27))
    {
    	str[0] = '$';
    	str[1] = 'k';
    	str[2] = '0' + (x - 26);
    	str[3] = '\0';
    }
    if (x == 28)
    {
    	strcpy(str, "$gp");
    }
    if (x == 29)
    {
    	strcpy(str, "$sp");
    }
    if (x == 30)
    {
    	strcpy(str, "$fp");
    }
    if (x == 31)
    {
    	strcpy(str, "$ra");
    }
    return str;    
}
int hasEmptyMipsReg() //if there is a S or T register that holds no variable
{
	for (int i = 0; i < 32; i++)
	{
		if ((MipsReg[i].type == T) || (MipsReg[i].type == S))
		{
			if (ST_Stack_isEmpty(&(MipsReg[i].reg_descriptor)))
				return 1;
		}
	}
	return 0;
}

// return the first quad id that needs x
int get_last_used_id_in_block(Quad *bs, symrec * x) 
{
	int result = bs->id;
	Quad * current = bs;
	while (current != NULL)
	{
		if ((current->arg1 == x) || (current->arg2 == x)) return current->id;
		current = current->next;
		if (current->isBlock == 1)
		{
			return current->id;
		}
	}
	return 0;
}

//get the id of a empty T or S register
int getEmptyMipsReg()
{
	for (int i = 0; i < 32; i++)
	{
		if ((MipsReg[i].type == T) || (MipsReg[i].type == S))
		{
			if (ST_Stack_isEmpty(&(MipsReg[i].reg_descriptor)))
				return i;
		}
	}
	return 0;
}

//spill the value in reg[x] into memory
void Free(int x)
{
    if (!ST_Stack_isEmpty(&(MipsReg[x].reg_descriptor)))
    {
        //printf(";free begins!\n");
        for (int i = 0; i < MipsReg[x].reg_descriptor.size; i++)
        {
        	if (MipsReg[x].reg_descriptor.data[i]->type != Constant)
        	{
        		if (MipsReg[x].reg_descriptor.data[i]->isGlobal == 1) //if stores global variable
        		{
        			//printf("la $a2, _glb_%s\n", MipsReg[x].reg_descriptor.data[i]->name);
        			fprintf(outfp, "la $a2, _glb_%s\n", MipsReg[x].reg_descriptor.data[i]->name);
        			//printf("sw %s, 0($a2)\n", RegName(x));
        			fprintf(outfp, "sw %s, 0($a2)\n", RegName(x));        			
        		}
            	else //if stores local variable
            	{
            		//printf("sw %s, -%d($fp)\n", RegName(x), MipsReg[x].reg_descriptor.data[i]->offset + 4);
            		fprintf(outfp, "sw %s, -%d($fp)\n", RegName(x), MipsReg[x].reg_descriptor.data[i]->offset + 4);
            	}
            }
        }
        ST_Stack_Init(&(MipsReg[x].reg_descriptor));
        //printf(";free ends!\n");
    }
}
void spill()
{
    for (int i = 0; i < 32; i++)
    {
        Free(i);
    }        
                
}

//allocate a register to store variable x
int Allocate(Quad *bs, symrec *x)
{
	int result = -1;
	int i, j;
	for (i = 0; i < 32; i++)
	{		
		//have empty register or a register already has x
		if (((ST_Stack_isEmpty(&(MipsReg[i].reg_descriptor)))|| (ST_Stack_has(&(MipsReg[i].reg_descriptor), x)))
			&& ((MipsReg[i].type == S) || (MipsReg[i].type == T)))
		{
			result = i;
			if (ST_Stack_isEmpty(&(MipsReg[i].reg_descriptor)))
				ST_Stack_Push(&(MipsReg[result].reg_descriptor), x);
			return result;
		}
	}
	int max = bs->id;
	if (result == -1) //if not, spill one register
	{
		for (i = 0; i < 32; i++)
		{
			//find the register that contains the variable used last in the block
			if ((MipsReg[i].type == S) || (MipsReg[i].type == T))
			{
				for (j = 0; j < MipsReg[i].reg_descriptor.size; j++)
				{
					int temp = get_last_used_id_in_block(bs->next, MipsReg[i].reg_descriptor.data[j]);
					if (temp >= max)
					{
						max = temp;
						result = i;
					}
				}
			}
		}
		//TODO spill result
		Free(result);		
		ST_Stack_Push(&(MipsReg[result].reg_descriptor), x);
	}
	return result;
}

//check whether there is a register containning x, if no, allocate one for x
int Ensure(Quad *bs, symrec *x)
{
	int result = -1;
	int i;
	for (i = 0; i < 32; i++)
	{//check whether there is a register containning x
		if (ST_Stack_has(&(MipsReg[i].reg_descriptor), x) == 1)
		{
			result = i;
			return result;
		}
	}
	if (result == -1)//if no, allocate one for x
	{
		result = Allocate(bs, x);
		if (x->type == Constant)
		{
			//printf("li %s, %d\n", RegName(result), x->ConstVal);
			fprintf(outfp, "li %s, %d\n", RegName(result), x->ConstVal);
		}		
		else if (x->isGlobal == 1) //Global variable shoule be accessed using name
		{
			//printf("la $a2, _glb_%s\n", x->name);
			fprintf(outfp, "la $a2, _glb_%s\n", x->name);
			//printf("lw %s, 0($a2)\n", RegName(result));
			fprintf(outfp, "lw %s, 0($a2)\n", RegName(result));			
		}
		else //local variable shoule be accessed using offset
		{
			//printf("lw %s, -%d($fp)\n", RegName(result), x->offset+4);
			fprintf(outfp, "lw %s, -%d($fp)\n", RegName(result), x->offset+4);
		}

	}
	return result;
}

//check whether x will be used after quadruple bs
int notUsed(Quad *bs, symrec *x)
{
	if ((bs == NULL) || (bs->id == 0))return 1;
	while (bs != NULL)
	{
		if ((bs->arg1 == x) || (bs->arg2 == x) || (bs->dest == x)) return 0;
		bs = bs->next;
		if ((bs != NULL) && (bs->isBlock == 1)) return 1;
	}
	return 1;
}

//initialize registers
void MipsRegInit()
{
	int i;
	for (i = 0;i < 32; i++)
	{
		MipsReg[i].id = i;
		ST_Stack_Init(&(MipsReg[i].reg_descriptor));
	}
	//Type
	MipsReg[0].type = Z;
	MipsReg[1].type = O;
	MipsReg[2].type = V;
	MipsReg[3].type = V;
	MipsReg[4].type = A;
	MipsReg[5].type = A;
	MipsReg[6].type = A;
	MipsReg[7].type = A;
	for (i = 8; i <= 15; i++) MipsReg[i].type = T;
	for (i = 16; i <= 23; i++) MipsReg[i].type = S;
	for (i = 24; i <= 25; i++) MipsReg[i].type = T;		
	for (i = 26; i <= 27; i++) MipsReg[i].type = O;
	MipsReg[28].type = GP;
	MipsReg[29].type = SP;
	MipsReg[30].type = FP;
	MipsReg[31].type = RA;
}
