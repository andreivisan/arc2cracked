#!/usr/bin/env python3

import random

def quick_sort(array: list[int], low: int, high: int):
    if low < high:
        pivot_index = partition_pivot_random(array, low, high)
        quick_sort(array, low, pivot_index - 1)
        quick_sort(array, pivot_index + 1, high)

def partition_pivot_first(array: list[int], low: int, high: int) -> int:
    i = high + 1
    pivot_index = low
    for j in range(high, low, -1):
        if array[j] >= array[pivot_index]:
            i -= 1
            swap(array, i, j)
    swap(array, i-1, pivot_index) 
    return i - 1

def partition_pivot_last(array: list[int], low: int, high: int) -> int:
    i = low - 1
    pivot_index = high
    for j in range(low, high):
        if array[j] <= array[pivot_index]:
            i += 1
            swap(array, i, j)
    swap(array, i+1, pivot_index)
    return i + 1

def partition_pivot_random(array: list[int], low:int, high: int) -> int:
    # Randomly select pivot and move it to the end
    pivot_index = random.randrange(low, high) 
    swap(array, pivot_index, high)
    pivot = array[high]
    i = low - 1
    for j in range(low, high):
        if array[j] <= pivot:
            i += 1
            swap(array, i, j)
    swap(array, i+1, high)
    return i + 1

def swap(array: list[int], i: int, j: int):
    array[i], array[j] = array[j], array[i]

if __name__ == "__main__":
    array = [10, 7, 8, 9, 1, 5]
    quick_sort(array, 0, len(array) - 1)
    print(array)
