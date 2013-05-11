void f()
{
    int a[10];
    a[0] = 0;
    int x = a[2]; // BAD
}

void init()
{
    int a[10];
    a[a[0]] = 0; // BAD
}
