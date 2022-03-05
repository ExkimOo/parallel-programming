#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

__global__ void calc(int* matrix, int N, int M);

int main()
{
    srand(time(NULL));

    int N = 4;
    int M = 6;


    int* matrix = new int[N * M];

    for (int i = 0; i < N * M; i++)
    {
        matrix[i] = 1 + rand() % 5;
    }

    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            std::cout << matrix[i * M + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    int* m;
    cudaMalloc((void**)&m, (N * M) * sizeof(int));
    cudaMemcpy(m, matrix, (N * M) * sizeof(int), cudaMemcpyHostToDevice);

    calc << < 1, 2 >> > (m, N, M);

    cudaFree(m);
    return 0;
}

__global__ void calc(int* matrix, int N, int M)
{
    int idx = threadIdx.x;

    int* rowSums = new int[N];
    int* colMuls = new int[M];

    if (idx == 0)
    {
        for (int i = 0; i < N; i++)
        {
            int sum = 0;
            for (int j = 0; j < M; j++)
            {
                sum += matrix[i * M + j];
            }
            rowSums[i] = sum;
        }

        printf("Rows sums: ");
        for (int i = 0; i < N; i++)
        {
            printf("%d ", rowSums[i]);
        }
        printf("\n");
    }

    if (idx == 1)
    {
        for (int i = 0; i < M; i++)
        {
            int mul = 1;
            for (int j = 0; j < N; j++)
            {
                mul *= matrix[j * M + i];
            }
            colMuls[i] = mul;
        }

        printf("Columns multiplications: ");
        for (int i = 0; i < M; i++)
        {
            printf("%d ", colMuls[i]);
        }
        printf("\n");
    }
}