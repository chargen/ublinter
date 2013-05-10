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

void struct3()
{
     struct ABC abc;
     int x = abc.a;  // BAD
}

void struct4()
{
     struct ABC abc;
     if (abc.a == 2) {} // BAD
}

void struct5()
{
     struct ABC abc;
     struct ABC *p = &abc;
     int x = p->a; // BAD
}

void struct6()
{
     struct ABC abc;
     struct ABC *p = &abc;
     if (p->a == 2) {} // BAD
}

