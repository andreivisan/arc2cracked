#!/usr/bin/env python3

from typing import List, Optional
from singly_linked_list import Node, SinglyLinkedList

def merge_k_sorted_lists(lists: List[Optional[Node]]) -> Optional[Node]:
    n = len(lists)
    if n == 0:
        return None
    if n == 1:
        return lists[0]
    mid = n // 2
    left = merge_k_sorted_lists(lists[:mid])
    right = merge_k_sorted_lists(lists[mid:])
    return merge(left, right)

def merge(left: Optional[Node], right: Optional[Node]) -> Optional[Node]:
    head_result = Node()
    current = head_result
    while left and right:
        if left.data <= right.data:
            current.next = left
            left = left.next
        else:
            current.next = right
            right = right.next
        current = current.next
    if left:
        current.next = left
    else:
        current.next = right
    return head_result.next

if __name__ == "__main__":
    sll1 = SinglyLinkedList()
    sll1.append(1)
    sll1.append(4)
    sll1.append(5)
    sll2 = SinglyLinkedList()
    sll2.append(1)
    sll2.append(3)
    sll2.append(4)
    sll3 = SinglyLinkedList()
    sll3.append(2)
    sll3.append(6)
    lists = []
    lists.append(sll1.head)
    lists.append(sll2.head)
    lists.append(sll3.head)
    result = SinglyLinkedList()
    result.head = merge_k_sorted_lists(lists)
    result.print_list()


