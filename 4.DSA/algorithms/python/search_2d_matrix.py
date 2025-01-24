#!/usr/bin/env python3

from typing import List

def search_2d_matrix(matrix: List[List[int]], target: int) -> bool:
    rows = len(matrix)
    if rows == 0:
        return False
    cols = len(matrix[0])
    for i in range(rows):
        if target == matrix[i][cols - 1]:
            return True
        if target < matrix[i][cols - 1]:
            return binary_search(matrix[i], target, 0, cols - 1)
    return False

def binary_search(array: List[int], target: int, low: int, high: int) -> bool:
    if low > high:
        return False
    mid = low + (high - low) // 2
    if array[mid] < target:
        return binary_search(array, target, mid + 1, high)
    elif array[mid] > target:
        return binary_search(array, target, low, mid - 1)
    else:
        return True

if __name__ == "__main__":
    matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]]
    print(search_2d_matrix(matrix, 7))
    