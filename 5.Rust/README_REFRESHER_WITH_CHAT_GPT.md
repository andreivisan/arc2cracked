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

> **Real-world mental model**
> 
> You own a GP. Letting a friend look at it is &GPU. Letting them tinker is an
> &mut GPU, but only one tinkerer at a time. Selling the GPU transfers ownership (a move).
> A lifetime error is to return a pointer to your GPU after you tossed it in the bin.

### Example: zero-copy "longest line" finder

**Goal:** Given a &str (whole file contents), return a &str slice of the longest 
line without allocating. This forces us to juggle borrows/lifetimes and avoid
unnecessary String clones.

```rust
use std::cmp::Ordering;

pub fn longest_line(input: &str) -> &str {
    // 1. Split into &str slices; each slice borrows from input.
    let mut best: &str = "";

    for line in input.split('\n') {
        // 2. Comparing lenghts is O(1) on &str (stored as pointer+len).
        match line.len().cmp(&best.len()) {
            Ordering::Greater => best = line, // 3. Rebind best to borrow this longer slice,
            _=> {}                            // No copies. Just moving a pointer+len
        }
    }

    best
}
````
