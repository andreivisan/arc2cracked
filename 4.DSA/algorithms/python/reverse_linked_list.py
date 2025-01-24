#!/usr/bin/env python3

from singly_linked_list import Node, SinglyLinkedList


def reverse_list(head):
    previous_node = None
    current_node = head
    while current_node:
        next_node = current_node.next
        current_node.next = previous_node
        previous_node = current_node
        current_node = next_node
    return previous_node

def reverse_recursive(head):
    def _reverse_list_rec(current, prev=None):
        if not current:
            return prev
        next_node = current.next
        current.next = prev
        return _reverse_list_rec(next_node, current)
    return _reverse_list_rec(head)
    

if __name__ == "__main__":
    sll = SinglyLinkedList()
    sll.append(1)
    sll.append(2)
    sll.append(3)
    sll.head = reverse_list(sll.head)
    sll.print_list()
    sll.head = reverse_recursive(sll.head)
    sll.print_list()
