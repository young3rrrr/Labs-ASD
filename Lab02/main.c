#include <stdio.h>
#include <math.h>

int main() {
    double n;
    double sum = 0;
    double prod = 0;
    int func = 0;
    printf("Enter n: ");
    scanf("%lf", &n);

    for (int i = 1; i <= n; i++) {
        prod += i * sum;
        func += 2;
        for (int j = 1; j <= i; j++) {
            sum += (sin(j) + 1.0 / (i * (i + 1.0)));
            func += 6;
        }
    }
    printf("Result: %.1f\n", prod);
    printf("Function calls: %d\n", func);
    return 0;
}