void struct1()
{
    struct X x;
    x.y = 0; // OK
    x.y.z = 0; // OK
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

void memset1()
{
    struct ABC abc;
    memset(&abc, 0, sizeof(abc));
    do_something(abc);
}

void memset2()
{
    struct ABC abc;
    memset(&abc, 0, sizeof abc);
    do_something(abc);
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

struct XYZ knownstruct3()
{
    struct XYZ xyz;
    xyz.z = xyz.z + 1; // BAD
}

struct XYZ knownstruct3()
{
    // TODO: warn about 'int x = *p;' instead.
    struct XYZ xyz;
    int *p = &xyz.z; // BAD
    int z = *p;
}

