from multiprocessing import Process
import string
from random import choice


def main():
    N = 15
    step = 3
    C = ''.join([choice(string.ascii_uppercase) for _ in range(N)])
    encode_symbols = {letter: choice(string.ascii_uppercase) for letter in string.ascii_uppercase}

    print('String:', C)
    print('Encode set:', encode_symbols)

    threads = [None] * (N // 3)
    for num_proc, i in enumerate(range(0, N, step)):
        threads[num_proc] = Process(target=encode, args=(num_proc, C[i:i + step], encode_symbols))
        threads[num_proc].start()

    for i in range(len(threads)):
        threads[i].join()


def encode(num_proc, string, encode_symbols):
    print(f'Process #{num_proc}'.ljust(12), end='')
    encoded_part = ''
    for letter in string:
        encoded_part += encode_symbols[letter]

    print(f'{string} -> {encoded_part}')

    return encoded_part


if __name__ == '__main__':
    main()