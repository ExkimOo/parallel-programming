from threading import Thread, Barrier, Lock, current_thread
from random import randint
from time import sleep
from functools import reduce

import numpy as np

sets = [None] * 2
mutex = Lock()


def main():
    barrier = Barrier(2)
    threads = [None] * 2
    for i in range(len(threads)):
        threads[i] = Thread(target=calc_rows_product, args=(barrier,))
        threads[i].start()

    return


def calc_rows_product(barrier):
    global sets
    N = 5
    matrix = np.random.randint(1, 5, (N, N))
    id = int(current_thread().name[-1])

    while True:
        products = set()
        for i in range(N):
            matrix[i, i] = randint(1, 5)

        for row in matrix:
            products.add(reduce(lambda x, y: x * y, row))

        with mutex:
            print(f'Thread {id}')
            print(f'Matrix:\n{matrix}')
            print(f'Products: {products}')
            sets[id - 1] = products

        barrier.wait()
        sleep(0.5)

        interception = sets[0] & sets[1]

        if id == 1:
            print(f'Interception: {interception if len(interception) > 0 else "empty"}')

        if len(interception) > 0:
            if id == 1:
                print('Threads stopped')
            break

        sleep(1)

if __name__ == '__main__':
    main()
