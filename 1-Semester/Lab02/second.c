#include <stdio.h>
#include <math.h>

int main()
{
    double n;
    printf("Enter n: ");
    scanf("%lf", &n);
    int func = 0;
    double sum = 0;
    double prod = 1;

    for (int i = 1; i <= n; i++)
    {
        sum += sin(i) + 1.0;
        prod *= sum / (i * (i + 1.0));
        func += 7;
    }

    printf("Result: %.7f\n", prod);
    printf("Function calls: %d\n", func);
    return 0;
}
