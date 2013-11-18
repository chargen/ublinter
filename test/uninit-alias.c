
void var(int x) {
    int a;
    int *p = &a;
    int *q = p; // FP
    *p = 1 + *p; // BAD
}

void arr() {
    int a[100];
    int *p = a;
    p[0] = p[1]; // BAD
}
