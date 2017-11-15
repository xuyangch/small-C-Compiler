int main()
{	
	int x[10], y;
	if (x) return 1;

	for (y = 0; x; ++y)
	{
		break;
	}

	y = x + 1;
	return 0;
}



