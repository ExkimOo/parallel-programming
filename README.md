# Parallel Programming

## Laboratory work #1

Given **N** students with three grades in subjects.
Need to choose not more than **M** *students* with average mark not less than **3** points in five-point scale.

## Laboratory work #2-3

Given matrix **M**x**N** and array size of **K**. 
Find the *number* of occurrences of the array in the matrix in forward and backward notation in parallel and diagonal sequences of array elements.

## Laboratory work #4

*Rework* laboratory work 3 using semaphors.

## Laboratory work #5

A tribe of **N** savages together eat from a pot, that accomodate **M** pieces of stewed missioner. When a savage want to eat, he eats one piece from the pot, if it's not empty. If the pot is empty, missioner wakes up a cook and waits until he fulled the pot. The cook gets asleep after cooking.
*Build a multi-threaded application* that implements the lunch of the savages. Use semaphors.

## Laboratory work #6

*Rework* laboratory work 5 using condition variables.

## Laboratory work #7

*Build a multi-threaded application* working with linked list. The threads must:
- change random element of the list
- read random element of the list and print it
- count number of elements in list
- add elements to the beginning or to the end of the list
- delete random element of the list

## Laboratory work #8

*Build a 4-threaded application*. Each of the threads generates a text string and count its number of vowels. If numbers at least of two of the threads is equal the work of threads is terminated, otherwise the threads generates strings again. Threads are working in infinite loops until the work is done.

## Laboratory work #9

*Rework* laboratory works 1, 2 and 8 with **MPI** library.

## Laboratory work #10

Given a set A of N numbers and a set B of K numbers. Make a set C, each number of which is the product of the number of sets A and B, so that the sum of the numbers in set C is maximum. *Use NVIDIA CUDA*.
