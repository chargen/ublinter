void struct1()
{
    struct X x;
    x.y = 0; // OK
}

void struct2()
{
    struct X *x;
    x->y = 0; // BAD
}
