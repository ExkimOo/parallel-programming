//g++ -pthread -o main main.cpp
#include <iostream>
#include <list>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>


void* change(void* arg);
void* read(void* arg);
void* count(void* arg);
void* add(void* arg);
void* del(void* arg);
void random();

std::list<int> lst(10, 0);

pthread_rwlock_t rwlock;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    pthread_rwlock_init(&rwlock, NULL);
    
    pthread_t p[5];
    pthread_create(
        &p[0],
        NULL,
        change,
        NULL
    );
    pthread_create(
        &p[1],
        NULL,
        read,
        NULL
    );
    pthread_create(
        &p[2],
        NULL,
        count,
        NULL
    );
    pthread_create(
        &p[3],
        NULL,
        add,
        NULL
    );
    pthread_create(
        &p[4],
        NULL,
        del,
        NULL
    );

    // while (true)
    // {   
    //     pthread_mutex_lock(&mutex);
    //         std::cout << "List:" << std::endl;
    //         for (auto it = lst.begin(); it != lst.end(); it++)
    //         {
    //             std::cout << *it << " ";
    //         }
    //         std::cout << std::endl << std::endl;
    //     pthread_mutex_unlock(&mutex);

    //     Sleep(2000);
    // }
    
    Sleep(1000000);
    system("pause");

    return 0;
}


void* change(void* arg)
{
    random();
    while(true)
    {
        // int index = rand() % lst.size();
        int val = rand() % 10;
        
        pthread_rwlock_rdlock(&rwlock);
            std::list<int>::iterator elem = lst.begin();
        pthread_rwlock_unlock(&rwlock);
        std::advance(elem, rand() % lst.size());
        pthread_rwlock_wrlock(&rwlock);
            *elem = val;
        pthread_rwlock_unlock(&rwlock);

        // std::cout << "Element #" << index << " changed its value on " << val << std::endl;

        Sleep(2000);
    }
    
    return NULL;
}

void* read(void* arg)
{
    random();
    while(true)
    {
        int index = rand() % lst.size();

        pthread_rwlock_rdlock(&rwlock);
            std::list<int>::iterator elem = lst.begin();
        pthread_rwlock_unlock(&rwlock);
        std::advance(elem, index);
        pthread_rwlock_rdlock(&rwlock);
            int val = *elem;
        pthread_rwlock_unlock(&rwlock);

        pthread_mutex_lock(&mutex);
            std::cout << "Element #" << index << " was read and its value is " << val << std::endl;
        pthread_mutex_unlock(&mutex); 
        
        Sleep(2000);
    }
    
    return NULL;
}

void* count(void* arg)
{
    random();
    while(true)
    {
        pthread_rwlock_rdlock(&rwlock);
            pthread_mutex_lock(&mutex);
                std::cout << "Size of the list: " << lst.size() << std::endl;
            pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);

        Sleep(2000); 
    }

    return NULL;
}

void* add(void* arg)
{
    random();
    while(true)
    {
        bool position = (bool)(rand() % 2);
        int val = rand() % 10;
        
        pthread_rwlock_wrlock(&rwlock);
            lst.insert(position ? lst.begin() : lst.end(), val);
        pthread_rwlock_unlock(&rwlock);

        Sleep(2000);
    }

    return NULL;
}

void* del(void* arg)
{
    random();
    while (true)
    {   
        pthread_rwlock_rdlock(&rwlock);
            int size = lst.size();
        pthread_rwlock_unlock(&rwlock);
        if (size != 0)
        {
            int index = rand() % lst.size();

            pthread_rwlock_rdlock(&rwlock);
                std::list<int>::iterator elem = lst.begin();
            pthread_rwlock_unlock(&rwlock);
            std::advance(elem, index);
            pthread_rwlock_wrlock(&rwlock);
                lst.erase(elem);
            pthread_rwlock_unlock(&rwlock);
        }

        Sleep(3000);
    }

    return NULL;
}

void random()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    srand((unsigned)li.QuadPart);
}