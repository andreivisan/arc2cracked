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

## Shared ownership: Rc / Arc vs & / &mut

- Rc<T> / Arc<T>: cloning duplicates the pointer and bumps a refcount (non-atomic for Rc, 
atomic for Arc). The T is not cloned; all owners point to the same heap allocation. 
Memory is freed when the last strong owner drops.
- &T / &mut T: these are borrows, not owners. No refcount, zero runtime cost. They cannot 
outlive the real owner, and &mut enforces exclusive access.

Quick contrasts:

- Cost: &/&mut (compile-time checks, zero runtime) ⟶ cheaper. 
Rc/Arc (refcount updates) ⟶ some runtime overhead; Arc uses atomics.
- Mutability: Rc<T> is shared-immutable by default. To mutate through many owners:
	- single-thread: Rc<RefCell<T>>
	- multi-thread: Arc<Mutex<T>> or Arc<RwLock<T>>
- Cycles: Rc/Arc can leak with reference cycles. Use Weak<T> to break cycles.

### Tiny demos

**Many owners, single-thread**

```rust
use std::rc::Rc;

let a = Rc::new(String::from("hi"));
let b = Rc::clone(&a);           // bump refcount, same allocation
assert_eq!(Rc::strong_count(&a), 2);
```

**Shared mutation, single-thread**

```rust
use std::cell::RefCell;
use std::rc::Rc;

let v = Rc::new(RefCell::new(0));
let v1 = Rc::clone(&v);
let v2 = Rc::clone(&v);
*v1.borrow_mut() += 1;
*v2.borrow_mut() += 1;
assert_eq!(*v.borrow(), 2);
```

**Shared mutation, multi-thread**

```rust
use std::sync::{Arc, Mutex};
use std::thread;

let data = Arc::new(Mutex::new(0));
let mut handles = vec![];

for _ in 0..4 {
    let d = Arc::clone(&data);
    handles.push(thread::spawn(move || *d.lock().unwrap() += 1));
}

for h in handles { h.join().unwrap(); }
assert_eq!(*data.lock().unwrap(), 4);
```

**Avoid cycles with Weak**

```rust
use std::rc::{Rc, Weak};

let a = Rc::new(42);
let weak: Weak<i32> = Rc::downgrade(&a);   // not counted as strong owner
assert!(weak.upgrade().is_some());
drop(a);
assert!(weak.upgrade().is_none());         // allocation freed
```

If you want a quick “when to reach for what” rule:
	
- Start with borrows (&T, &mut T).
- If you must store data beyond a borrower’s lifetime, take ownership (T, e.g., String).
- If you need multiple owners, use Rc<T> (same thread) or Arc<T> (across threads). 
Add RefCell/Mutex only when you truly need mutation from multiple owners.

## AsRef

**What is it?**: A trait for cheat, by-reference conversions.

```rust
pub trait AsRef<T: ?Sized> {
    fn as_ref(&self) -> &T;
}
```

Use it when your function can work with many “string-ish/path-ish/byte-ish” inputs and 
all you need is a borrowed view (&str, &Path, &[u8])—no allocation, no ownership transfer.

### When to use AsRef

- Flexible inputs: accept String or &str or Cow<str> with one signature: impl AsRef<str>.
- Path-like APIs: idiomatic file APIs use impl AsRef<Path> so callers pass &str, String, Path, PathBuf, etc.
- Byte views: impl AsRef<[u8]> to accept Vec<u8>, &[u8], String (as bytes), etc.

### When not to use AsRef

- If you just need a plain borrow, &str is simpler.
- If you need to return a reference derived from the input, take &'a str (not impl AsRef<str>), 
so you can tie the return to the caller’s lifetime.
- If you need ownership, take String/PathBuf/Vec<u8> instead.

### AsRef vs Into/From vs Borrow vs Deref (mental map)

- **AsRef<T>:** &self -> &T (borrowed view). Cheap, no allocate. Great for flexible inputs.
- **Into<T>/From<T>:** by value conversion (may move/allocate). Use when you want to own the result.
- **Borrow<T>:** Like AsRef but with stronger Eq/Hash promises; used by collections so you can look up 
String keys with &str.
- **Deref<Target = U>:** pointer-like auto-coercion (&String → &str, &PathBuf → &Path). Not for arbitrary 
conversions.

### 1) Accept “anything path-like” with `AsRef<Path>`

```rust
use std::path::Path;

// Accepts &str, String, &Path, PathBuf, etc. — all zero-alloc.
fn file_exists<P: AsRef<Path>>(p: P) -> bool {
    // 1) Convert whatever P is into a borrowed &Path.
    let path: &Path = p.as_ref();
    // 2) Use the borrowed view; no copies of the path happen.
    path.exists()
}

// Calls (all valid and zero-alloc):
let _ = file_exists("config.toml");
let _ = file_exists(String::from("config.toml"));
let _ = file_exists(std::path::Path::new("config.toml"));
let _ = file_exists(std::path::PathBuf::from("config.toml"));
```

**Why this is good**

* Line 4: `P: AsRef<Path>` = flexible public API.
* Line 6: `as_ref()` gives `&Path` without allocation.
* Works for both owned and borrowed inputs thanks to blanket impls.

---

### 2) Generic string input where you only need a borrowed view

```rust
// Works with &str, String, Box<str>, Cow<'_, str>, etc.
fn ends_with_semicolon<S: AsRef<str>>(s: S) -> bool {
    // 1) Borrow as &str regardless of the concrete type.
    let view: &str = s.as_ref();
    // 2) Operate on the borrowed view; no cloning.
    view.ends_with(';')
}

// Calls:
let _ = ends_with_semicolon("let x = 1;");
let _ = ends_with_semicolon(String::from("let x = 1;"));
```

**Why this is good**

* You don’t force callers to allocate or convert first.
* Keeps the function zero-alloc and ergonomic.

---

### 3) Returning a slice: prefer lifetimes over `AsRef` for outputs

```rust
// We are returning a slice that *borrows from the input*.
// Tie the output lifetime ('a) to the input explicitly.
fn first_char<'a>(s: &'a str) -> Option<&'a str> {
    // 1) Get the first byte as a 1-byte slice if it exists.
    s.get(..1)
}
```

**Why not `AsRef` here?**

* A signature like `fn first_char<S: AsRef<str>>(s: S) -> Option<&str>` can’t safely
  return a reference that outlives the local `s` binding.
* Using `&'a str` input + `&'a str` output makes the borrow relationship explicit and sound.

---

# Strings

## `&str` vs `String`: when to use each

- Rust text is basically:
    - `&str` = borrowed view of UTF-8 bytes
    - `String` = owned, growable buffer of UTF-8 bytes

- Under the hood:

```rust
// Conceptually:
struct String {
    ptr: *mut u8,   // heap buffer
    len: usize,     // bytes used
    cap: usize,     // bytes allocated
}

// &str is like:
struct StrRef<'a> {
    ptr: *const u8, // into some existing UTF-8 buffer
    len: usize,     // bytes
    // plus lifetime 'a
}
```

### When to use each

**`str` - borrowed, read-only, zero-copy**

- Function inputs: you just need to read text.
- Slices and other strings: substrings.
- Static literals: "hello" is &'static str.

**`String` - owned, growable**

- You need to build or modify text.
- You need to store text beyond the scope of the caller.
- You need to put it in `Vec`, `HashMap`, etc.

- Heuristic (95% right):
    - Function parameters: &str
    - Function returns:
        - return &str if it’s a slice of caller data
        - return String if it’s new / owned text

## Encoding: why strings are weird in Rust

- Rust strings are UTF-8:
    - `len()` = bytes, not characters.
    - Characters (`char`) are 4-byte Unicode scalar values.
    - Human-visible "characters" can be multiple `char` s.

```rust
let s = "hé";        // "h" + "é"
println!("{}", s.len());  // 3 bytes
println!("{}", s.chars().count()); // 2 chars
```

Because:
- h - 1 byte (0x68)
- é - 2 bytes in UTF-8

### Indexing is forbidden

- This won't compile:

```rust
let c = s[0]; // ❌
```

- Why? Because index 0 might land in the middle of a UTF-8 code point. Rust refuses to let you create invalid Unicode.

- Instead:
    - for byte-level: s.as_bytes()[0]
    - for char-level: s.chars().nth(i)
    - for safe slicing: only slice at valid boundaries (e.g., from indices you got from char_indices()).

```rust
let s: &str = "...";

for b in s.bytes() { /* u8 */ }          // raw bytes
for ch in s.chars() { /* char */ }       // Unicode code points
for (i, ch) in s.char_indices() { /* byte index + char */ }
```

- Use cases:
    - ASCII / simple text problems: bytes() is usually enough (and faster).
    - Unicode-aware problems: chars() or even a grapheme library if they care about real “characters”.
    - Substring on char boundary: use char_indices() to capture safe indices.

## Common operations and their cost

### Building strings

```rust
let mut s = String::new();           // empty
let mut s = String::with_capacity(64); // pre-reserve

s.push('a');                         // O(1) amortized
s.push_str("bc");                    // O(len("bc"))
s += "def";                          // sugar for push_str
s = format!("{}-{}", s, 123);        // can be more alloc / copies
```

- Performance tips:
    - Use push/push_str in loops.
    - Use with_capacity when you can estimate final size.
    - Don’t chain a ton of + on Strings in a hot path; prefer push_str.

### Slicing / substrings (no allocation)

```rust
let s = "hello world";
let hello = &s[..5];   // &str slice
let world = &s[6..];   // &str slice
```

- You must ensure 0..5 etc. are at UTF-8 boundaries; otherwise: panic at runtime. For ASCII-only tasks, indices are safe; for Unicode, use indices from char_indices().

### Splitting and joining

```rust
let s = "a b  c";
s.split_whitespace();        // splits on any unicode whitespace
s.split(' ');                // exact character
s.lines();                   // split on \n / \r\n
["a", "b", "c"].join(",");   // returns String
```

## Advanced tools

- Cow<'a, str>: “clone-on-write” string. Either &'a str or String. Great for APIs that usually just return a borrowed slice but sometimes need to allocate.
- String::from_utf8(Vec<u8>) / String::from_utf8_lossy: converting raw bytes from I/O into String.
- OsStr / OsString / Path / PathBuf: for OS paths that might not be valid UTF-8 on some platforms.

# `matc` and `loop`

## Pattern 1: Matching on a single `Option`

```rust
let mut iter = word.chars();
loop {
    match iter.next() {
        Some(ch) => {
            // use ch
        }
        None => break,  // iterator exhausted
    }
}
```

- iter.next() returns Option<char>.

## Pattern 2: Matching on multiple values

```rust
loop {
    match (iter1.next(), iter2.next()) {
        (Some(a), Some(b)) => { /* got both */ }
        (Some(a), None)    => { /* only iter1 had data */ }
        (None, Some(b))    => { /* only iter2 had data */ }
        (None, None)       => break, // both empty
    }
}
```
