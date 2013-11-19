
int x = 5 / y; // BAD

void f() {
	int a = 5 / 0; // BAD
	int b = 5 / x; // BAD
	int c = x / 2;
	
	float f1 = x / 0.5;
	float f2 = 1.0 / (1.0 * (float)2);
}

void casts() {
    int x = 12 / (char)1000;
	float f1 = x / (float)2;
}

void sz() {
	int n = 1234 / sizeof(x);
	int n = 1234 / (signed long int)sizeof(x);
}
