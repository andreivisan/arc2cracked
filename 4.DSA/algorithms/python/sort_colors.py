#!/usr/bin/env python3

def sort_colors(array: list[int]) -> list[int]:
    buckets = bucket_elem(array)
    return [item for sublist in buckets for item in sublist]

def sort_colors_dutch_flag_algo(array: list[int]) -> list[int]:
    low, mid, high = 0, 0, len(array) - 1
    while mid <= high:
        if array[mid] == 0:
            array[low], array[mid] = array[mid], array[low]
            low += 1
            mid += 1
        elif array[mid] == 1:
            mid += 1
        else:
            array[mid], array[high] = array[high], array[mid]
            high -= 1
    return array

def bucket_elem(array: list[int]) -> list[list[int]]:
    buckets = [[], [], []]
    for elem in array:
        buckets[elem].append(elem)
    return buckets

if __name__ == "__main__":
    array = [2,0,2,1,1,0]
    print(sort_colors_dutch_flag_algo(array))