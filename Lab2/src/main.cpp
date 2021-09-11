//g++ -pthread -o main main.cpp
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

const int N = 5;
const int M = 5;
int A[N][M];

const int K = 2;
int B[K];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

std::vector<std::string> queue;

void* check(void* _action);


int main()
{
    srand(time(NULL));

    std::vector<std::string> output = {
        "Horizontal forward:\t", 
        "Horizontal backward:\t", 
        "Vertical forward:\t",
        "Vertical backward:\t",
        "Main diagonal forward:\t",
        "Main diagonal backward:\t",
        "Secondary diagonal f:\t",
        "Secondary diagonal b:\t"
        };

    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            A[i][j] = rand() % 3;
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "Array:" << std::endl;
    for (int i = 0; i < K; i++)
    {
        B[i] = rand() % 3;
        std::cout << B[i] << " ";
    }
    std::cout << std::endl << std::endl;
    
    int arg[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    pthread_t p[8];

    for (int i = 0; i < 8; i++)
    {
        pthread_create(
            &p[i], 
            NULL, 
            check, 
            (void*)&arg[i]);
    }

    void* result[8];
    for (int i = 0; i < 8; i++)
    {  
        pthread_join(p[i], &result[i]);
        std::cout << output[i] << (size_t)(off_t)result[i] << std::endl;
    }

    std::cout << std::endl;
    for (int i = 0; i < queue.size(); i++)
    {
        std::cout << queue[i] << std::endl;
    }
    

    system("pause");

    return 0;
}

void* check(void* _action)
{
    int count = 0;
    int action = *(int*)_action;

    switch(action)
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
                        for (int k = 0; k < K; k++)
                        {
                            pthread_mutex_lock(&mutex);
                            if (A[i][j+k] == B[k])
                            {
                                pthread_mutex_unlock(&mutex);
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                pthread_mutex_unlock(&mutex);
                                break;
                            }
                        }
                    }
                    
                }
            }
            pthread_mutex_lock(&mutex);
            queue.push_back("Thread with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            pthread_mutex_unlock(&mutex);
            return (void*)count;
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
                        for (int k = 0; k < K; k++)
                        {
                            pthread_mutex_lock(&mutex);
                            if (A[i][j+(K-1)-k] == B[k])
                            {
                                pthread_mutex_unlock(&mutex);
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                pthread_mutex_unlock(&mutex);
                                break;
                            }
                        }
                    }
                    
                }
            }
            pthread_mutex_lock(&mutex);
            queue.push_back("Thread with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            pthread_mutex_unlock(&mutex);
            return (void*)count;
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
                        for (int k = 0; k < K; k++)
                        {
                            pthread_mutex_lock(&mutex);
                            if (A[i+k][j] == B[k])
                            {
                                pthread_mutex_unlock(&mutex);
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                pthread_mutex_unlock(&mutex);
                                break;
                            }
                        }
                    }
                    
                }
            }
            pthread_mutex_lock(&mutex);
            queue.push_back("Thread with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            pthread_mutex_unlock(&mutex);
            return (void*)count;
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
                        for (int k = 0; k < K; k++)
                        {
                            pthread_mutex_lock(&mutex);
                            if (A[i+(K-1)-k][j] == B[k])
                            {
                                pthread_mutex_unlock(&mutex);
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                pthread_mutex_unlock(&mutex);
                                break;
                            }
                        }
                    }
                    
                }
            }
            pthread_mutex_lock(&mutex);
            queue.push_back("Thread with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            pthread_mutex_unlock(&mutex);
            return (void*)count;
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
                        for (int k = 0; k < K; k++)
                        {
                            pthread_mutex_lock(&mutex);
                            if (A[i+k][j+k] == B[k])
                            {
                                pthread_mutex_unlock(&mutex);
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                pthread_mutex_unlock(&mutex);
                                break;
                            }
                        }
                    }
                }
            }
            pthread_mutex_lock(&mutex);
            queue.push_back("Thread with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            pthread_mutex_unlock(&mutex);
            return (void*)count;
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
                        for (int k = 0; k < K; k++)
                        {
                            pthread_mutex_lock(&mutex);
                            // if (A[i+(K-1)-k][j+(K-1)-k] == B[k])
                            if (A[i+k][j+k] == B[(K-1)-k])
                            {
                                pthread_mutex_unlock(&mutex);
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                pthread_mutex_unlock(&mutex);
                                break;
                            }
                        }
                    }
                    
                }
            }
            pthread_mutex_lock(&mutex);
            queue.push_back("Thread with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            pthread_mutex_unlock(&mutex);
            return (void*)count;
        }
        //secondary diaganal forward
        case 6:
        {
            if (K <= M and K <= N)
            {
                for (int i = N - 1; i >= K - 1; i--)
                {
                    for (int j = 0; j < M - K + 1; j++)
                    {
                        for (int k = 0; k < K; k++)
                        {
                            pthread_mutex_lock(&mutex);
                            if (A[i-k][j+k] == B[k])
                            {
                                pthread_mutex_unlock(&mutex);
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                pthread_mutex_unlock(&mutex);
                                break;
                            }
                        }
                    }
                    
                }
            }
            pthread_mutex_lock(&mutex);
            queue.push_back("Thread with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            pthread_mutex_unlock(&mutex);
            return (void*)count;
        }
        //secondary diagonal backward
        case 7:
        {
            if (K <= M and K <= N)
            {
                for (int i = N - 1; i >= K - 1; i--)
                {
                    for (int j = 0; j < M - K + 1; j++)
                    {
                        for (int k = 0; k < K; k++)
                        {   
                            // std::cout << "A[" << i+(K-2)+k << "][" << j+(K-2)-k << "] = " << A[i+(K-2)+k][j+(K-2)-k] << std::endl;
                            // std::cout << i-k << " " << j+k << std::endl;
                            pthread_mutex_lock(&mutex);
                            if (A[i-k][j+k] == B[(K-1)-k])
                            {
                                pthread_mutex_unlock(&mutex);
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                pthread_mutex_unlock(&mutex);
                                break;
                            }
                        }
                    }
                    
                }
            }
            pthread_mutex_lock(&mutex);
            queue.push_back("Thread with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            pthread_mutex_unlock(&mutex);
            return (void*)count;
        }
        default:
        {
            return 0;
        }
    }
}