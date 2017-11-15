int gcd(int a, int b)    
{    
    int i;    
    if (b==0)    
        i=a;    
    else    
        i=gcd(b,a%b);    
    return i;    
}    
int main()    
{    
    int m,n;    
    read(m);
    read(n);
    write(gcd(m,n));
    return 0;
}      