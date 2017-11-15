int main()
{
	int x = 0xfff;
	int y = 010;
	x = x << 1;
	write(x);
	write(y);
	return(0);
}