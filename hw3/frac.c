#include <stdio.h>
#include <string.h>

// get Greatest Common Divisor
int gcd(int a, int b) {
    int c, m, n;
    if (a > b) {
        m = a; n = b;
    } else {
        m = b; n = a;
    }
    while (n != 0) {
        c = m % n;
        m = n;
        n = c;
    }
    return m;
}

typedef struct fraction {
    int numerator;
    int denominator;
} fraction;

fraction add(fraction a, fraction b) {
    fraction c;
    c.numerator = a.numerator * b.denominator + a.denominator * b.numerator;
    c.denominator = a.denominator * b.denominator;
    return c;
}

fraction minus(fraction a, fraction b) {
    fraction c;
    c.numerator = a.numerator * b.denominator - a.denominator * b.numerator;
    c.denominator = a.denominator * b.denominator;
    return c;    
}

fraction multiply(fraction a, fraction b) {
    fraction c;
    c.numerator = a.numerator * b.numerator;
    c.denominator = a.denominator * b.denominator;
    return c;    
}

fraction divide(fraction a, fraction b) {
    fraction c;
    c.numerator = a.numerator * b.denominator;
    c.denominator = a.denominator * b.numerator;
    return c;    
}

fraction reduce(fraction complex) {
    fraction simple;
    int g;
    g = gcd(complex.numerator, complex.denominator);
    simple.numerator = complex.numerator / g;
    simple.denominator = complex.denominator / g;
    return simple;
}

float frac_to_float(fraction a) {
    float b;
    b = float(a.numerator) / float(a.denominator);
    return b;
}

void print_frac(fraction a) {
    printf("Value: %d / %d\n\n", a.numerator, a.denominator);
}

int main() {
    fraction a;
    a.numerator = 3;
    a.denominator = 5;
    printf("Fraction a: \n");
    print_frac(a);

    fraction b;
    b.numerator = 7;
    b.denominator = 10;  
    printf("Fraction b: \n");
    print_frac(b);  

    fraction re_add;
    re_add = add(a, b);
    printf("Fraction a + b: \n");
    print_frac(re_add);

    fraction re_minus;
    re_minus = minus(a, b);
    printf("Fraction a - b: \n");
    print_frac(re_minus);

    fraction re_multiply;
    re_multiply = multiply(a, b);
    printf("Fraction a * b: \n");
    print_frac(re_multiply);

    fraction re_divide;
    re_divide = divide(a, b);
    printf("Fraction a / b: \n");
    print_frac(re_divide);

    fraction re_simple;
    re_simple = reduce(re_divide);
    printf("After Reducing: \n");
    print_frac(re_simple);

    float f;
    f = frac_to_float(re_simple);
    printf("Transform to Float Format: \n");
    printf("Float value: %f\n", f);
    
    return 0;
}