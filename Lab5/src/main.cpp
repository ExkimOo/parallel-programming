//g++ -pthread -o main main.cpp
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

const int N = 10;
const int M = 5;

int pieces = M;

pthread_mutex_t eat = PTHREAD_MUTEX_INITIALIZER;

void* savage(void* arg);
void* cook(void* arg);

int main()
{
    pthread_t p_savages[N];

    int arg[N];
    for (int i = 0; i < N; i++)
    {
        arg[i] = i+1;
    }
    
    for (int i = 0; i < N; i++)
    {
        pthread_create(
            &p_savages[i],
            NULL,
            savage,
            (void*)&arg[i]
        );
    }

    pthread_t p_cook;
    pthread_create(
        &p_cook,
        NULL,
        cook,
        NULL
    );

    Sleep(1000000);
    system("pause");
    
    return 0;
}

void* savage(void* arg)
{
    int id = *(int*) arg;

    while(true)
    {
        pthread_mutex_lock(&eat);
            if (pieces != 0)
            {
                pieces--;

                Sleep(rand() % 500 + 200);
                std::cout << "A Savage #" << id << " eated 1 piece and now there's " << pieces << " pieces" << std::endl;
            }
        pthread_mutex_unlock(&eat);
        Sleep(rand() % 6000 + 500);
    }

    return NULL;
}

void* cook(void* arg)
{   
    while(true)
    {
        pthread_mutex_lock(&eat);
            if (pieces == 0)
            {
                Sleep(1000);
                std::cout << "------------------------------------" << std::endl;
                std::cout << "A Cook woke up" << std::endl;
                std::cout << "------------------------------------" << std::endl;
                
                //increase pieces
                pieces = M;

                Sleep(1000);
                std::cout << "Now there's " << pieces << " pieces" << std::endl;
                std::cout << "------------------------------------" << std::endl;
            }
        pthread_mutex_unlock(&eat);
    }
    
    return NULL;
}
