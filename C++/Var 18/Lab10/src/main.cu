#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>

#define N 10
#define K 5

__global__ void solve(int* arr, int* result);
void print_arrays(int* A_B);
void initialize_arrays(int* A_B);


int main()
{
    srand(time(NULL));

    int A_B[N + K];
    int result[K];
    
    int* A;
    int* R;

    cudaMalloc((void**)&A, (N+K)*sizeof(int));
    cudaMalloc((void**)&R, K * sizeof(int));

    initialize_arrays(A_B);
    print_arrays(A_B);

    cudaMemcpy(A, A_B, (N + K) * sizeof(int), cudaMemcpyHostToDevice);

    solve << < 1, 2 >> > (A, R);

    cudaMemcpy(result, R, K * sizeof(int), cudaMemcpyDeviceToHost);
    
    for (int i = 0; i < K; i++)
    {
        std::cout << result[i] << " ";
    }

    cudaFree(A);
    cudaFree(R);

    return 0;
}

__global__ void solve(int* arr, int* result)
{
    int idx = threadIdx.x;

    if (idx == 0)
    {
        for (int i = 0; i < N - 1; i++)
        {
            for (int j = i + 1; j < N; j++)
            {
                if (arr[i] < arr[j])
                {
                    int tmp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = tmp;
                }
            }
        }
    }

    if (idx == 1)
    {
        for (int i = N; i < N + K - 1; i++)
        {
            for (int j = i + 1; j < N + K; j++)
            {
                if (arr[i] < arr[j])
                {
                    int tmp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = tmp;
                }
            }
        }
    }

    __syncthreads();

    for (int i = idx; i < K; i += 2)
    {
        result[i] = arr[i] * arr[N + i];
    }
}

void initialize_arrays(int* A_B)
{
    for (int i = 0; i < N + K; i++)
    {
        A_B[i] = rand() % 10;
    }
}

void print_arrays(int* A_B)
{
    std::cout << "Array A:" << std::endl;
    for (int i = 0; i < N; i++)
    {
        std::cout << A_B[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Array B:" << std::endl;
    for (int i = 0; i < K; i++)
    {
        std::cout << A_B[N + i] << " ";
    }
    std::cout << std::endl;
}