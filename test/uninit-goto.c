
int goto1()
{
    int x;
    goto label1;
    x = 1;
label1:
    return x; // BAD
}

int goto2()
{
    int x;
    goto label1;
    x = 1;
label1:
    if (abc)
        x=2;
    return x; // BAD
}

int goto3()
{
    int x;
    while (maybe()) {
        x = 1;
        label1:
        do_something(x); // BAD
    }
    if (maybe())
        goto label1;
}


