#!/usr/bin/env python3

def bubble_sort(nums):
    n = len(nums)

    if n == 0:
        return nums

    for i in range(0, n - 1):
        swapped = False
        for j in range (0, n - i - 1):
            if nums[j] > nums[j + 1]:
                nums[j], nums[j + 1] = nums[j + 1], nums[j]
                swapped = True
        if not swapped:
            break

    return nums 


if __name__ == "__main__":
    array = [5, 1, 4, 2, 8]
    sorted_array = bubble_sort(array)
    print(f"Sorted array: {sorted_array}")
