from threading import Thread, Lock
from random import randint

import numpy as np


mutex = Lock()


class ThreadWithReturnValue(Thread):
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs={}, Verbose=None):
        Thread.__init__(self, group, target, name, args, kwargs)
        self._return = None
    def run(self):
        if self._target is not None:
            self._return = self._target(*self._args,
                                                **self._kwargs)
    def join(self, *args):
        Thread.join(self, *args)
        return self._return


def main():
    N = 3
    matrix = np.array([randint(1, 10) for _ in range(N ** 2)]).reshape(N, N)
    print('N:', N)
    print('Matrix:\n', matrix)

    threads = [None] * N
    for i in range(N):
        threads[i] = (ThreadWithReturnValue(target=calc_adjunct, args=(i, matrix)))
        threads[i].start()

    adjuncts = []
    for i in range(len(threads)):
        adjuncts.append({i: int(threads[i].join())})

    print(adjuncts)


def calc_adjunct(num_thread, matrix):
    mutex.acquire()

    print(f'Thread #{num_thread} ', end='')
    matrix = np.delete(matrix, 0, axis=0)
    matrix = np.delete(matrix, num_thread, axis=1)
    adjunct = (-1) ** (1 + (num_thread + 1)) * np.round(np.linalg.det(matrix))
    print('Adjunct:', adjunct)

    mutex.release()

    return adjunct


if __name__ == '__main__':
    main()