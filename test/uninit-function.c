
static void do_something(int *p) {
  *p = 123;
}

static void conditional_init(int *p) {
  if (abc)
    return;
  *p = 123;
}

static void inc(int *p) {
  *p = *p + 1;
}

void f() {
    int x,y,z;
    
    do_something(&x);
    if (x == 0) { }
    
    inc(&y); // BAD
    if (y== 0) { } // BAD

    conditional_init(&z); // BAD
    if (z == 0) { } // BAD
}
