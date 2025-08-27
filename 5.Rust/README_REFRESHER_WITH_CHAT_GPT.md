# Ownership → Borrowing → Lifetimes

## Ownership

- Every value has exactly one owner. When the owner goes out of scope, the value
is dropped.
- When a value is moved, the ownership is transfered. 
- When a value is copied, buts are duplicated (copy type).

## Borrowing

- Temporary access to someone else's data without taking it. 
- &T (read-only), &mut T (exclusive, writable).
- You can have many &T or one mut &T - not both at once.

## Lifetimes

- The (compile-time) guarantee that references never outlive what they point to.

## Real-world mental model



