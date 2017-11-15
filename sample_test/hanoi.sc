int mov(int n,int a, int b, int c)
{
    if(n==1)
    {
        write(a);
        write(0);
        write(c);
    }
    else
    {
        mov(n-1,a,c,b);
        write(a);
        write(0);
        write(c);
        mov(n-1,b,a,c);
    }
    return 0;
}

int main()
{
    int n;    
    read(n);
    mov(n,1, 2, 3);
    return 0;
}

