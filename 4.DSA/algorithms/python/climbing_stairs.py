#!/usr/bin/env python3


def climbing_stairs(n: int) -> int:
    # we start from the last 2 steps (bottom-up DP)
    # at the last step we have 1 choice and at the second to last also 1 choice
    # that's why we initialise both variables with 1
    one, two = 1, 1

    # we go until n-1 because we delt with the last step
    for i in range(n-1):
        temp = one
        # it's a deviation from Fibonnaci
        # by using memoization we see that every step the possibilities 
        # equal with the sum of possibilities from the previous 2 steps
        one = one + two
        two = temp

    return one

