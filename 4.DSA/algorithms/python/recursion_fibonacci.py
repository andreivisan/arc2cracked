#!/usr/bin/env python3

import time


def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n-1) + fibonacci(n-2)

memo = {}

def fibonacci_memo(n):
    if n <= 1:
        return n
    if n in memo:
        return memo[n]
    memo[n] = fibonacci_memo(n-1) + fibonacci_memo(n-2)
    return memo[n]


if __name__ == "__main__":
    start_time = time.time()
    print(fibonacci(10))
    end_time = time.time()
    print(f"time of execution for fibonacci without memo is {end_time - start_time} seconds")

    start_time = time.time()
    print(fibonacci_memo(10))
    end_time = time.time()
    print(f"time of execution for fibonacci memo is {end_time - start_time} seconds")



