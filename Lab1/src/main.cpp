//g++ -pthread -o main main.cpp
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <numeric>

void *callback(void *args);

class Student
{
    public:
        int id;
        int physics;
        int math;
        int informatics;
        float avg;
    
        Student();
        friend std::ostream& operator<<(std::ostream& os, const Student& s);

        Student(int _id, int _physics, int _math, int _informatics, float _avg)
        {
            id = _id;
            physics = _physics;
            math = _math;
            informatics = _informatics;
            avg = _avg;
        }

        friend std::ostream& operator<<(std::ostream& os, const Student& s)
        {
            os << s.id << ".\t" << s.physics << "\t" << s.math << "\t" << s.informatics << "\tavg: " << s.avg;
            return os;
        }

};

std::vector<Student> students;

// struct arg_struct {
//     int N;
//     int M;
//     int arg[1];
//     std::vector<Student> students;
// };

int main()
{
    srand(time(NULL));

    // arg_struct args;
    int N = 10;
    int M = 6;
    int arg[2] = {0, 1};
    // std::cout << "arg " << &args.students[0].avg << std::endl;
    // std::cout << "123 " << &args.students << std::endl;
    // std::cout << "N = " << args.N << std::endl;
    // std::cout << "M = " << args.M << std::endl;

    //generating random marks
    for (int i = 0; i < N; i++)
    {
        Student s = Student(i, (int)rand() % 4 + 2, (int)rand() % 4 + 2, (int)rand() % 4 + 2, .0);
        students.push_back(s);
    }

    for (int i = 0; i < students.size(); i++)
    {
        std::cout <<  students[i] << std::endl;
    }

    pthread_t p[2];

    for (int i = 0; i < 2; i++)
    {
        pthread_create(
            &p[i], 
            NULL, 
            callback, 
            (void*)&arg);
    }

    for (int i = 0; i < 2; i++)
    {
        pthread_join(p[i], NULL);
    }

    std::cout << std::endl << "Students with avg mark: " << std::endl;

    for (int i = 0; i < students.size(); i++)
    {
        std::cout << students[i] << std::endl;
    }

    //sorting by avg mark
    for (int i = 0; i < students.size() - 1; i++)
    {
        for (int j = i + 1; j < students.size(); j++)
        {
            if (students[i].avg < students[j].avg)
            {
                std::swap(students[i], students[j]);
            }
        }
        
    }

    std::cout << std::endl << "Students with sorted avg mark: " << std::endl;
    
    for (int i = 0; i < students.size(); i++)
    {
        std::cout << students[i] << std::endl;
    }
    
    std::cout << std::endl << "Chosen Students id:" << std::endl;

    for (int i = 0; i < M && students[i].avg >= 3; i++)
    {
        std::cout << students[i].id << " ";
    }

    std::cout << std::endl;

    system("pause");

    return 0;
}

void *callback(void *_arg){
    int N = *(int*)_arg;
    // std::cout << "123 " << &args->students[0].avg << std::endl;
    // std::cout << "123 " << &args->students << std::endl;
    int begin = N * 5;
    int end = N * 5 + 10;
    // std::cout << "N = " << args->N << std::endl;

    for (int i = begin; i < end; i++)
    {
        float avg_mark = (float)(students[i].math + students[i].physics + students[i].informatics) / 3;
        // std::cout << (*args).students[i].math << " " << args->students[i].physics << " " << args->students[i].informatics << std::endl;
        students[i].avg = avg_mark;
    }
    
    return NULL;
}