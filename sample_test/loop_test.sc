int main()
{
	int i;
	int j;
	int t;
	for (i = 0; i < 10; ++i)
	{
		if (i > 5) break;
	}
	write(i);
	for (i = 0; i < 10; ++i)
	{
		t = t + 1;
		for (j = 0; i < 10; ++j)
			if (j > 5) break;
		continue;
		t = 0;
	}
	write(t);
	return 0;
}