
static void do_something(int *p) {
  *p = 123;
}

static void inc(int *p) {
  *p = *p + 1;
}

void f() {
    int x,y;
    do_something(&x);
    inc(&y); // BAD
}
