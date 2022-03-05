from multiprocessing import Process
from random import randint
import numpy as np


def main():
    N = 3
    matrix = np.array([randint(1, 10) for _ in range(N ** 2)]).reshape(N, N)
    print('N:', N)
    print('Matrix:\n', matrix)

    processes = [None] * N
    for i in range(N):
        processes[i] = Process(target=calc_adjunct, args=(i, matrix))
        processes[i].start()

    for i in range(len(processes)):
        processes[i].join()


def calc_adjunct(num_proc, matrix):
    print(f'Process #{num_proc} ', end='')
    matrix = np.delete(matrix, 0, axis=0)
    matrix = np.delete(matrix, num_proc, axis=1)
    adjunct = (-1) ** (1 + (num_proc + 1)) * np.round(np.linalg.det(matrix))
    print('Adjunct:', adjunct)


if __name__ == '__main__':
    main()