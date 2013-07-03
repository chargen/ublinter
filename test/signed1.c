int globalvar;

void calc() {
    static int x;
    int y = x & 2; // BAD
}

void arrayindex() {
    int array[10];
    static int x;
    array[x] = 123; // BAD
}

void badsqrtf() {
    static int x;
    sqrtf(x); // BAD
}

void f(int par) {
    int x = globalvar & 2; // BAD
    int y = par & 2; // BAD
}

void types() {
    int array[123];

    char   c = x;
    array[c] = 0; // BAD

    short  s = x;
    array[s] = 0; // BAD

    int    i = x;
    array[i] = 0; // BAD

    long   l = x;
    array[l] = 0; // BAD

    float  f = x;
    sqrtf(f); // OK

    double d = x;
    sqrtf(d); // OK
}

