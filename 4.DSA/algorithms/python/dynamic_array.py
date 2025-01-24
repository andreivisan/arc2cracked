#!/usr/bin/env python3

class DynamicArray:

    def __init__(self, capacity:int):
        self.capacity = capacity
        self.size = 0
        self.arr = [0] * capacity # initialise an array of size capacity filled with zeros

    def get(self, i: int) -> int:
        if not 0 <= i < self.size:
            raise IndexError('Index out of bounds')
        return self.arr[i] 

    def insert(self, i: int, n:int) -> None:
        if not 0 <= i < self.size:
            raise IndexError('Index out of bounds')
        self.arr[i] = n

    def pushback(self, n: int) -> None:
        if self.size == self.capacity:
            self.resize()
        self.arr[self.size] = n
        self.size += 1

    def popback(self) -> int:
        self.size -= 1
        return self.arr[self.size]

    def resize(self) -> None:
        self.capacity = 2 * self.capacity
        new_arr = [0] * self.capacity
        for i in range(self.size):
            new_arr[i] = self.arr[i]
        self.arr = new_arr

    def get_size(self) -> int:
        return self.size

    def get_capacity(self) -> int:
        return self.capacity


# Leetcode 1929: Concatenation of Array
def get_concatenation(nums):
    # simplest way is return nums + nums
    result = resize(nums)
    for index, num in enumerate(nums):
        result[index] = num
        result[index + len(nums)] = num
    return result

def resize(nums):
    new_capacity = 2 * len(nums)
    new_arr = [0] * new_capacity
    return new_arr

if __name__ == "__main__":
    nums = [1, 2, 1]
    print(get_concatenation(nums))
