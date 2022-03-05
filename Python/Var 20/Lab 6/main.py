from threading import Thread, Condition
from random import sample
from time import sleep

COMPONENTS = ['Tobacco', 'Paper', 'Match']
rand_components = sample(COMPONENTS, 2)

con = Condition()
flag = False


def main():
    smokers = [None] * 3
    for i in range(3):
        smokers[i] = Thread(target=smoker, args=(i, COMPONENTS[i]))
        smokers[i].start()

    interm = Thread(target=intermediary)
    interm.start()


def smoker(num_thread, component):
    global flag

    while True:
        if component not in rand_components:
            with con:
                if not flag:
                    con.wait()
                print(f'Smoker #{num_thread} now smokes. Component: {component}.'.ljust(42) +
                      f' Intermidiary components: {rand_components}.')
                flag = False

            sleep(2)


def intermediary():
    global flag

    while True:
        generate_componenents()
        with con:
            flag = True
            con.notify()


def generate_componenents():
    global rand_components
    rand_components = sample(COMPONENTS, 2)


if __name__ == '__main__':
    main()
