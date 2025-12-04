#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 7
#define N 10

void sortRowAscending(float row[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (row[j] > row[j + 1])
            {
                float temp = row[j];
                row[j] = row[j + 1];
                row[j + 1] = temp;
            }
        }
    }
}

int binarySearchAlgorithm2(float row[], int size, float min_val)
{
    int left = 0;
    int right = size - 1;

    while (left < right)
    {
        int mid = left + (right - left) / 2;
        if (row[mid] < min_val)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }

    if (row[left] >= min_val && row[left] <= 5.0)
    {
        return left;
    }

    return -1;
}

int main()
{
    float A[M][N];
    srand(time(NULL));

    printf("Матриця A[%d][%d] (відсортована за незменшенням):\n", M, N);

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = (float)(rand() % 1500) / 100.0 - 5.0;
        }
        sortRowAscending(A[i], N);

        for (int j = 0; j < N; j++)
        {
            printf("%5.2f ", A[i][j]);
        }
        printf("\n");
    }

    printf("\n--- Результати пошуку (Діапазон [0, 5]) ---\n");

    int foundTotal = 0;
    for (int i = 0; i < M; i++)
    {
        int idx = binarySearchAlgorithm2(A[i], N, 0.0);

        if (idx != -1)
        {
            printf("Рядок %d: ", i);
            for (int k = idx; k < N; k++)
            {
                if (A[i][k] <= 5.0)
                {
                    printf("[%.2f - %d] ", A[i][k], k);
                    foundTotal++;
                }
                else
                {
                    break;
                }
            }
            printf("\n");
        }
        else
        {
            printf("Рядок %d: Елементів з діапазону [0, 5] не знайдено.\n", i);
        }
    }

    if (foundTotal == 0)
        printf("У матриці немає шуканих чисел.\n");

    return 0;
}