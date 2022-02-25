from threading import Thread
from random import randint


def main():
    N = 10

    arr = [randint(1, 10) for _ in range(N)]
    sums = [arr[i - 1] + arr[i + 1] for i in range(1, N - 1)]
    sums.insert(0, 0)
    sums.append(0)

    print(arr)
    print(sums)

    threads = [None] * N
    for i in range(1, N - 1):
        threads[i-1] = Thread(target=check_divisibility, args=(i, arr[i], sums[i]))
        threads[i-1].start()

    for i in range(0, N - 2):
        threads[i].join()

def check_divisibility(num_thread, number, neighbors_sum):
    print(f'Thread #{num_thread} {number}/{neighbors_sum}: '.ljust(17), end='')
    if number % neighbors_sum == 0:
        print('Divisible')
        return True
    print('Not divisible')
    return False


if __name__ == '__main__':
    main()