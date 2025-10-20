#include <stdio.h>

int x, y;

int main() {
    printf("Enter x: ");
    scanf("%i", &x);

    if ((x == 0 || x == 8) || (x >= 16)) {
        y = -3 * x * x / 9 + 9;
        printf("%i\n", y);
    }
    else if ( x <= -1) {
        y = 15 * x - 2;
        printf("y = %i\n", y);

    }
    else
        printf("Значення не підходить\n");

    return 0;
}