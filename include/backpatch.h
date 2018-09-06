/*************************************************************************
                                BACKPATCHING

This head file is used to define function needed for back patching, including
makelist, merge and backpatch
***************************************************************************/

list *makelist(Quad *r)//make a list containning the current quadruple r
{
    list *ptr = malloc(sizeof(list));
    ptr->rec = r;
    ptr->next = NULL;
    return ptr;
}

list *merge(list *l1, list *l2) //merge list l1 and l2
{
    list *t;
    if (l1 == NULL)
    {
        t = l1;
        l1 = l2;
        l2 = t;
    }
    if (l1 != NULL)
    {
        list *ptr = l1;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = l2;
        return l1;
    }
    return NULL;
}

void backpatch(list *l, Quad *inst) //backpatch all quadruples in l with inst
{
    if (l == NULL) return;
    list *ptr = l;
    Quad *pt = NULL;
    do
    {
        if (ptr != NULL)
        {
            pt = ptr->rec;
            pt->target = inst;
        }
        ptr = ptr->next;
    } while (ptr != NULL);
}