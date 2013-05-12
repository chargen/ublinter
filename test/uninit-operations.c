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

void eq()
{
    int x;
    if (x==4) {} // BAD
}

void ne()
{
    int x;
    if (x!=4) {} // BAD
}

void gt()
{
    int x;
    if (x>4) {} // BAD
}

void ge()
{
    int x;
    if (x>=4) {} // BAD
}

void lt()
{
    int x;
    if (x<4) {} // BAD
}

void le()
{
    int x;
    if (x<=4) {} // BAD
}

void function_call()
{
    int x;
    f(x); // BAD
}

