from threading import Thread, Barrier, Lock, current_thread
from random import randint
from time import sleep

sets = [None] * 3
mutex = Lock()


def main():
    barrier = Barrier(3)
    threads = [None] * 3
    for i in range(len(threads)):
        threads[i] = Thread(target=generate_set, args=(barrier,))
        threads[i].start()

    return


def generate_set(barrier):
    global sets
    while True:
        id = int(current_thread().name[-1])
        numbers = set([randint(1, 7) for _ in range(5)])
        sets[id - 1] = numbers

        with mutex:
            print(f'Thread {id}, set:', numbers)

        sleep(0.5)

        barrier.wait()

        with mutex:
            a_b, a_c = sets[0] - sets[1], sets[0] - sets[2]

        if id == 1:
            print(f'A-B: {a_b if len(a_b) != 0 else "empty"}')
            print(f'A-C: {a_c if len(a_c) != 0 else "empty"}')

        if len(a_b) >= 2 and len(a_c) >= 2:
            if id == 1:
                print('Threads stopped')
            break

        sleep(1)

if __name__ == '__main__':
    main()