#include <stdio.h>
#include <math.h>

int main() {
    double n;
    double sum = 0;
    double prod = 0;
    double sin_sum = 0;
    
    printf("Enter n: ");
    scanf("%lf", &n);

    for (int i = 1; i <= n; i++) {
        prod += i * sum;
        sin_sum += sin(i);
        sum += sin_sum + (1.0 / (i + 1.0));
    }
    
    printf("Result: %.1f\n", prod);
    return 0;
}