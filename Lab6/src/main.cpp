//g++ -pthread -o main main.cpp
#include <iostream>
#include <stdlib.h>
#include <pthread.h>

const int N = 10;
const int M = 5;

int pieces = M;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t output = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full;
pthread_cond_t not_empty;

void* savage(void* arg);
void* cook(void* arg);

int main()
{
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);
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
        pthread_mutex_lock(&mutex);
            while (pieces == 0) 
            {
                pthread_cond_wait(&not_full, &mutex);
            }
            pieces--;
            pthread_mutex_lock(&output);
                std::cout << "A Savage #" << id << " ate 1 piece" << std::endl;
            pthread_mutex_unlock(&output);
        pthread_mutex_unlock(&mutex);

        pthread_cond_broadcast(&not_empty);

        Sleep(1000);
    }

    return NULL;
}

void* cook(void* arg)
{   
    while(true)
    {
        pthread_mutex_lock(&mutex);
            while (pieces != 0) 
            {
                pthread_cond_wait(&not_empty, &mutex); 
            }
            pieces = M;
            pthread_mutex_lock(&output);
                std::cout << std::endl << "A Cook woke up and cooked " << M << " pieces" << std::endl << std::endl;
            pthread_mutex_unlock(&output);
        pthread_mutex_unlock(&mutex);

        pthread_cond_broadcast(&not_full) ;

        Sleep(1000);
    }

    return NULL;
}