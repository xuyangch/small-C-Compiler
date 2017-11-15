int fib(int n)
{
    if (n <= 2) return 1;
    else return (fib(n-1)+fib(n-2));
}
int jj(int x)
{
	return 0;
}
int main()
{
    int x;
    read(x);
    write(fib(x));
    return 0;
}

