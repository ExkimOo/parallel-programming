//g++ -pthread -o main main.cpp
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <numeric>

void *callback(void *arg);

std::vector<std::vector<float>> marks;

int main()
{
    srand(time(NULL));

    short N = 100;
    short M = 90;
    int arg[2] = {0, 1};

    std::cout << "N = " << N << std::endl;
    std::cout << "M = " << M << std::endl;

    for (int i = 0; i < N; i++)
    {
        std::vector<float> student_marks = {(float)(i + 1), (float)(rand() % 4 + 2), (float)(rand() % 4 + 2), (float)(rand() % 4 + 2)};
        marks.push_back(student_marks);
    }

    pthread_t p[2];

    for (int i = 0; i < 2; i++)
    {
        pthread_create(
            &p[i], 
            NULL, 
            callback, 
            (void*)&arg[i]);
    }

    for (std::vector<std::vector<float>>::iterator it = marks.begin(); it != marks.end() ; it++)
    {
        std::cout << (*it)[0] << ". " << (*it)[1]<< " " << (*it)[2] << " " << (*it)[3] << " " << std::endl;
    }

    for (int i = 0; i < 2; i++)
    {
        pthread_join(p[i], NULL);
    }

    std::cout << std::endl << "Students with avg mark: " << std::endl;

    for (int i = 0; i < marks.size(); i++)
    {
        std::cout << marks[i][0] << ". "<< marks[i][4] << std::endl;
    }

    //sorting by avg mark
    for (int i = 0; i < marks.size() - 1; i++)
    {
        for (int j = i + 1; j < marks.size(); j++)
        {
            if (marks[i][4] < marks[j][4])
            {
                swap(marks[i], marks[j]);
            }
        }
        
    }

    std::cout << std::endl << "Students with sorted avg mark: " << std::endl;
    
    for (int i = 0; i < marks.size(); i++)
    {
        std::cout << marks[i][0] << ". "<< marks[i][4] << std::endl;
    }
    
    std::cout << std::endl << "Chosen Students id:" << std::endl;

    for (int i = 0; i < M && marks[i][4] >= 3; i++)
    {
        std::cout << marks[i][0] << " ";
    }

    std::cout << std::endl;

    system("pause");

    return 0;
}

void *callback(void *arg)
{
    short N = *(int*)arg;

    short begin = N * 50;
    short end = N * 50 + 50;

    for (int i = begin; i < end; i++)
    {
        double avg_mark = (float)(std::accumulate(marks[i].begin() + 1, marks[i].end(), 0)) / 3;
        marks[i].push_back(avg_mark);
    }
    
    return NULL;
}