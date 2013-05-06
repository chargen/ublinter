
int f() {
    int *p;
    int a, b;
    if (x)
        p = &a;
    else
        p = &b;
    return *p;
}

