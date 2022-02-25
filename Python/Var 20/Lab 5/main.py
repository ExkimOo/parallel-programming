from threading import Thread, Semaphore
from random import choice
from time import sleep


COMPONENTS = ['Tobacco', 'Paper', 'Match']
rand_components = rand_components = [choice(COMPONENTS) for _ in range(2)]

sem = Semaphore(1)
sem.release()


def main():
    smokers = [None] * 3
    for i in range(3):
        smokers[i] = Thread(target=smoker, args=(i, COMPONENTS[i]))
        smokers[i].start()

    interm = Thread(target=intermediary)
    interm.start()


def smoker(num_thread, component):
    while True:
        if component not in rand_components:
            print(f'Smoker #{num_thread} now smokes. Component: {component}.'.ljust(42) + f' Intermidiary components: {rand_components}.')
            generate_componenents()
            sem.release()
            sleep(2)


def intermediary():
    while True:
        generate_componenents()
        sem.acquire()


def generate_componenents():
    global rand_components
    rand_components = [choice(COMPONENTS) for _ in range(2)]
    while rand_components[0] == rand_components[1]:
        rand_components = [choice(COMPONENTS) for _ in range(2)]

    return rand_components

if __name__ == '__main__':
    main()