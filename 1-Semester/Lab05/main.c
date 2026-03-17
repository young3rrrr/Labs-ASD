#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 7
#define N 10

void sortEvenColumn(float A[M][N], int numRows, int colIndex)
{

    int limit = numRows - 1;

    while (1)
    {
        int LastSwapIndex = 0;
        int swapped = 0;
        for (int i = 0; i < limit - 1; i += 2)
        {
            if (A[i][colIndex] < A[i + 2][colIndex])
            {
                float temp = A[i][colIndex];
                A[i][colIndex] = A[i + 2][colIndex];
                A[i + 2][colIndex] = temp;

                swapped = 1;
                LastSwapIndex = i + 2;
            }
        }

        limit = LastSwapIndex;
        if (!swapped)
        {
            break;
        }
    }
}

int main()
{
    float A[M][N];
    srand(time(NULL));

    printf("Початкова матриця A[%d][%d]:\n", M, N);
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = (float)(rand() % 1500) / 100.0 - 5.0;
            printf("%5.2f ", A[i][j]);
        }
        printf("\n");
    }

    printf("\n--- Результати сортування (тільки парні позиції останнього стовпця) ---\n");

    sortEvenColumn(A, M, N - 1);

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%5.2f ", A[i][j]);
        }
        printf("\n");
    }
    return 0;
}