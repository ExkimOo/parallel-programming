from threading import Thread
from random import randint
import numpy as np


def main():
    N = 3
    matrix = np.array([randint(1, 10) for _ in range(N ** 2)]).reshape(N, N)
    print('N:', N)
    print('Matrix:\n', matrix)

    threads = [None] * N
    for i in range(N):
        threads[i] = Thread(target=calc_adjunct, args=(i, matrix))
        threads[i].start()

    for i in range(len(threads)):
        threads[i].join()


def calc_adjunct(num_thread, matrix):
    print(f'Thread #{num_thread} ', end='')
    matrix = np.delete(matrix, 0, axis=0)
    matrix = np.delete(matrix, num_thread, axis=1)
    adjunct = (-1) ** (1 + (num_thread + 1)) * np.round(np.linalg.det(matrix))
    print('Adjunct:', adjunct)


if __name__ == '__main__':
    main()