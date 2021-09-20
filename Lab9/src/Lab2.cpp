#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>


const int N = 5;
const int M = 5;
const int K = 3;

std::vector<std::string> queue;

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

int check(int** A, int* B, int action);
int** create_matrix(int N, int M);
void print_matrix(int** A, int N, int M);
void delete_matrix(int** A, int N);
int* create_array(int K);
void print_array(int* B, int K);
void delete_array(int* B);


int main(int argc, char* argv[])
{
    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int** A;
    int* B;
    int count;

    srand(clock_gettime(2, CLOCK_REALTIME));
         
    if (rank == 0)
    {
        A = create_matrix(N, M);
        print_matrix(A, N, M);

        B = create_array(K);
        print_array(B, K);

        for (int i = 1; i < size; i++)
        {
            MPI_Send(&(A[0][0]), N*M, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&(B[0]), K, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    }

    if (rank != 0)
    {
        A = new int* [N];
        for (int i = 0; i < N; i++)
        {
            A[i] = new int [M];
        }

        B = new int [K];

        MPI_Recv(&(A[0][0]), N*M, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, NULL);
        MPI_Recv(&(B[0]), K, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, NULL);
        
        print_matrix(A, N, M);
        check(A, B, rank);
        delete_matrix(A, N);
        delete_array(B);
    }

    if (rank == 0)
    {
        check(A, B, rank);
        delete_matrix(A, M);
        delete_array(B);

        std::cout << std::endl;
        for (int i = 0; i < queue.size(); i++)
        {
            std::cout << queue[i] << std::endl;
        }
    }

    MPI_Finalize();    

    return 0;
}

int** create_matrix(int N, int M)
{
    int** A = new int* [N];
    for (int i = 0; i < N; i++)
    {
        A[i] = new int [M];
    }
    
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            A[i][j] = rand() % 3;
        }
    }

    return A;
}

void print_matrix(int** A, int N, int M)
{
    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
    // std::cout << std::endl << std::endl;
}

void delete_matrix(int** A, int N)
{
    for (int i = 0; i < N; i++)
    {
        delete[] A[i];
    }
    delete[] A;
}

int* create_array(int K)
{
    int* B = new int [K];

    for (int i = 0; i < K; i++)
    {
        B[i] = rand() % 3;
    }    

    return B;
}

void print_array(int* B, int K)
{
    std::cout << std::endl << "Array:" << std::endl;
    for (int i = 0; i < K; i++)
    {
        std::cout << B[i] << " ";
    }
    std::cout << std::endl;
}

void delete_array(int* B)
{
    delete[] B;
}

int check(int** A, int* B, int action)
{
    int count = 0;

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
                            if (A[i][j+k] == B[k])
                            {
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    
                }
            }
            queue.push_back("Process with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            return count;
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
                            if (A[i][j+(K-1)-k] == B[k])
                            {
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    
                }
            }
            queue.push_back("Process with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            return count;
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
                            if (A[i+k][j] == B[k])
                            {
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    
                }
            }
            queue.push_back("Process with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            return count;
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
                            if (A[i+(K-1)-k][j] == B[k])
                            {
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    
                }
            }
            queue.push_back("Process with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            return count;
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
                            if (A[i+k][j+k] == B[k])
                            {
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
            }
            queue.push_back("Process with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            return count;
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
                            // if (A[i+(K-1)-k][j+(K-1)-k] == B[k])
                            if (A[i+k][j+k] == B[(K-1)-k])
                            {
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    
                }
            }
            queue.push_back("Process with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            return count;
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
                            if (A[i-k][j+k] == B[k])
                            {
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    
                }
            }
            queue.push_back("Process with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            return count;
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
                            if (A[i-k][j+k] == B[(K-1)-k])
                            {
                                if (k == K - 1)
                                {
                                    count++;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    
                }
            }
            queue.push_back("Process with id " + std::to_string(action) + " found its number of occurrence: " + std::to_string(count));
            return count;
        }
        default:
        {
            return 0;
        }
    }
}