#!/usr/bin/env python3


class MyStack:

    def __init__(self):
        self.s1 = []
        self.s2 = []
        
    def push(self, x: int) -> None:
        if len(self.s1) == 0:    
            self.s1.append(x)
        else:
            self.s2.append(self.s1.pop())
            self.s1.append(x)

    def pop(self) -> int:
        if self.s1:
            return self.s1.pop()
        else:
            return self.s2.pop()

    def top(self) -> int:
        if self.s1:
            return self.s1[0]
        elif self.s2:
            return self.s2[len(self.s2) - 1]
        else:
            return -1

    def empty(self) -> bool:
        return len(self.s1) == 0 and len(self.s2) == 0

    def print_queues(self):
        print(f's1: {self.s1}')
        print(f's2: {self.s2}')

if __name__ == "__main__":
    stack = MyStack()
    stack.push(1)
    stack.push(2)
    stack.push(3)
    stack.print_queues()
    print(stack.top())
    print(stack.pop())
    stack.print_queues()

