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



