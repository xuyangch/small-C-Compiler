int i;
int foo(int n)
{	
	for (i = 0; i < n; ++i)
	{
		if (i >= n/2)
		{
			break;
		}
	}
	return i;
}
int main()
{
	write(foo(10));
	return 0;
}