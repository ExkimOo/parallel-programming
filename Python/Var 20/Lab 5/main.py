from threading import Thread, Semaphore
from random import sample
from time import sleep

COMPONENTS = ['Tobacco', 'Paper', 'Match']
rand_components = sample(COMPONENTS, 2)

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
            print(f'Smoker #{num_thread} now smokes. Component: {component}.'.ljust(42) +
                  f' Intermidiary components: {rand_components}.')
            sem.release()
            sleep(2)


def intermediary():
    while True:
        generate_componenents()
        sem.acquire()


def generate_componenents():
    global rand_components
    rand_components = sample(COMPONENTS, 2)


if __name__ == '__main__':
    main()
