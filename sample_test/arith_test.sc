int main()
{
    int x = 1, y = 2, z = 3;

    x = -y;
    write(x);
    x = -2;
    write(x);

    x = ++y;
    write(x);
    write(y);
    
    x = --y;
    write(x);
    write(y);

    x = ~3;
    write(x);
    x = ~z;
    write(x);

    x = 2 * 3;
    write(x);
    x = y * 3;
    write(x);
    x = 2 * z;
    write(x);
    x = y * z;
    write(x);

    x = 6 / 2;
    write(x);
    x = (y * z) / 2;
    write(x);
    x = 6 / y;
    write(x);
    x = (y * z) / y;
    write(x);

    x = 3 % 2;
    write(x);
    x = z % 2;
    write(x);
    x = 3 % y;
    write(x);
    x = z % y;
    write(x);

    x = 2 + 3;
    write(x);
    x = y + 3;
    write(x);
    x = 2 + z;
    write(x);
    x = y + z;
    write(x);

    x = 2 - 3;
    write(x);
    x = y - 3;
    write(x);
    x = 2 - z;
    write(x);
    x = y - z;
    write(x);    

    x = 3 << 2;
    write(x);
    x = z << 2;
    write(x);
    x = 3 << y;
    write(x);
    x = z << y;
    write(x);

    x = 3 >> 1;
    write(x);
    x = z >> 1;
    write(x);
    x = 3 >> x;
    write(x);
    x = z >> x;
    write(x);

    x = 2 & 3;
    write(x);
    x = y & 3;
    write(x);
    x = 2 & z;
    write(x);
    x = y & z;
    write(x);

    x = 2 | 3;
    write(x);
    x = y | 3;
    write(x);
    x = 2 | z;
    write(x);
    x = y | z;
    write(x);

    x = 2 ^ 3;
    write(x);
    x = y ^ 3;
    write(x);
    x = 2 ^ z;
    write(x);
    x = y ^ z;
    write(x);

    x += 1;
    write(x);
    x -= 1;
    write(x);
    x *= 2;
    write(x);
    x /= 2;
    write(x);
    x ^= 1;
    write(x);
    x |= 1;
    write(x);
    x &= 1;
    write(x);
    x <<= 1;
    write(x);
    x >>= 1;
    write(x);

    return 0;    
}