#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

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

int main(int argc, char* argv[])
{
    int N = 10;
    int M = 6;
    std::vector<Student> students;

    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);

    int avg_elems = N / size;
    int num_of_elems = rank == 0 ? N-(size-1)*avg_elems : avg_elems;
    int* data = new int [4*num_of_elems];

    for (int i = 0; i < num_of_elems; i++)
        {
            data[4*i] = (rank == 0 ? 0 : N+(rank-size)*avg_elems) + i + 1;
            data[4*i+1] = (int)rand() % 4 + 2;
            data[4*i+2] = (int)rand() % 4 + 2;
            data[4*i+3] = (int)rand() % 4 + 2;
        }

    if (rank != 0)
    {
        MPI_Send(data, 4*num_of_elems, MPI_INT, 0, 1, MPI_COMM_WORLD);    
    }

    if (rank == 0)
    {
        int *ints = new int [4*avg_elems];

        for (int i = 0; i < size-1; i++)
        {
            MPI_Recv(ints, 4*avg_elems, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, NULL);

            for (int i = 0; i < avg_elems; i++)
            {
                float avg = (float)(ints[4*i+1]+ints[4*i+2]+ints[4*i+3]) / 3;
                Student s = Student(ints[4*i], ints[4*i+1], ints[4*i+2], ints[4*i+3], avg);
                students.push_back(s);
            }
        }

        for (int i = 0; i < num_of_elems; i++)
        {
            float avg = (float)(data[4*i+1]+data[4*i+2]+data[4*i+3]) / 3;
            Student s = Student(data[4*i], data[4*i+1], data[4*i+2], data[4*i+3], avg);
            students.push_back(s);
        }

        std::cout << std::endl << "Students: " << std::endl;

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
    }

    MPI_Finalize();

    return 0;
}

