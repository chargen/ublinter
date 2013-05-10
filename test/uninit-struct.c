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

struct XY {
    int x;
    int y;
};

struct XYZ {
    struct XY xy;
    int z;
};

struct XY knownstruct1()
{
    struct XY xy;
    xy.x = 1;
    xy.y = 2;
    return xy; // OK
}

struct XYZ knownstruct2()
{
    struct XYZ xyz;
    xyz.z = 1;
    return xyz; // BAD
}



