
char f() {
    char *p;
    p = malloc(100);
    return *p; // BAD
}
