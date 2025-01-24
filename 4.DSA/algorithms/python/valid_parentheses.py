#!/usr/bin/env python3

def is_valid(str):
    stack = []
    storage = {
        ")": "(",
        "]": "[",
        "}": "{",
    }
    for ch in str:
        if ch in storage.keys() and stack and stack[-1] == storage[ch]:
                stack.pop()
        else:
            stack.append(ch) 

    return len(stack) == 0


if __name__ == "__main__":
    print(is_valid("([{}])"))
    print(is_valid("[(])"))
