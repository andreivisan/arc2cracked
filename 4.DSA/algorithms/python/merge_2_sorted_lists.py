#!/usr/bin/env python3

from singly_linked_list import Node, SinglyLinkedList

def merge_2_lists(head1, head2):
    head_result = Node()
    current = head_result
    while head1 and head2:
        if head1.data <= head2.data:
            current.next = head1
            head1 = head1.next
        else:
            current.next = head2
            head2= head2.next
        current = current.next
    if head1:
        current.next = head1
    else:
        current.next = head2
    return head_result.next

if __name__ == "__main__":
    sll1 = SinglyLinkedList()
    sll1.append(1)
    sll1.append(2)
    sll1.append(4)
    sll2 = SinglyLinkedList()
    sll2.append(1)
    sll2.append(3)
    sll2.append(4)
    result = SinglyLinkedList()
    result.head = merge_2_lists(sll1.head, sll2.head)
    result.print_list()


