int pointer1()
{
    int *p;
    return *p; // BAD
}

int pointer2()
{
    int a, *p;
    if (maybe())
        p = &a;
    *p = 0; // BAD
}

int alias1()
{
    int *p;
    int a;
    if (x)
        p = &a;
    else
        return 0;
    return *p; // BAD
}

int alias2()
{
    int *p;
    int a;
    if (x) {
        p = &a;
        return 0;
    } else {
        p = &a;
    }
    *p = 2;
    return a;
}

int alias3()
{
    int a[10];
    int *p = a;
    return *p; // BAD
}

