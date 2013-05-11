
int if1(int a)
{
    int x;
    if (a == 3)
        x = 1;
    return x; // BAD
}

int if2(int y)
{
    int x;
    if (y > 3)
        x = 2;
    else
        return 1;
    return x; // OK
}

int switch1(int a)
{
    int x;
    switch (a) {
    case 1:
        x=1;
        break;
    case 2:
        x=2;
        break;
    };
    return x; // BAD
}

int while1(int a)
{
    int x;
    while (a > 0) {
        x = 1;
        a--;
    }
    return x; // BAD
}
