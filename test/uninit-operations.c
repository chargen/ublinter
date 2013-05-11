void init()
{
    int x = x; // BAD
}

void add()
{
    int x;
    x = x + 1; // BAD
}

void sub()
{
    int x;
    x = x - 1; // BAD
}

void mul()
{
    int x;
    x = x * 2; // BAD
}

void div()
{
    int x;
    x = x / 2; // BAD
}

void and()
{
    int x;
    x = x & 2; // BAD
}

void or()
{
    int x;
    x = x & 2; // BAD
}

void xor()
{
    int x;
    x = x ^ 2; // BAD
}

void shl()
{
    int x;
    x = x << 1; // BAD
}

void shr()
{
    int x;
    x = x >> 1; // BAD
}

void function_call()
{
    int x;
    x = f(x); // BAD
}

