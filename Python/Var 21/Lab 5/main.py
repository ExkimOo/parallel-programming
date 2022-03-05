from threading import Thread, Lock, Semaphore
from random import randint
from time import sleep

N = 5
M = 3

passengers = [randint(1, 3) for _ in range(N)]
buses = [[i, randint(0, N - 1), 0] for i in range(M)]

change_mutex = Lock()
print_mutex = Lock()
sem = Semaphore(M)


def main():
    buses_thread = [None] * M
    for i in range(M):
        buses_thread[i] = Thread(target=stop_bus, args=(i,))
        buses_thread[i].start()

    passengers_thread = [None] * N
    for i in range(N):
        passengers_thread[i] = Thread(target=board_passengers, args=(i,))
        passengers_thread[i].start()


def stop_bus(num_thread):
    global passengers
    global buses

    while True:
        change_mutex.acquire()
        passengers_to_board = randint(-buses[num_thread][2], passengers[buses[num_thread][1]])
        buses[num_thread][2] += passengers_to_board
        passengers[buses[num_thread][1]] -= passengers_to_board

        print_mutex.acquire()
        print(f'Bus #{buses[num_thread][0]} at stop #{buses[num_thread][1]} with {buses[num_thread][2]} passengers. ',
              end='')
        if passengers_to_board >= 0:
            print(f'{passengers_to_board} have boarded to bus #{buses[num_thread][0]}')
        else:
            print(f'{-passengers_to_board} have out of bus #{buses[num_thread][0]}')
        print_mutex.release()
        change_mutex.release()

        sem.acquire()
        sleep(2.9)


def board_passengers(num_thread):
    global passengers
    global buses

    while True:
        print_mutex.acquire()
        print(f'Stop {num_thread} with {passengers[num_thread]} passengers')
        print_mutex.release()

        change_mutex.acquire()
        for i in range(len(buses)):
            buses[i][1] = randint(0, N - 1)
        change_mutex.release()

        sem.release()
        sleep(3)


if __name__ == '__main__':
    main()
