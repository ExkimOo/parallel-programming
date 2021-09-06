//g++ -pthread -o main main.cpp
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <numeric>

const int N = 4;
const int M = 4;
int A[N][M] = {
        {0, 0, 0, 3}, 
        {0, 1, 2, 3}, 
        {3, 1, 2, 0}, 
        {0, 1, 0, 3}
    };

const int K = 3;
int B[K] = {1, 2, 3};

int check(int type);

int main()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "horizontal forward:\t" << check(0) << std::endl;
    std::cout << "horizontal backward:\t" << check(1) << std::endl;
    std::cout << "vertical forward:\t" << check(2) << std::endl;
    std::cout << "vertical backward:\t" << check(3) << std::endl;
    std::cout << "main diagonal forward:\t" << check(4) << std::endl;
    std::cout << "main diagonal backward:\t" << check(5) << std::endl;
    std::cout << "secondary diagonal f:\t" << check(6) << std::endl;
    std::cout << "secondary diagonal b:\t" << check(7) << std::endl;

    system("pause");

    return 0;
}

int check(int type)
{
    int count = 0;

    switch(type)
    {
        //horizontal forward
        case 0:
        {
            if (K <= M)
            {
                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < M - K + 1; j++)
                    {
                        int cnt = 0;

                        for (int k = 0; k < K; k++)
                        {
                            if (A[i][j+k] == B[k])
                            {
                                cnt++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (cnt == K)
                        {
                            count++;
                        }
                    }
                    
                }
            }
            return count;
            break;
        }
        //horizontal backward
        case 1:
        {
            if (K <= M)
            {
                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < M - K + 1; j++)
                    {
                        int cnt = 0;

                        for (int k = 0; k < K; k++)
                        {
                            if (A[i][j+(K-1)-k] == B[k])
                            {
                                cnt++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (cnt == K)
                        {
                            count++;
                        }
                    }
                    
                }
            }
            return count;
            break;
        }
        //vertical forward
        case 2:
        {
            if (K <= N)
            {
                for (int i = 0; i < N - K + 1; i++)
                {
                    for (int j = 0; j < M; j++)
                    {
                        int cnt = 0;

                        for (int k = 0; k < K; k++)
                        {
                            if (A[i+k][j] == B[k])
                            {
                                cnt++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (cnt == K)
                        {
                            count++;
                        }
                    }
                    
                }
            }
            return count;
            break;
        }
        //vertical backward
        case 3:
        {
            if (K <= N)
            {
                for (int i = 0; i < N - K + 1; i++)
                {
                    for (int j = 0; j < M; j++)
                    {
                        int cnt = 0;

                        for (int k = 0; k < K; k++)
                        {
                            if (A[i+(K-1)-k][j] == B[k])
                            {
                                cnt++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (cnt == K)
                        {
                            count++;
                        }
                    }
                    
                }
            }
            return count;
            break;
        }
        //main diagonal forward
        case 4:
        {
            if (K <= M and K <= N)
            {
                for (int i = 0; i < N - K + 1; i++)
                {
                    for (int j = 0; j < M - K + 1; j++)
                    {
                        int cnt = 0;

                        for (int k = 0; k < K; k++)
                        {
                            if (A[i+k][j+k] == B[k])
                            {
                                cnt++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (cnt == K)
                        {
                            count++;
                        }
                    }
                    
                }
            }
            return count;
            break;
        }
        //main diagonal backward
        case 5:
        {
            if (K <= M and K <= N)
            {
                for (int i = 0; i < N - K + 1; i++)
                {
                    for (int j = 0; j < M - K + 1; j++)
                    {
                        int cnt = 0;

                        for (int k = 0; k < K; k++)
                        {
                            if (A[i+(K-1)-k][j+(K-1)-k] == B[k])
                            {
                                cnt++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (cnt == K)
                        {
                            count++;
                        }
                    }
                    
                }
            }
            return count;
            break;
        }
        //secondary diaganal forward
        case 6:
        {
            if (K <= M and K <= N)
            {
                for (int i = N - K + 1; i >= 0; i--)
                {
                    for (int j = M - K + 1; j >= 0; j--)
                    {
                        int cnt = 0;

                        for (int k = 0; k < K; k++)
                        {
                            if (A[i+k][j+k] == B[k])
                            {
                                cnt++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (cnt == K)
                        {
                            count++;
                        }
                    }
                    
                }
            }
            return count;
            break;
        }
        //secondary diagonal backward
        case 7:
        {
            if (K <= M and K <= N)
            {
                for (int i = N - K + 1; i >= 0; i--)
                {
                    for (int j = 0; j < M - K + 1; j++)
                    {
                        int cnt = 0;

                        for (int k = 0; k < K; k++)
                        {
                            if (A[i+(K-1)-k][j+(K-1)-k] == B[k])
                            {
                                cnt++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (cnt == K)
                        {
                            count++;
                        }
                    }
                    
                }
            }
            return count;
            break;
        }
        default:
        {
            return 0;
        }
    }
}