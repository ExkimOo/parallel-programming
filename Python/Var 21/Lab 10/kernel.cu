#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

__global__ void createMatrix(double* matrix, int stepsX, double x1, double y1, double h);
double checkInput();

int main()
{
    double x1, y1, x2, y2, h;
    std::cout << "x1: ";
    x1 = checkInput();
    std::cout << "x2: ";
    x2 = checkInput();
    std::cout << "y1: ";
    y1 = checkInput();
    std::cout << "y2: ";
    y2 = checkInput();
    std::cout << "Step h: ";
    h = checkInput();

    if (x1 > x2)
    {
        std::swap(x1, x2);
    }

    if (y1 > y2)
    {
        std::swap(y1, y2);
    }

    int stepsX = int((x2 - x1) / h) + 1;
    int stepsY = int((y2 - y1) / h) + 1;

    double* matrix = new double[stepsX * stepsY];

    double x = x1;
    double y = y1;

    double* m;
    cudaMalloc((void**)&m, (stepsX * stepsY) * sizeof(double));

    createMatrix << < 1, stepsX * stepsY >> > (m, stepsX, x1, y1, h);

    cudaMemcpy(matrix, m, (stepsX * stepsY) * sizeof(double), cudaMemcpyDeviceToHost);

    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < stepsX; i++)
    {
        for (int j = 0; j < stepsX; j++)
        {
            std::cout << matrix[i * stepsX + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    cudaFree(m);
    return 0;
}

__global__ void createMatrix(double* m, int stepsX, double x1, double y1, double h)
{
    int idx = threadIdx.x;
    double x = x1 + (idx / stepsX) * h;
    double y = y1 + (idx % stepsX) * h;

    m[idx] = (y - x) * y * cosf(x);
}

double checkInput() 
{
    double a = 0;
    do
    {
        if (!std::cin)
        {
            std::cin.clear();
            while (std::cin.get() != '\n');
            std::cout << "Wrong value, try again\n";
        }
        std::cin >> a;
    } while (!std::cin);

    return a;
}