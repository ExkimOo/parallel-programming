#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

__global__ void checkMagic(int* matrix, int N, int e);

int main()
{
    srand(time(NULL));

    int N = 0;
    int e = 0;

    std::cout << "Enter N: ";
    std::cin >> N;

    std::cout << "Enter e: ";
    std::cin >> e;

    int* matrix = new int[N * N];

    for (int i = 0; i < N * N; i++)
    {
        matrix[i] = rand() % 5;
    }

    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            std::cout << matrix[i*N+j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    int* m;
    cudaMalloc((void**)&m, (N * N) * sizeof(int));
    cudaMemcpy(m, matrix, (N * N) * sizeof(int), cudaMemcpyHostToDevice);

    checkMagic << < 1, 2 >> > (m, N, e);

    cudaFree(m);
    return 0;
}

__global__ void checkMagic(int* matrix, int N, int e)
{
    int idx = threadIdx.x;

    int* horSums = new int[N];
    int* verSums = new int[N];

    if (idx == 0)
    {
        for (int i = 0; i < N; i++)
        {
            int sum = 0;
            for (int j = 0; j < N; j++)
            {
                sum += matrix[i * N + j];
            }
            horSums[i] = sum;
        }

        printf("Horizontal sums: ");
        for (int i = 0; i < N; i++)
        {
            printf("%d ", horSums[i]);
        }
        printf("\n");
    }

    if (idx == 1)
    {
        for (int i = 0; i < N; i++)
        {
            int sum = 0;
            for (int j = 0; j < N; j++)
            {
                sum += matrix[j * N + i];
            }
            verSums[i] = sum;
        }

        printf("Vertical sums: ");
        for (int i = 0; i < N; i++)
        {
            printf("%d ", verSums[i]);
        }
        printf("\n");
    }

    __syncthreads();

    if (idx == 0)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (!(horSums[i] - verSums[j] < e))
                {
                    printf("Not magic");
                    return;
                }
            }
        }
        printf("Magic");
    }
}