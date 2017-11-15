/***************************************************************************
                             Code Generator

This file contains all functions needed for generating code                             
***************************************************************************/

int produce = 1;//used for unused jump elimination

void gen_code_in_block(Quad * start)
{
    int paramcnt = 0;
    int rz;
    Quad * current = start;
    symrec * ptr;    
    while (current != NULL)
    {
        //for debugging
        /*
        //printf("next is the code for ");
        int dd;
        if (current->target == NULL)
        {
            dd = 0;
        }
        else dd = current->target->id;

        //printf("%d:  %s %s %s %s %d %d\n", current->id, current->Op, getName(current->arg1),
            getName(current->arg2), getName(current->dest), dd, current->isBlock); */
        
        if (strcmp(current->Op,"label") == 0)
        {
            produce = current->dest->isCalled;//check whether this function should be eliminated
        }
        //if not to be eliminated
        if (produce == 1){
        //Label Generation
        if (current->generateLabel == 1)
        {
            //printf("L%d: \n", current->id);
            fprintf(outfp, "L%d: \n", current->id);
            if (strcmp(current->Op, "label") == 0)
            {
                

                //printf("%s: \n", current->dest->name);
                fprintf(outfp, "%s: \n", current->dest->name);
                if (strcmp(current->dest->name, "main") == 0)
                {
                    reOffset(current->dest->strField);
                    //printf("move $fp, $sp\n");
                    fprintf(outfp, "move $fp, $sp\n");
                    int max = 0;
                    for (ptr =  current->dest->strField;
                        ptr != (symrec *) 0;
                        ptr =  (symrec *)ptr->next )
                    {
                        if (max < ptr->offset + ptr->width) max = ptr->offset + ptr->width;                        
                    }
                    //max += 4;
                    //printf("addi $sp, $fp, -%d\n", max);
                    fprintf(outfp, "addi $sp, $fp, -%d\n", max);
                }
            }            
        }
        else if (strcmp(current->Op, "label") == 0)
        {
            //printf("%s: \n", current->dest->name);
            fprintf(outfp, "%s: \n", current->dest->name);
            if (current->dest->type == Function)
            reOffset(current->dest->strField);
        }        
        //arg1[arg2]=dest
        if (strcmp(current->Op, "[]=") == 0)
        {
            int ry = Ensure(current, current->arg2);
            
            if (current->arg1->isGlobal == 1) //global array
            {                                
                //printf("la $a0, _glb_%s\n", current->arg1->name);                
                //printf("add $a0, $a0, %s\n", RegName(ry));
                fprintf(outfp, "la $a0, _glb_%s\n", current->arg1->name);                
                fprintf(outfp, "add $a0, $a0, %s\n", RegName(ry));
                if (current->dest->type == Constant) //assign to constant 
                {
                    //printf("li $a1, %d\n", current->dest->ConstVal);
                    //printf("sw $a1, 0($a0)\n");
                    fprintf(outfp, "li $a1, %d\n", current->dest->ConstVal);
                    fprintf(outfp, "sw $a1, 0($a0)\n");
                }
                else //otherwise allocate register for the dest
                {
                    int rz = Ensure(current, current->dest);
                    //printf("sw %s, 0($a0)\n", RegName(rz));
                    fprintf(outfp, "sw %s, 0($a0)\n", RegName(rz));
                }
            }
            else
            { //not global array, we use stack and offset to access
                int t = 4 * current->arg1->offset;
                //printf("addi $a0, $fp, %d\n", t);
                //printf("add $a0, $a0, %s\n", RegName(ry));   
                fprintf(outfp, "addi $a0, $fp, %d\n", t);
                fprintf(outfp, "add $a0, $a0, %s\n", RegName(ry));   
                if (current->dest->type == Constant)
                {
                    //printf("li $a1, %d\n", current->dest->ConstVal);
                    //printf("sw $a1, -4($a0)\n");
                    fprintf(outfp, "li $a1, %d\n", current->dest->ConstVal);
                    fprintf(outfp, "sw $a1, -4($a0)\n");
                }                             
                else
                {
                    int rz = Ensure(current, current->dest);
                    //printf("sw %s, -4($a0)\n", RegName(rz));
                    fprintf(outfp, "sw %s, -4($a0)\n", RegName(rz));
                }
            }            
            if (notUsed(current->next, current->arg2)) Free(ry);

        }
        //dest=arg1[arg2]
        if (strcmp(current->Op, "=[]") == 0)
        {            
            int ry = Ensure(current, current->arg2);
            
            if (current->arg1->isGlobal == 1)
            {
                int rz = Allocate(current, current->dest);
                //printf("la $a0, _glb_%s\n", current->arg1->name);                
                //printf("add $a0, $a0, %s\n", RegName(ry));
                //printf("lw %s, 0($a0)\n", RegName(rz));
                fprintf(outfp, "la $a0, _glb_%s\n", current->arg1->name);                
                fprintf(outfp, "add $a0, $a0, %s\n", RegName(ry));
                fprintf(outfp, "lw %s, 0($a0)\n", RegName(rz));
            }
            else
            {
                int rz = Allocate(current, current->dest);
                int t = 4 * current->arg1->offset;
                //printf("addi $a0, $fp, %d\n", t);
                //printf("add $a0, $a0, %s\n", RegName(ry));                                
                //printf("lw %s, -4($a0)\n", RegName(rz));
                fprintf(outfp, "addi $a0, $fp, %d\n", t);
                fprintf(outfp, "add $a0, $a0, %s\n", RegName(ry));                                
                fprintf(outfp, "lw %s, -4($a0)\n", RegName(rz));
            }
            if (notUsed(current->next, current->arg2)) Free(ry);
        }        
        //Pass Parameter 
        if (strcmp(current->Op, "param") == 0)
        {
            if (paramcnt == 0)
            {
                //printf("addi $sp, $sp, -4\n");
                //printf("sw $fp, 0($sp)\n");
                //printf("addi $sp, $sp, -4\n");
                //printf("sw $ra, 0($sp)\n");
                //printf("addi $sp, $sp, -4\n");
                fprintf(outfp, "addi $sp, $sp, -4\n");
                fprintf(outfp, "sw $fp, 0($sp)\n");
                fprintf(outfp, "addi $sp, $sp, -4\n");
                fprintf(outfp, "sw $ra, 0($sp)\n");
                fprintf(outfp, "addi $sp, $sp, -4\n");
                int rx = Ensure(current, current->arg1);                
                if (notUsed(current->next, current->arg1)) Free(rx);

                //printf("sw %s, 0($sp)\n", RegName(rx));
                fprintf(outfp, "sw %s, 0($sp)\n", RegName(rx));                
                paramcnt += 1;
            }
            else
            {
                //printf("addi $sp, $sp, -4\n");
                fprintf(outfp, "addi $sp, $sp, -4\n");
                int rx = Ensure(current, current->arg1);
                //printf("sw %s, 0($sp)\n", RegName(rx));
                fprintf(outfp, "sw %s, 0($sp)\n", RegName(rx));
                if (notUsed(current->next, current->arg1)) Free(rx);
                paramcnt += 1;
            }
        }
        //Call Function
        if (strcmp(current->Op, "call") == 0)
        {
            reOffset(current->arg1->strField);
            for (int i = 0; i < 32; i++)
            {
                Free(i);
            }
            int temp = 4*current->arg2->ConstVal;
            if (temp < 0) temp = temp * -1;
            //printf("addi $fp, $sp, %d\n", temp);
            fprintf(outfp, "addi $fp, $sp, %d\n", temp);

            int max = 0;
            for (ptr =  current->arg1->strField;
                    ptr != (symrec *) 0;
                        ptr =  (symrec *)ptr->next )
            {
                if (max < ptr->offset+ptr->width) max = ptr->offset+ptr->width;                
            }            
            //printf("addi $sp, $fp, -%d\n", max);
            //printf("jal %s\n", current->arg1->name);
            //printf("lw $ra, 0($sp)\n");
            //printf("addi $sp, $sp, 4\n");
            //printf("lw $fp, 0($sp)\n");
            //printf("addi $sp, $sp, 4\n");

            fprintf(outfp, "addi $sp, $fp, -%d\n", max);
            fprintf(outfp, "jal %s\n", current->arg1->name);
            fprintf(outfp, "lw $ra, 0($sp)\n");
            fprintf(outfp, "addi $sp, $sp, 4\n");
            fprintf(outfp, "lw $fp, 0($sp)\n");
            fprintf(outfp, "addi $sp, $sp, 4\n");
            symrec * ptr;
            
            paramcnt = 0;
            if ((current->dest) != NULL)
            {
                rz = Allocate(current, current->dest);            
                if (strcmp(current->arg1->name, "write") != 0)
                //printf("move %s, $v0\n", RegName(rz));
                fprintf(outfp, "move %s, $v0\n", RegName(rz));
            }
        }
        //Branch
        //Larger
        if (strcmp(current->Op, ">") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);
                int ry = Ensure(current, current->arg2);
                spill();
                //printf("bgt %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                fprintf(outfp, "bgt %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx); 
                if (notUsed(current->next, current->arg2)) Free(ry);  
            }
            if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg2->ConstVal);
                spill();
                //printf("bgt %s, $a0, L%d\n", RegName(rx), current->target->id);
                fprintf(outfp, "bgt %s, $a0, L%d\n", RegName(rx), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx); 
            }            
            if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                spill();
                //printf("bgt $a0, %s, L%d\n", RegName(ry), current->target->id);
                fprintf(outfp, "bgt $a0, %s, L%d\n", RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg2)) Free(ry);
            }
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                //printf("li $a1, %d\n", current->arg2->ConstVal);

                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a1, %d\n", current->arg2->ConstVal);
                spill();
                //printf("bgt $a0, $a1, L%d\n", current->target->id);
                fprintf(outfp, "bgt $a0, $a1, L%d\n", current->target->id);
                
            }
        }
        //Less
        if (strcmp(current->Op, "<") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);
                int ry = Ensure(current, current->arg2);
                spill();
                //printf("blt %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                fprintf(outfp, "blt %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx);
                if (notUsed(current->next, current->arg2)) Free(ry);
            }
            if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg2->ConstVal);
                spill();
                //printf("blt %s, $a0, L%d\n", RegName(rx), current->target->id);
                fprintf(outfp, "blt %s, $a0, L%d\n", RegName(rx), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx);
            }          
            if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                spill();
                //printf("blt $a0, %s, L%d\n", RegName(ry), current->target->id);
                fprintf(outfp, "blt $a0, %s, L%d\n", RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg2)) Free(ry);
            }
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                //printf("li $a1, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a1, %d\n", current->arg2->ConstVal);
                spill();
                //printf("blt $a0, $a1, L%d\n", current->target->id);
                fprintf(outfp, "blt $a0, $a1, L%d\n", current->target->id);                
            }  
        }
        //LargerOrEqual
        if (strcmp(current->Op, ">=") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);
                int ry = Ensure(current, current->arg2);
                spill();
                //printf("bge %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                fprintf(outfp, "bge %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx);
                if (notUsed(current->next, current->arg2)) Free(ry);
            }
            if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg2->ConstVal);
                spill();
                //printf("bge %s, $a0, L%d\n", RegName(rx), current->target->id);
                fprintf(outfp, "bge %s, $a0, L%d\n", RegName(rx), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx); 
            }
            if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                spill();
                //printf("bge $a0, %s, L%d\n", RegName(ry), current->target->id);
                fprintf(outfp, "bge $a0, %s, L%d\n", RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg2)) Free(ry); 
            }
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                //printf("li $a1, %d\n", current->arg2->ConstVal);

                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a1, %d\n", current->arg2->ConstVal);
                spill();
                //printf("bge $a0, $a1, L%d\n", current->target->id);
                fprintf(outfp, "bge $a0, $a1, L%d\n", current->target->id);                
            }
        }
        //LessOrEqual
        if (strcmp(current->Op, "<=") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);
                int ry = Ensure(current, current->arg2);
                spill();
                //printf("ble %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                fprintf(outfp, "ble %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx); 
                if (notUsed(current->next, current->arg2)) Free(ry);  
            }
            if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg2->ConstVal);
                spill();
                //printf("ble %s, $a0, L%d\n", RegName(rx), current->target->id);
                fprintf(outfp, "ble %s, $a0, L%d\n", RegName(rx), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx); 
            }
            if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                spill();
                //printf("ble $a0, %s, L%d\n", RegName(ry), current->target->id);
                fprintf(outfp, "ble $a0, %s, L%d\n", RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg2)) Free(ry); 
            }
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                //printf("li $a1, %d\n", current->arg2->ConstVal);

                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a1, %d\n", current->arg2->ConstVal);
                spill();
                //printf("ble $a0, $a1, L%d\n", current->target->id);
                fprintf(outfp, "ble $a0, $a1, L%d\n", current->target->id);                
            }
        }        
        //Equal
        if (strcmp(current->Op, "==") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);
                int ry = Ensure(current, current->arg2);
                spill();
                //printf("beq %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                fprintf(outfp, "beq %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx); 
                if (notUsed(current->next, current->arg2)) Free(ry);  
            }
            if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg2->ConstVal);
                spill();
                //printf("beq %s, $a0, L%d\n", RegName(rx), current->target->id);
                fprintf(outfp, "beq %s, $a0, L%d\n", RegName(rx), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx); 
            }
            if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                spill();
                //printf("beq %s, $a0, L%d\n", RegName(ry), current->target->id);
                fprintf(outfp, "beq %s, $a0, L%d\n", RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg2)) Free(ry); 
            }
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                //printf("li $a1, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a1, %d\n", current->arg2->ConstVal);
                spill();
                //printf("beq $a0, $a1, L%d\n", current->target->id);
                fprintf(outfp, "beq $a0, $a1, L%d\n", current->target->id);
                 ; 
            }
        }
        //NotEqual
        if (strcmp(current->Op, "!=") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);
                int ry = Ensure(current, current->arg2);
                spill();
                //printf("bne %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                fprintf(outfp, "bne %s, %s, L%d\n", RegName(rx), RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx); 
                if (notUsed(current->next, current->arg2)) Free(ry);  
            }
            if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg2->ConstVal);
                spill();
                //printf("bne %s, $a0, L%d\n", RegName(rx), current->target->id);
                fprintf(outfp, "bne %s, $a0, L%d\n", RegName(rx), current->target->id);
                if (notUsed(current->next, current->arg1)) Free(rx); 
            }
            if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                spill();
                //printf("bne %s, $a0, L%d\n", RegName(ry), current->target->id);
                fprintf(outfp, "bne %s, $a0, L%d\n", RegName(ry), current->target->id);
                if (notUsed(current->next, current->arg2)) Free(ry); 
            }
            if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                //printf("li $a1, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a1, %d\n", current->arg2->ConstVal);
                spill();
                //printf("bne $a0, $a1, L%d\n", current->target->id);
                fprintf(outfp, "bne $a0, $a1, L%d\n", current->target->id);
                 ; 
            }
        }
        //Jump
        if (strcmp(current->Op, "jump") == 0)
        {
            spill();
            //printf("j L%d\n", current->target->id);
            fprintf(outfp, "j L%d\n", current->target->id);
        }
        //Return 
        if (strcmp(current->Op, "return") == 0)
        {
            if (current->arg1->type == Constant)
            {
                //printf("li $v0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $v0, %d\n", current->arg1->ConstVal);
            }
            else
            {
                int rx = Ensure(current, current->arg1);
                //printf("move $v0, %s\n", RegName(rx));
                fprintf(outfp, "move $v0, %s\n", RegName(rx));
                if (notUsed(current->next, current->arg1)) Free(rx);
            }
            spill();
            //printf("move $sp, $fp\n");
            //printf("jr $ra\n");
            fprintf(outfp, "move $sp, $fp\n");
            fprintf(outfp, "jr $ra\n");
        }
        //Assign
        if ((strcmp(current->Op, "=") == 0) && (current->need == 1))
        {
            if (current->arg1->type != Constant)
            {
                int rx = Ensure(current, current->arg1);
                int rz = Allocate(current, current->dest);
                //printf("move %s, %s\n", RegName(rz), RegName(rx));
                fprintf(outfp, "move %s, %s\n", RegName(rz), RegName(rx));
            }
            else
            {
                int rz = Allocate(current, current->dest);
                //printf("li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                fprintf(outfp, "li %s, %d\n", RegName(rz), current->arg1->ConstVal);
            }
        }
        //Binary
        //Add
        if (strcmp(current->Op, "+") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);                
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg1)) Free(rx); 
                if (notUsed(current->next, current->arg2)) Free(ry);  
                int rz = Allocate(current, current->dest);
                //TODO
                
                //printf("add %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
                fprintf(outfp, "add %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
            }
            else if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                if (notUsed(current->next, current->arg1)) Free(rx);                 
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("addi %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
                fprintf(outfp, "addi %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg2)) Free(ry);  
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("addi %s, %s, %d\n", RegName(rz), RegName(ry), current->arg1->ConstVal);                
                fprintf(outfp, "addi %s, %s, %d\n", RegName(rz), RegName(ry), current->arg1->ConstVal);                
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                //printf("addi %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
                fprintf(outfp, "li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                fprintf(outfp, "addi %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
            }
        }
        //Sub
        if (strcmp(current->Op, "-") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg1)) Free(rx); 
                if (notUsed(current->next, current->arg2)) Free(ry);  
                int rz = Allocate(current, current->dest);
                //TODO
                /*printStack(&(MipsReg[rx].reg_descriptor));
                //printf("-------------------\n");
                //printStack(&(MipsReg[rz].reg_descriptor));*/
                //printf("sub %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
                fprintf(outfp, "sub %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
            }
            else if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                if (notUsed(current->next, current->arg1)) Free(rx); 
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("addi %s, %s, -%d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
                fprintf(outfp, "addi %s, %s, -%d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg2)) Free(ry);  
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("addi %s, %s, -%d\n", RegName(rz), RegName(ry), current->arg1->ConstVal);                
                //printf("sub %s, %s, %s\n", RegName(rz), "$zero", RegName(rz));
                fprintf(outfp, "addi %s, %s, -%d\n", RegName(rz), RegName(ry), current->arg1->ConstVal);                
                fprintf(outfp, "sub %s, %s, %s\n", RegName(rz), "$zero", RegName(rz));
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                //printf("addi %s, %s, -%d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
                fprintf(outfp, "li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                fprintf(outfp, "addi %s, %s, -%d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
            }
        }
        //Multiply
        if (strcmp(current->Op, "*") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);                
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg1)) Free(rx); 
                if (notUsed(current->next, current->arg2)) Free(ry);  
                int rz = Allocate(current, current->dest);
                //TODO
                /*printStack(&(MipsReg[rx].reg_descriptor));
                //printf("-------------------\n");
                //printStack(&(MipsReg[rz].reg_descriptor));*/
                //printf("mul %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
                fprintf(outfp, "mul %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
            }
            else if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                if (notUsed(current->next, current->arg1)) Free(rx); 
                //printf("li $a0, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg2->ConstVal);
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("mul %s, %s, $a0\n", RegName(rz), RegName(rx));
                fprintf(outfp, "mul %s, %s, $a0\n", RegName(rz), RegName(rx));
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg2)) Free(ry);  
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("mul %s, %s, $a0\n", RegName(rz), RegName(ry));
                fprintf(outfp, "mul %s, %s, $a0\n", RegName(rz), RegName(ry));
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                //printf("li $a1, %d\n", current->arg2->ConstVal);
                //printf("mul %s, $a0, $a1\n", RegName(rz));
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a1, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "mul %s, $a0, $a1\n", RegName(rz));
            }
        }

        //Divide
        if (strcmp(current->Op, "/") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);                
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg1)) Free(rx);
                if (notUsed(current->next, current->arg2)) Free(ry);
                int rz = Allocate(current, current->dest);
                //TODO
                /*printStack(&(MipsReg[rx].reg_descriptor));
                //printf("-------------------\n");
                //printStack(&(MipsReg[rz].reg_descriptor));*/
                //printf("div %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
                fprintf(outfp, "div %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));
            }
            else if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                if (notUsed(current->next, current->arg1)) Free(rx);
                //printf("li $a0, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg2->ConstVal);
                int rz = Allocate(current, current->dest);
                
                //printf("div %s, %s, $a0\n", RegName(rz), RegName(rx));
                fprintf(outfp, "div %s, %s, $a0\n", RegName(rz), RegName(rx));
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg2)) Free(ry);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                int rz = Allocate(current, current->dest);
                
                //printf("div %s, $a0, %s\n", RegName(rz), RegName(ry));
                fprintf(outfp, "div %s, $a0, %s\n", RegName(rz), RegName(ry));
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                //printf("li $a1, %d\n", current->arg2->ConstVal);
                //printf("div %s, $a0, $a1\n", RegName(rz));
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a1, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "div %s, $a0, $a1\n", RegName(rz));
            }
        }
        //Modulus
        if (strcmp(current->Op, "%") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);                
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg1)) Free(rx);
                if (notUsed(current->next, current->arg2)) Free(ry);
                int rz = Allocate(current, current->dest);
                
                //printf("rem %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
                fprintf(outfp, "rem %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));
            }
            else if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                if (notUsed(current->next, current->arg1)) Free(rx);
                //printf("li $a0, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg2->ConstVal);
                int rz = Allocate(current, current->dest);
                
                //printf("rem %s, %s, $a0\n", RegName(rz), RegName(rx));
                fprintf(outfp, "rem %s, %s, $a0\n", RegName(rz), RegName(rx));
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg2)) Free(ry);
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                int rz = Allocate(current, current->dest);
                
                //printf("rem %s, $a0, %s\n", RegName(rz), RegName(ry));
                fprintf(outfp, "rem %s, $a0, %s\n", RegName(rz), RegName(ry));
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                //printf("li $a1, %d\n", current->arg2->ConstVal);
                //printf("rem %s, $a0, $a1\n", RegName(rz));
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a1, %d\n", current->arg2->ConstVal);
                fprintf(outfp, "rem %s, $a0, $a1\n", RegName(rz));
            }
        }        
        //Not
        if (strcmp(current->Op, "~") == 0)
        {
            if (current->arg1->type != Constant)
            {
                int rx = Ensure(current, current->arg1);                
                if (notUsed(current->next, current->arg1)) Free(rx);                
                int rz = Allocate(current, current->dest);
                
                //printf("not %s, %s\n", RegName(rz), RegName(rx));
                fprintf(outfp, "not %s, %s\n", RegName(rz), RegName(rx));
            }            
            else if (current->arg1->type == Constant)
            {                
                //printf("li $a0, %d\n", current->arg1->ConstVal);
                fprintf(outfp, "li $a0, %d\n", current->arg1->ConstVal);
                int rz = Allocate(current, current->dest);
                
                //printf("not %s, $a0\n", RegName(rz));
                fprintf(outfp, "not %s, $a0\n", RegName(rz));
            }            
        }
        //And
        if (strcmp(current->Op, "&") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);                
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg1)) Free(rx);
                if (notUsed(current->next, current->arg2)) Free(ry);
                int rz = Allocate(current, current->dest);
                
                //printf("and %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
                fprintf(outfp, "and %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
            }
            else if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                if (notUsed(current->next, current->arg1)) Free(rx);
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("andi %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
                fprintf(outfp, "andi %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg2)) Free(ry);
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("andi %s, %s, %d\n", RegName(rz), RegName(ry), current->arg1->ConstVal);                
                fprintf(outfp, "andi %s, %s, %d\n", RegName(rz), RegName(ry), current->arg1->ConstVal);                
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                //printf("andi %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
                fprintf(outfp, "li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                fprintf(outfp, "andi %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
            }
        }
        //Or
        if (strcmp(current->Op, "|") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);                
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg1)) Free(rx);
                if (notUsed(current->next, current->arg2)) Free(ry);
                int rz = Allocate(current, current->dest);
                
                //printf("or %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
                fprintf(outfp, "or %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
            }
            else if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                if (notUsed(current->next, current->arg1)) Free(rx);
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("ori %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
                fprintf(outfp, "ori %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg2)) Free(ry);
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("ori %s, %s, %d\n", RegName(rz), RegName(ry), current->arg1->ConstVal);                
                fprintf(outfp, "ori %s, %s, %d\n", RegName(rz), RegName(ry), current->arg1->ConstVal);                
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                //printf("ori %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
                fprintf(outfp, "li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                fprintf(outfp, "ori %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
            }
        }
        //Xor
        if (strcmp(current->Op, "^") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);                
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg1)) Free(rx);
                if (notUsed(current->next, current->arg2)) Free(ry);
                int rz = Allocate(current, current->dest);
                
                //printf("xor %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
                fprintf(outfp, "xor %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
            }
            else if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                if (notUsed(current->next, current->arg1)) Free(rx);
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("xori %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
                fprintf(outfp, "xori %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg2)) Free(ry);
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("xori %s, %s, %d\n", RegName(rz), RegName(ry), current->arg1->ConstVal);                
                fprintf(outfp, "xori %s, %s, %d\n", RegName(rz), RegName(ry), current->arg1->ConstVal);                
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                //printf("xori %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
                fprintf(outfp, "li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                fprintf(outfp, "xori %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
            }
        }
        //SL
        if (strcmp(current->Op, "<<") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg1)) Free(rx);
                if (notUsed(current->next, current->arg2)) Free(ry);
                int rz = Allocate(current, current->dest);
                
                //printf("sllv %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
                fprintf(outfp, "sllv %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
            }
            else if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                if (notUsed(current->next, current->arg1)) Free(rx);
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("sll %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
                fprintf(outfp, "sll %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int rz = Allocate(current, current->dest);
                //printf("li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                fprintf(outfp, "li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg2)) Free(ry);
                
                //TODO
                //printf("sllv %s, %s, %s\n", RegName(rz), RegName(rz), RegName(ry));
                fprintf(outfp, "sllv %s, %s, %s\n", RegName(rz), RegName(rz), RegName(ry));
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                //printf("sll %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
                fprintf(outfp, "li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                fprintf(outfp, "sll %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
            }
        }
        //SR
        if (strcmp(current->Op, ">>") == 0)
        {
            if ((current->arg1->type != Constant) && (current->arg2->type != Constant))
            {
                int rx = Ensure(current, current->arg1);
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg1)) Free(rx);
                if (notUsed(current->next, current->arg2)) Free(ry);
                int rz = Allocate(current, current->dest);
                
                //printf("srav %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));                
                fprintf(outfp, "srav %s, %s, %s\n", RegName(rz), RegName(rx), RegName(ry));
            }
            else if ((current->arg1->type != Constant) && (current->arg2->type == Constant))
            {
                int rx = Ensure(current, current->arg1);
                if (notUsed(current->next, current->arg1)) Free(rx);
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("sra %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
                fprintf(outfp, "sra %s, %s, %d\n", RegName(rz), RegName(rx), current->arg2->ConstVal);
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type != Constant))
            {
                int rz = Allocate(current, current->dest);
                //printf("li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                fprintf(outfp, "li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                int ry = Ensure(current, current->arg2);
                if (notUsed(current->next, current->arg2)) Free(ry);
                
                //TODO
                //printf("srav %s, %s, %s\n", RegName(rz), RegName(rz), RegName(ry));
                fprintf(outfp, "srav %s, %s, %s\n", RegName(rz), RegName(rz), RegName(ry));
            }
            else if ((current->arg1->type == Constant) && (current->arg2->type == Constant))
            {
                int rz = Allocate(current, current->dest);
                //TODO
                //printf("li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                //printf("sra %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
                fprintf(outfp, "li %s, %d\n", RegName(rz), current->arg1->ConstVal);
                fprintf(outfp, "sra %s, %s, %d\n", RegName(rz), RegName(rz), current->arg2->ConstVal);
            }
        }     
        }       
        current = current->next;
        if (current != NULL)
        {
            //TODO if isBlock, store all registers
            if (current->isBlock == 1) 
            {
                spill();
                break;
            }
            if (current->id == 0) break;
        }
                
    }
}
//set up the global data into .data segment
void GlobalDataSet()
{
    //printf(".data\n");
    fprintf(outfp, ".data\n");
    symrec *ptr;
    for ( ptr =  global_sym_table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
    {
        if (ptr->type == Integer) //initialization for integer
        {
            //printf("_glb_%s:\t.word %d\n", ptr->name, ptr->ConstVal);
            fprintf(outfp, "_glb_%s:\t.word %d\n", ptr->name, ptr->ConstVal);
        }
        else if (ptr->type == Array) //initialization for array
        {
            //printf("_glb_%s:\t.word ", ptr->name);
            fprintf(outfp, "_glb_%s:\t.word ", ptr->name);
            for (int i = 0; i < ptr->row * ptr->col; i++)
            {
                //printf("%d",ptr->arrayInit[i]);
                fprintf(outfp, "%d",ptr->arrayInit[i]);
                if (i < ptr->row * ptr->col - 1)
                {
                    //printf(", ");            
                    fprintf(outfp, ", ");
                }                
            }
            //printf("\n");
            fprintf(outfp, "\n");
        }
    }
    //the regular global variables
    //printf("_prompt: .asciiz \"Enter an integer:\"\n");
    //printf(".globl main\n");
    //printf("_ret: .asciiz \"\\n\"\n");
    fprintf(outfp, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(outfp, ".globl main\n");
    fprintf(outfp, "_ret: .asciiz \"\\n\"\n");
}
void InitText()//print the read and write function
{
    //printf(".text\n");
    //printf("read:\nli $v0, 4\nla $a0, _prompt\nsyscall\nli $v0, 5\nsyscall\nmove $sp, $fp\njr $ra\n");
    //printf("write:\nlw $a0, -4($fp)\nli $v0, 1\nsyscall\nli $v0, 4\nla $a0, _ret\nsyscall\nmove $v0, $0\nmove $sp, $fp\njr $ra\n");
    fprintf(outfp, ".text\n");
    fprintf(outfp, "read:\nli $v0, 4\nla $a0, _prompt\nsyscall\nli $v0, 5\nsyscall\nmove $sp, $fp\njr $ra\n");
    fprintf(outfp, "write:\nlw $a0, -4($fp)\nli $v0, 1\nsyscall\nli $v0, 4\nla $a0, _ret\nsyscall\nmove $v0, $0\nmove $sp, $fp\njr $ra\n");
}
void Markglobal() //mark all the variables that are global variables
{
    symrec *ptr;
    for ( ptr =  global_sym_table;
        ptr != (symrec *) 0;
        ptr =  (symrec *)ptr->next )
        {            
            ptr->isGlobal = 1;
        }
}
void gen_code(Quad * codeHead)/* Generates code at current location      */
{
    Quad * current = codeHead;    
    GlobalDataSet();
    InitText();
    MipsRegInit();
    Markglobal();
    while (current != NULL)
    {
        if (current->isBlock == 1)
        {
            gen_code_in_block(current);
        }
        current = current->next;
        if (current->id == 0) break;
    }
}