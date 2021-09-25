#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>


int check(int* A, int* B, int action);
int* create_matrix(int N, int M);
void print_matrix(int* A, int N, int M);
int* create_array(int K);
void print_array(int* B, int K);

const int N = 5;
const int M = 5;
const int K = 3;

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


int main(int argc, char* argv[])
{
    int rank;
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* A;
    int* B;
    int count;

    srand(time(NULL) + rank);

    if (rank == 0)
    {
        A = create_matrix(N, M);
        print_matrix(A, N, M);

        B = create_array(K);
        print_array(B, K);

        for (int i = 1; i < size; i++)
        {
            MPI_Send(&A[0], N*M, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&B[0], K, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    }

    if (rank != 0)
    {
        A = new int [N*M];

        B = new int [K];

        MPI_Recv(&A[0], N*M, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, NULL);
        MPI_Recv(&B[0], K, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, NULL);
        
        check(A, B, rank);
        check(A, B, rank + 4);
        delete[] A;
        delete[] B;
    }

    if (rank == 0)
    {
        check(A, B, rank);
        check(A, B, size);
        delete[] A;
        delete[] B;
    }

    MPI_Finalize();    

    return 0;
}

int* create_matrix(int N, int M)
{
    int* A = new int [N*M];
    
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            A[N*i+j] = rand() % 3;
        }
    }

    return A;
}

void print_matrix(int* A, int N, int M)
{
    std::cout << "Matrix:" << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            std::cout << A[N*i+j] << " ";
        }
        std::cout << std::endl;
    }
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
    std::cout << std::endl << std::endl;
}

int check(int* A, int* B, int action)
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
                            if (A[i*N+j+k] == B[k])
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
            std::cout << "Horizontal forward:\t" << count << std::endl;
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
                            if (A[i*N+j+(K-1)-k] == B[k])
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
            std::cout << "Horizontal backward:\t" << count << std::endl;
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
                            if (A[N*(i+k)+j] == B[k])
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
            std::cout << "Vertical forward:\t" << count << std::endl;
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
                            if (A[N*(i+(K-1)-k)+j] == B[k])
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
            std::cout << "Vertical backward:\t" << count << std::endl;
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
                            if (A[N*(i+k)+j+k] == B[k])
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
            std::cout << "Main diagonal forward:\t" << count << std::endl;
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
                            if (A[N*(i+k)+j+k] == B[(K-1)-k])
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
            std::cout << "Main diagonal backward:\t" << count << std::endl;
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
                            if (A[N*(i-k)+j+k] == B[k])
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
            std::cout << "Secondary diagonal f:\t" << count << std::endl;
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
                            if (A[N*(i-k)+j+k] == B[(K-1)-k])
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
            std::cout << "Secondary diagonal b:\t" << count << std::endl;
            return count;
        }
        default:
        {
            return 0;
        }
    }
}