#!/usr/bin/env python3

def merge_sort(array: list[int]):
    n = len(array)
    if n < 2:
        return
    mid = n // 2
    left = array[:mid]
    right = array[mid:]
    # divide phase
    merge_sort(left)
    merge_sort(right)
    print(f"left: {left} | right: {right}")
    # merge phase
    merge(n, array, left, right)

def merge(n, array: list[int], left: list[int], right: list[int]):
    nl = len(left)
    nr = len(right)
    i, j, k = 0, 0, 0
    while i < nl and j < nr:
        print(f"comparing {left[i]} with {right[j]}")
        if left[i] <= right[j]:
            array[k] = left[i]
            i += 1
        else:
            array[k] = right[j]
            j += 1
        k += 1
    while i < nl and k < n:
        array[k] = left[i]
        i += 1
        k += 1
    while j < nr and k < n:
        array[k] = right[j]
        j += 1
        k += 1
    
if __name__ == "__main__":
    array = [12, 11, 13, 5, 6, 7, 4]
    merge_sort(array)
    print(array)