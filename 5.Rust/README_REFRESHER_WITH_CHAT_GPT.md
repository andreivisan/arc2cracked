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
```

### Borrowing in practice: "count words" without clones

Parse words from a &str and count into a HashMap. The trick: intern &str keys by borrowing
via the raw input slice, not by allocating new Strings.

```rust
use std::collections::HashMap;

pub fn words_counts(input: &str) -> HashMap<&str, usize> {
    // we must use mut here as we will keep changing the map
    let mut map: HashMap<&str, usize> = HashMap::new();

    for word in input.split_whitespace() {
        // word is a &str slice into input; OK to use as map key that lives as long as input
        // good to always think if this slice and source of slice live as long as the map
        *map.entry(word).or_insert(0) += 1;
    }

    map
}
```

>**Notes**
> - Keys are &str, not String. This means the map doesn't own the characters; it borrows
> from input. Fast, but it contrains lifetime: the map must not outlive input. That's fine
> if I keep them in the same scope.
>
> - If I must return the map beuond input's lifetime, I switch to HashMap<String, usize> and
> pay for the allocations at the edges. Know my data lifetimes → choose ownership boundaries
> accordingly.
