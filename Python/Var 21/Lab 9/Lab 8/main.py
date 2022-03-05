from multiprocessing import Process, Barrier, Manager, current_process
from random import randint
from time import sleep
from functools import reduce

import numpy as np


def main():
    manager = Manager()
    sets = manager.list()
    lock = manager.Lock()
    barrier = Barrier(2)

    processes = [None] * 2
    for i in range(len(processes)):
        processes[i] = Process(target=calc_rows_product, args=(barrier, sets, lock))
        processes[i].start()

    for i in range(len(processes)):
        processes[i].join()


def calc_rows_product(barrier, sets, lock):
    N = 5
    matrix = np.random.randint(1, 5, (N, N))
    id = int(current_process().name[-1])

    while True:
        products = set()
        for i in range(N):
            matrix[i, i] = randint(1, 5)

        for row in matrix:
            products.add(reduce(lambda x, y: x * y, row))

        lock.acquire()
        try:
            print(f'Process {id}')
            print(f'Matrix:\n{matrix}')
            print(f'Products: {products}')
            sets.append(products)
        finally:
            lock.release()

        barrier.wait()
        sleep(0.5)

        lock.acquire()
        interception = sets[0] & sets[1]
        lock.release()

        if id == 2:
            print(f'Interception: {interception if len(interception) > 0 else "empty"}')

        if len(interception) > 0:
            if id == 2:
                print('Processes stopped')
            break

        if id == 2:
            lock.acquire()
            sets[:] = []
            lock.release()

        sleep(1)


if __name__ == '__main__':
    main()
