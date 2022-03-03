from threading import Thread
from string import ascii_uppercase
from random import randint, sample, choice
from time import sleep

from rwlock import RwLock

rwlock = RwLock()
cities = [letter for letter in ascii_uppercase[:5]]


def main():
    N = [sample(list(filter(lambda x: x != city, cities)), randint(1, 2)) for city in cities]

    d = [None] * len(cities)
    for i in range(len(cities)):
        d[i] = {}
        for city in N[i]:
            d[i][city] = randint(10, 20)

    N = dict(zip(cities, d))
    print(N)

    for func in [change_price, add_delete_road, add_delete_city, find_road]:
        thread = Thread(target=func, args=(N,))
        thread.start()


def change_price(N):
    while True:
        new_price = randint(10, 20)

        rwlock.acquire_w()
        try:
            city_fr = choice(list(N.keys()))
            city_to = choice(list(N[city_fr].keys()))
            print(f'The price of the ticket from city {city_fr} to '
                  f'{city_to} now {new_price} (was {N[city_fr][city_to]})')
            N[city_fr][city_to] = new_price
        except:
            pass
        rwlock.release_w()

        sleep(2)


def add_delete_road(N):
    while True:
        if randint(0, 1) == 1:
            rwlock.acquire_w()
            try:
                city_fr = choice(list(N.keys()))
                city_to = choice(cities)
                price = randint(10, 20)

                if city_to not in N[city_fr].keys():
                    N[city_fr][city_to] = price
                    print(f'Added road from {city_fr} to {city_to} with price {price}')
            except:
                pass
            rwlock.release_w()
        else:
            rwlock.acquire_w()
            try:
                city_fr = choice(list(N.keys()))
                city_to = choice(list(N[city_fr].keys()))
                print(f'Deleted road from {city_fr} to {city_to}')
                del N[city_fr][city_to]
            except:
                pass

            rwlock.release_w()

        sleep(2)


def add_delete_city(N):
    global cities

    while True:
        if randint(0, 1) == 1:
            rwlock.acquire_w()
            try:
                city = choice(list(N.keys()))
                del N[city]
                print(f'City {city} was deleted')
            except:
                pass
            rwlock.release_w()
        else:
            new_city = choice(ascii_uppercase)
            rwlock.acquire_w()
            if new_city not in cities:
                city_to = choice(cities)
                N[new_city] = {}
                N[new_city][city_to] = randint(10, 20)
                cities.append(new_city)
                print(f'New city {new_city} was added')
            rwlock.release_w()

        sleep(2)


def find_road(N):
    global cities

    while True:
        rwlock.acquire_r()
        city_fr, city_to = sample(cities, 2)
        try:
            print(N[city_fr][city_to])
        except:
            print(f'No direct road from {city_fr} to {city_to}')
            try:
                for city in list(N[city_fr].keys()):
                    if city_to not in list(N[city].keys()):
                        print(f'Road {city_fr} -> {city} -> {city_to}')
            except:
                pass
        rwlock.release_r()

        sleep(2)


if __name__ == '__main__':
    main()
