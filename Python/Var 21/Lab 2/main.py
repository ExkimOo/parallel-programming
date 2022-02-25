from threading import Thread
from random import choice
from string import ascii_uppercase


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
        threads[i] = Thread(target=calc_child_genes, args=(i, parent1_gene, parent2_gene, genes))
        threads[i].start()

    for i in range(len(threads)):
        threads[i].join()


def calc_child_genes(num_thread, parent1_gene, parent2_gene, genes):
    print(f'Thread #{num_thread}  ', end='')
    child_gene = genes[(genes.index(parent1_gene) + genes.index(parent2_gene)) % len(genes)]
    print(f'{parent1_gene} + {parent2_gene} = {child_gene}')


if __name__ == '__main__':
    main()