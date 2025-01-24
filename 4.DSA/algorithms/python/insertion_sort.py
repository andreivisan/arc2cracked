#!/usr/bin/env python3

def insertion_sort(array: list[int]) -> list[int]:
    n = len(array)
    for i in range(1, n):
        key = array[i]
        j = i - 1
        while j >= 0 and array[j] > key:
            array[j+1] = array[j]
            j -= 1
        array[j+1] = key
    return array

def insert_sorted(array: list[int], val: int, start: int, end: int) -> list[int]:
    if start > end:
        array.insert(start, val)
        return array
    
    mid = start + (end - start) // 2
    
    if array[mid] == val:
        array.insert(mid, val)
        return array
    elif val > array[mid]:
        return insert_sorted(array, val, mid + 1, end)
    else:
        return insert_sorted(array, val, start, mid - 1)

if __name__ == "__main__":
    array = [8, 4, 2, 9, 5]
    print(insertion_sort(array))
    print(insert_sorted(array, 11, 0, len(array) - 1))
