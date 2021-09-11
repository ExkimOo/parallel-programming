//g++ -pthread -o main main.cpp
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

const int N = 10;
const int M = 5;

int pieces = M;

pthread_mutex_t eat = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;

void* savage(void* arg);
void* cook(void* arg);

int main()
{
    sem_init(&sem, 0, M);
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

    Sleep(60000);
    system("pause");
    
    return 0;
}

void* savage(void* arg)
{
    int id = *(int*) arg;

    while(true)
    {
        pthread_mutex_lock(&eat);

            int residue;
            sem_getvalue(&sem, &residue);

            if (residue == 0)
            {
                std::cout << "------------------------------------" << std::endl;
                std::cout << "There's no piece for a Savage #" << id << std::endl;
                std::cout << "------------------------------------" << std::endl;
                cook(NULL);
            }
        
            sem_wait(&sem);
            sem_getvalue(&sem, &residue);
            std::cout << "A Savage #" << id << " eated 1 piece and now there's " << residue << " pieces" << std::endl;
            Sleep(1000);

        pthread_mutex_unlock(&eat);
        Sleep(100);
    }

    return NULL;
}

void* cook(void* arg)
{   
    int residue;
    sem_getvalue(&sem, &residue);
    if (residue == 0)
    {
        Sleep(1000);
        std::cout << "A Cook woke up" << std::endl;
        std::cout << "------------------------------------" << std::endl;
        Sleep(1000);

        for (int i = 0; i < M; i++)
        {
            sem_post(&sem);
        }
        
        std::cout << "Now there's " << pieces << " pieces" << std::endl;
        std::cout << "------------------------------------" << std::endl;
        Sleep(1000);
    }
    
    return NULL;
}
