#include <stdio.h>
#include <math.h>

int main() {
    double n;
    double sum = 0;
    double prod = 0;
    
    printf("Enter n: ");
    scanf("%lf", &n);

    for (int i = 1; i <= n; i++) {
        prod += i * sum;
        for (int j = 1; j <= i; j++) {
            sum += (sin(j) + 1.0 / (i * (i + 1.0)));
        }
    }
    printf("Result: %.1f\n", prod);
    return 0;
}