#include <stdio.h>
#include <math.h>

int main()
{
    double n;
    printf("Enter n: ");
    scanf("%lf", &n);
    int func = 0;
    double prod = 1;
    for (int i = 1; i <= n; i++)
    {
        double sum = 0;
        for (int j = 1; j <= i; j++)
        {
            sum += (sin(j) + 1.0);
            func += 3;
        }
        prod *= sum / (i * (i + 1.0));
        func += 4;
    }
    printf("Result: %.7f\n", prod);
    printf("Function calls: %d\n", func);
    return 0;
}
