# Ownership → Borrowing → Lifetimes

## Ownership

- Every value has exactly one owner. When the owner goes out of scope, the value
is dropped.
- When a value is moved, the ownership is transfered. 
- When a value is copied, buts are duplicated (copy type).

### Concrete examples

```rust
// Move (non-Copy)
let s1 = String::from("hi");
let s2 = s1;        // s1 is now invalid; s2 owns the string's heap buffer.
// println!("{s1}"); // ❌ compile error

// Copy (Copy types)
let x = 42;
let y = x;          // x and y are both valid; integers are Copy.
println!("{x} {y}"); // ✅

// Passing by value moves (for non-Copy)
fn takes(s: String) { /* ... */ }

let s = String::from("data");
takes(s);           // move into function
// s is invalid here

// Returning by value gives ownership to caller
fn make() -> String { String::from("owned by caller") }
let got = make();   // got now owns it

// Method calls that take self also move:
struct Buf(String);
impl Buf {
    fn into_inner(self) -> String { self.0 } // moves out
}
let b = Buf(String::from("x"));
let inner = b.into_inner(); // b moved

// Iteration: into_iter moves items; iter borrows them
let v = vec![String::from("a"), String::from("b")];
for s in v.into_iter() {        // moves Strings out
    /* s is owned here */
}
// v is now moved and unusable

// Borrow instead:
let v = vec![String::from("a"), String::from("b")];
for s in v.iter() {             // s: &String
    println!("{}", s);
}
// v still usable (it still owns its Strings)
```

### Advanced but common move patterns

- **Partial moves out of structs:** use Option<T> + .take() to move a field while leaving 
None behind, keeping the rest usable.
- **std::mem::replace / take:** swap/move values out cleanly without cloning.
- **Shared ownership:** Rc<T>/Arc<T> let you “clone” a pointer and share ownership via 
ref counts (not a deep copy of T).

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

### Lifetimes: when I must annotate

When a function returns a reference that's tied to one of its inputs, you sometimes need
tell the compiler which one.

```rust
pub fn longer<'a>(a: &'a str, b: &'a str) -> &'a str {
    if a.len() >= b.len() { a } else { b }
}
```

- The 'a says: both inputs and the output share the same minimum lifetime.
- This guarantees the return value can't outlive either a or b.
- Without the annotation, the compiler can't figure out which input the output is tied to.

> Heuristic: If your function returns a  reference to something in its arguments, lifetimes
> likely need annotation. If you construct new owned data (e.g. String, Vec), you usually
> don't.

Tighten it:

- You never need lifetime annotations when your function returns owned data (String, Vec<T>, 
any non-reference). Ownership severs lifetime ties.
- You also don’t need annotations when you return Copy scalars by value.
- You do need lifetime annotations when your function returns a reference and the compiler 
cannot infer which input it’s tied to.

Rust’s lifetime elision rules often remove the need to write 'a:
	
1. Each input reference gets its own fresh lifetime.
2. If there’s exactly one input reference, the output reference is assumed to borrow from it.
3. If &self/&mut self is an input, output borrows from self.

### Examples

```rust
// ✅ No annotation needed: one input reference, one output reference (elided)
fn first_word(s: &str) -> &str {
    // Output slice borrows from input `s`
    let bytes = s.as_bytes();
    for i in 0..bytes.len() {
        if bytes[i].is_ascii_whitespace() {
            return &s[..i]; // slice of s (OK)
        }
    }
    s
}

// ✅ No annotation (owned output)
fn make_vec() -> Vec<i32> {
    vec![1, 2, 3]
}

// ✅ No annotation: returns Copy
// because ints are Copy
fn max3(a: i32, b: i32, c: i32) -> i32 {
    a.max(b).max(c)
}

// ❌ Cannot return a ref to a local temporary (would dangle)
fn bad_ref() -> &str {
    let s = String::from("temp");
    &s[..] // ERROR: s is dropped at end of function
}

// ✅ Needs annotation: output could come from `a` or `b`
fn longer<'a>(a: &'a str, b: &'a str) -> &'a str {
    if a.len() >= b.len() { a } else { b }
}

// ✅ Method elision rule #3: output borrows from &self, no explicit 'a needed
impl MyBuf {
    fn as_slice(&self) -> &str { &self.data }
}
```

### Bonus: tiny “why it matters” mental model
	
• Ownership answers: who cleans up?
• Borrowing answers: who can read/write right now, and does someone else still own it?
• Lifetimes answer: how long is this borrow guaranteed to be valid?

