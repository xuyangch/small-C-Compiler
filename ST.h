/***************************************************************************
                              SYMBOL TABLE
              
This file contains all the function for symbol table.
These function enables you to add or check the content of symbol table                  
***************************************************************************/

int offset;
symrec *identifier;

symrec *const_table = (symrec *)0; /* The pointer to the Const Table */
symrec *sym_table = (symrec *)0; /* The pointer to the Symbol Table */
symrec *global_sym_table = (symrec *)0; /* The pointer to the Symbol Table */
int tempCnt = 0;
/*========================================================================
                          SYMBOL TABLE OPERATIONS
========================================================================*/
//put a symbol table record into symbol_table
symrec * putsym (char *sym_name, int offset, Type t, symrec *sF, int Cnt, int ConstVal, int row, int col, int width)
{
  symrec *ptr;
  ptr = (symrec *) malloc (sizeof(symrec));
  ptr->name = (char *) malloc (strlen(sym_name)+1);
  ptr->name[strlen(sym_name)] = '\0';
  //assign value
  strcpy (ptr->name,sym_name);  
  ptr->offset = offset; 
  ptr->type = t;  
  {
    ptr->strField = sF;
  }
  ptr->tempCnt = Cnt;
  ptr->ConstVal = ConstVal;
  ptr->row = row;
  ptr->col = col;
  ptr->width = width;
  ptr->next = (struct symrec *)sym_table;
  ptr->isGlobal = 0;
  ptr->isCalled = 0;
  sym_table = ptr;
  if (ST_Stack_isEmpty(&StStack))
  {
    global_sym_table = sym_table;
  }
  return ptr;
}

//put a temperal symbol table record into symbol_table
symrec * puttemp (char *sym_name, int offset, Type t, symrec *sF, int Cnt, int ConstVal, int row, int col)
{
  symrec *ptr;
  ptr = (symrec *) malloc (sizeof(symrec));
  ptr->name = (char *) malloc (strlen(sym_name)+2);
  ptr->name[strlen(sym_name)] = '\0';
  //assign value
  strcpy (ptr->name,sym_name);
  ptr->name[0] = '_';
  ptr->name[1] = 't';
  ptr->name[2] ='\0';
  ptr->offset = offset;
  ptr->type = t;  
  {
    ptr->strField = sF;
  }
  ptr->tempCnt = Cnt;
  ptr->ConstVal = ConstVal;
  ptr->row = row;
  ptr->col = col;  
  ptr->next = (struct symrec *)sym_table;  
  sym_table = ptr;
  return ptr;
}
//get a symbol record from symbol_table using name
symrec * getsym (char *sym_name)
{  
    symrec *ptr;
    for ( ptr =  sym_table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
        {                        
            if (strcmp (ptr->name,sym_name) == 0)
            return ptr;
        }
    for ( ptr =  global_sym_table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
        {                        
            if (strcmp (ptr->name,sym_name) == 0)
            return ptr;
        }
  return 0;
}
//get a temporal symbol record from symbol_table using name
symrec * gettemp (char *sym_name)
{  
    symrec *ptr;
    for ( ptr =  sym_table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
        {            
            if (strcmp (ptr->name,sym_name) == 0)
            return ptr;
        }
    
  return 0;
}
//check whether a symbol record having the sym_name is in the symbol table
symrec * currsymtablecheck(char *sym_name)
{  
    symrec *ptr;
    for ( ptr =  sym_table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
        {                        
            if (strcmp (ptr->name,sym_name) == 0)
            return ptr;
        }
    return 0;
}
// get a symbol record from symbol_table using the offset
symrec * getsym_by_offset (int off)
{  
    symrec *ptr;
    for ( ptr =  sym_table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
        {            
            if ((ptr->type == Integer) && (ptr->offset == off)) return ptr;
        }
    
  return 0;
}
/*========================================================================
                          CONST TABLE OPERATIONS
========================================================================*/
//get the const symbol record having const value n
symrec * getconst (int n)
{  
    symrec *ptr;
    for ( ptr =  const_table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
        {
            
            if (ptr->ConstVal == n)
            return ptr;
        }
  return 0;
}
//get a const symbol record having const value n into the const_table
symrec * putconst(int n)
{
    symrec *ptr;
    ptr = getconst(n);
    if (ptr == 0)
    {
        ptr = (symrec *) malloc (sizeof(symrec));  
        ptr->type = Constant;
        ptr->ConstVal = n;
        //ptr->addressDescriptorList.size = 0;
        ptr->next = (struct symrec *)const_table;
        const_table = ptr;
    }  
    return ptr;
}

/*========================================================================
                          OTHER OPERATIONS
========================================================================*/
//assign the actual memory offset to the temporal variable symbol table
void reOffset(symrec *table)
{
    int max = 0;
    symrec * ptr;
    for ( ptr =  table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
    {
        if (ptr->type != Temp){
        //printf("Name: %s, offset: %d, Type: %d, Cnt: %d, ConstVal: %d, Width: %d\n",ptr->name, ptr->offset, ptr->type, tempCnt, ptr->ConstVal, ptr->width);
        if (max < ptr->offset + ptr->width) max = ptr->offset + ptr->width; //get the max offset of non-temporal variables
      }
    }
    //printf("reoffset max : %d\n", max);
    for ( ptr =  table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
    {
        if (ptr->type == Temp)
        {            
            ptr->offset = max; //temporal variables are located after other other variables
            ptr->width = 4;
            max += 4;
        }
    }
}

//transform a number to string
char* itoa(int n) {
    char* ret = NULL;
    int num_chars = 0;
 
    // Determine if integer is negative
    int negative = 0;
    if (n < 0) {
        n = -n;
        negative = 1;
        num_chars++;
    }
 
    // count how many chars
    int temp = n;
    do {
        num_chars++;
        temp /= 10;
    } while ( temp );
 
    ret = (char*)malloc(sizeof(char)*(num_chars+1));
    ret[num_chars] = '\0';
 
    //Add the negative sign if needed
    if (negative) ret[0] = '-';
 
    // Copy digits to string in reverse order
    int i = num_chars - 1;
    do {
        ret[i--] = n%10 + '0';
        n /= 10;
    } while (n);
 
    return ret;
}
//get the name of a symbol record
char *getName(symrec * s)
{
    if (s == NULL)
    {
        return "NULL";
    }
    else if(s->type == Constant) //const
    {

        return itoa(s->ConstVal);
        //return (str);
    }
    else if (s->type == Temp) //temp, for temp the name shoud be "_t"+"temp count"
    {
        char *str;
        str = malloc(25 * sizeof(char*));
        //s[3] = '\0';
        for (int i = 0; i < 25; i++) str[i] = '\0';
        strcpy(str, itoa(s->tempCnt));        
        s->name[2] = '\0';        
        return strcat(s->name, str);
    }
    return s->name;
}

//print the symbol table
void printST(symrec *table)
{
    symrec *ptr;
    for ( ptr =  table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
    {
        printf("Name: %s, offset: %d, Type: %d, Cnt: %d, ConstVal: %d, Width: %d\n",ptr->name, ptr->offset, ptr->type, tempCnt, ptr->ConstVal, ptr->width);
    }
}
