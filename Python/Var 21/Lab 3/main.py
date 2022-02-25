from threading import Thread, Lock
from random import choice
from string import ascii_uppercase


mutex = Lock()


class ThreadWithReturnValue(Thread):
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs={}, Verbose=None):
        Thread.__init__(self, group, target, name, args, kwargs)
        self._return = None
    def run(self):
        if self._target is not None:
            self._return = self._target(*self._args,
                                                **self._kwargs)
    def join(self, *args):
        Thread.join(self, *args)
        return self._return


def main():
    N = 8
    genes = ascii_uppercase[:N]
    print('Genes:', genes)

    parents = []
    for i in range(2):
        parents.append(''.join([choice(genes) for _ in range(N)]))
        print(f'Parent {i} genes:', parents[i])

    threads = [None] * N
    for i, (parent1_gene, parent2_gene) in enumerate(zip(parents[0], parents[1])):
        threads[i] = ThreadWithReturnValue(target=calc_child_genes, args=(i, parent1_gene, parent2_gene, genes))
        threads[i].start()

    results = []
    for i in range(len(threads)):
        results.append({i: threads[i].join()})

    print(results)

def calc_child_genes(num_thread, parent1_gene, parent2_gene, genes):
    mutex.acquire()

    print(f'Thread #{num_thread}  ', end='')
    child_gene = genes[(genes.index(parent1_gene) + genes.index(parent2_gene)) % len(genes)]
    print(f'{parent1_gene} + {parent2_gene} = {child_gene}')

    mutex.release()

    return child_gene


if __name__ == '__main__':
    main()