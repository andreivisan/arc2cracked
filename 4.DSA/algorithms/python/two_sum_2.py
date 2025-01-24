#!/usr/bin/env python3

def two_sum_2(numbers, target):
    p1 = 0
    p2 = len(numbers) - 1

    while p1 < p2:
        sum = numbers[p1] + numbers[p2]
        if sum == target:
            return [p1 + 1, p2 + 1]
        elif sum < target:
            p1 += 1
        else:
            p2 -= 1
    return [p1 + 1, p2 + 1]


if __name__ == "__main__":
    numbers = [2,7,11,15]
    target = 9
    print(two_sum_2(numbers, target))

