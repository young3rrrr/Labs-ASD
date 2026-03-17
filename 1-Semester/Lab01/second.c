#include <stdio.h>

int x, y;

int main() {
    printf("Enter x: ");
    scanf("%i", &x);

    if (x == 2) {
        y = x * 3 + 14;
        printf("%i\n", y);
    }
    else if (x == 7) {
        y = x * 3 + 14;
        printf("%i\n", y);
    }
    else {
        if (x > 14) {
            y = -4 * x * 3 + (3 * x) - 7;
            printf("y = %i\n", y);
        }
        else {
            if (x >= -13) {
                if (x <= -3) {
                    y = -4 * x * 3 + (3 * x) - 7;
                    printf("y = %i\n", y);
                }
                else {
                    printf("Значення не підходить\n");
                }
            }
            else {
                printf("Значення не підходить\n");
            }
        }
    }

    return 0;
}
