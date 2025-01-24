#!/usr/bin/env python3
 
from typing import List

def solve_n_quees(n: int) -> List[List[int]]:
    cols = set()
    # right up
    ru_diag = set()
    # right down
    rd_diag = set()
    res = []
    board = [['.'] * n for i in range(n)]

    def backtrack(row):
        if row == n:
            board_copy = ["".join(row) for row in board]
            res.append(board_copy)
        for col in range(n):
            if col in cols or (row + col) in ru_diag or (row - col) in rd_diag:
                continue
            board[row][col] = "Q"
            cols.add(col)
            ru_diag.add(row + col)
            rd_diag.add(row - col)
            backtrack(row + 1)
            cols.remove(col)
            ru_diag.remove(row + col)
            rd_diag.remove(row - col)
            board[row][col] = "."
    backtrack(0)
    return res
