from threading import Thread
from random import randint
from time import sleep

from rwlock import RwLock

rwlock = RwLock()


class Rectangle():
    def __init__(self, x1, y1, x2, y2):
        self.x1 = x1
        self.x2 = x2
        if x1 == x2:
            self.x2 = x2 + 1
        if x1 > x2:
            self.x1 = x2
            self.x2 = x1

        self.y1 = y1
        self.y2 = y2
        if y1 == y2:
            self.y2 = y2 + 1
        if y1 > y2:
            self.y1 = y2
            self.y2 = y1

        self.area = abs(self.y1 - self.y2) * abs(self.x1 - self.x2)

    def __repr__(self):
        return repr(f'Rectangle: (x1={self.x1}, y1={self.y1}, x2={self.x2}, y2={self.y2}), area: {self.area}')


rectangles = [Rectangle(randint(0, 15), randint(0, 15), randint(0, 15), randint(0, 15)) for _ in range(2)]


def main():
    for func in [delete_rectangle,
                 add_rectangle,
                 print_rectangles,
                 print_max_rectangle_area,
                 print_disjoint_rectangles]:
        thread = Thread(target=func)
        thread.start()


def delete_rectangle():
    global rectangles

    while True:
        rwlock.acquire_w()
        i = randint(0, len(rectangles))
        print(f'[Removed]'.ljust(22) + f' {i}')
        rectangles = rectangles[:i] + rectangles[i + 1:]
        rwlock.release_w()

        sleep(2.5)


def add_rectangle():
    global rectangles

    while True:
        rwlock.acquire_w()
        i = randint(0, len(rectangles))
        generated_rect = Rectangle(randint(0, 5), randint(0, 5), randint(0, 5), randint(0, 5))
        rectangles.append(Rectangle(randint(0, 5), randint(0, 5), randint(0, 5), randint(0, 5)))
        print(f'[Added]'.ljust(22) + f' {generated_rect}')
        rwlock.release_w()

        sleep(2.5)


def print_rectangles():
    global rectangles

    while True:
        rwlock.acquire_r()
        print(f'[Printed]'.ljust(22) + f' {rectangles}')
        rwlock.release_r()

        sleep(2.5)


def print_max_rectangle_area():
    global rectangles

    while True:
        rwlock.acquire_r()
        max_area_rect = rectangles[0]
        for rect in rectangles:
            if max_area_rect.area < rect.area:
                max_area_rect = rect

        print(f'[Max area rectangles]'.ljust(22) + f' {max_area_rect}')
        rwlock.release_r()

        sleep(2.5)


def print_disjoint_rectangles():
    global rectangles
    global lock

    while True:
        rwlock.acquire_r()
        count = 0
        for i in range(len(rectangles) - 1):
            for j in range(i + 1, len(rectangles)):
                if not (((rectangles[i].x1 >= rectangles[j].x1 and rectangles[i].x2 <= rectangles[j].x1) or
                         (rectangles[i].x1 >= rectangles[j].x2 and rectangles[i].x2 <= rectangles[j].x2)) and
                        ((rectangles[i].y1 >= rectangles[j].y1 and rectangles[i].y2 <= rectangles[j].y1) or
                         (rectangles[i].y1 >= rectangles[j].y2 and rectangles[i].y2 <= rectangles[j].y2))):
                    print(f'[Disjoint]'.ljust(22) + f' {rectangles[i]}, {rectangles[j]}')
                    count += 1
                    if count > 3:
                        break

        rwlock.release_r()

        sleep(2.5)


if __name__ == '__main__':
    main()
