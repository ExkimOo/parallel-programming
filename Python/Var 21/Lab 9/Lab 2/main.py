from multiprocessing import Process
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

    processes = [None] * N
    for i, (parent1_gene, parent2_gene) in enumerate(zip(parents[0], parents[1])):
        processes[i] = Process(target=calc_child_genes, args=(i, parent1_gene, parent2_gene, genes))
        processes[i].start()

    for i in range(len(processes)):
        processes[i].join()


def calc_child_genes(num_proc, parent1_gene, parent2_gene, genes):
    print(f'Process #{num_proc}  ', end='')
    child_gene = genes[(genes.index(parent1_gene) + genes.index(parent2_gene)) % len(genes)]
    print(f'{parent1_gene} + {parent2_gene} = {child_gene}')


if __name__ == '__main__':
    main()