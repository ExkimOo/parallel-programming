from threading import Thread, Lock, Condition
from random import randint
from time import sleep

N = 5
M = 3

passengers = [randint(1, 3) for _ in range(N)]
buses = [[i, randint(0, N - 1), 0] for i in range(M)]

change_mutex = Lock()
print_mutex = Lock()
con = Condition()
count = 0


def main():
    buses_thread = [None] * M
    for i in range(M):
        buses_thread[i] = Thread(target=stop_bus, args=(i, M))
        buses_thread[i].start()

    passengers_thread = [None] * N
    for i in range(N):
        passengers_thread[i] = Thread(target=board_passengers, args=(i, M))
        passengers_thread[i].start()


def stop_bus(num_thread, M):
    global passengers
    global buses
    global count

    while True:
        with con:
            if count != M:
                con.wait()

            with change_mutex:
                passengers_to_board = randint(-buses[num_thread][2], passengers[buses[num_thread][1]])
                buses[num_thread][2] += passengers_to_board
                passengers[buses[num_thread][1]] -= passengers_to_board

                with print_mutex:
                    print(
                        f'Bus #{buses[num_thread][0]} at stop #{buses[num_thread][1]} with {buses[num_thread][2]} passengers. ',
                        end='')
                    if passengers_to_board >= 0:
                        print(f'{passengers_to_board} have boarded to bus #{buses[num_thread][0]}')
                    else:
                        print(f'{-passengers_to_board} have out of bus #{buses[num_thread][0]}')

            count = 0

        sleep(2.9)


def board_passengers(num_thread, M):
    global passengers
    global buses
    global count

    while True:
        with con:
            count += 1

            with print_mutex:
                print(f'Stop {num_thread} with {passengers[num_thread]} passengers')

            with change_mutex:
                for i in range(len(buses)):
                    buses[i][1] = randint(0, N - 1)

            if count == M:
                con.notify_all()

        sleep(3)


if __name__ == '__main__':
    main()
