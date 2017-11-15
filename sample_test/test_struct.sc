struct stu {int x; int y;} z;
int main()
{
	struct {int x; int y;} g;
	struct stu x;
	z.x = 1;
	x.y = 2;
	g.y = 3;
	write(z.x);
	write(x.y);
	write(g.y);
	return 0;
}

