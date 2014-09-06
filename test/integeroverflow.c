
#define MAX_INT   0x7FFFFFFF

int safesum(int sum, int a) {
    // If a is negative there is integer overflow
    if (sum < MAX_INT - a)  // BAD
        sum += a;
    else
        sum = MAX_INT;
    return sum;
}
