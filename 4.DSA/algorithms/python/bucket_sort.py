#!/usr/bin/env python3

def bucket_sort(array: list[int]) -> list[int]:
    buckets = bucket_elem(array)
    sorted_buckets = list(map(merge_sort, buckets))
    return [item for sublist in sorted_buckets for item in sublist]

def bucket_elem(array: list[int]) -> list[list[int]]:
    buckets = [[], [], [], [], [], [], [], [], [], []]
    for elem in array:
        buckets[int(elem // 10)].append(elem)
    return buckets

def merge_sort(array: list[int]):
    n = len(array)
    if n < 2:
        return array
    mid = n // 2
    left = merge_sort(array[:mid])
    right = merge_sort(array[mid:])
    return merge(left, right)

def merge(left: list[int], right: list[int]):
    array = []
    nl = len(left)
    nr = len(right)
    i, j = 0, 0
    while i < nl and j < nr:
        if left[i] <= right[j]:
            array.append(left[i])
            i += 1
        else:
            array.append(right[j])
            j+= 1
    array.extend(left[i:])
    array.extend(right[j:])
    return array 

if __name__ == "__main__":
    array = [78, 17, 39, 26, 72, 94, 21, 12, 23, 68]
    print(bucket_sort(array))