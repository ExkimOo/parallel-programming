from multiprocessing import Process, Barrier, Manager, current_process
from random import randint
from time import sleep


def main():
    manager = Manager()
    sets = manager.list()
    lock = manager.Lock()
    barrier = Barrier(3)

    processes = [None] * 3
    for i in range(len(processes)):
        processes[i] = Process(target=generate_set, args=(barrier, sets, lock))
        processes[i].start()

    for i in range(len(processes)):
        processes[i].join()


def generate_set(barrier, sets, lock):
    while True:
        id = int(current_process().name[-1])
        numbers = set([randint(1, 7) for _ in range(5)])
        sets.append(numbers)

        print(f'Process {id}, set:', numbers)

        sleep(0.5)

        barrier.wait()

        lock.acquire()
        try:
            a_b, a_c = sets[0] - sets[1], sets[0] - sets[2]
        finally:
            lock.release()

        if id == 2:
            print(f'A-B: {a_b if len(a_b) != 0 else "empty"}')
            print(f'A-C: {a_c if len(a_c) != 0 else "empty"}')

        if len(a_b) >= 2 and len(a_c) >= 2:
            if id == 2:
                print('Processes stopped')
            break

        barrier.wait()

        if id == 2:
            sets[:] = []

        sleep(1)


if __name__ == '__main__':
    main()
