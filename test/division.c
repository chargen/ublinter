
int x = 5 / y; // BAD

void f() {
	int a = 5 / 0; // BAD
	int b = 5 / x; // BAD
	int c = x / 2;
	
	float f1 = x / 0.5;
	float f2 = x / (float)2;
	
	int n = 1234 / sizeof(x);
}
