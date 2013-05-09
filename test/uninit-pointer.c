int f()
{
    int *p;
    int a, b;
    if (x)
        p = &a; // BAD
    else
        p = &b; // BAD
    return *p;
}
