#!/usr/bin/env python3

from typing import Optional
from singly_linked_list import Node, SinglyLinkedList

def remove_nth_from_end(head: Node, n: int) -> Optional[Node]:
    sll_len = compute_sll_length(head)
    if sll_len == 0:
        return None
    index_to_delete = sll_len - n
    current_index = 0
    prev_node = None
    current_node = head
    if index_to_delete == 0:
        head = current_node.next
        return head
    while current_index < index_to_delete:
        current_index += 1
        prev_node = current_node
        current_node = current_node.next
    prev_node.next = current_node.next
    return head

def compute_sll_length(head: Node) -> int:
    sll_len = 0
    current_node = head
    while current_node:
        sll_len += 1
        current_node = current_node.next
    return sll_len

if __name__ == "__main__":
    sll = SinglyLinkedList()
    sll.append(1)
    sll.append(2)
    sll.append(3)
    sll.append(4)
    sll.append(5)
    remove_nth_from_end(sll.head, 2)
    sll.print_list()
    