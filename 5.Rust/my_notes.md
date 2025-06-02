# Personal learnings

## Choosing between &str and String

- When I want to borrow an immutable slice of UTF-8 text, choose `&str`.
- When I want to mutate or own or grow the test, use `String`.

## Return types

- Sometimes I can return a set of 3 (maybe even more variables - but then the
case for a struct can be made) variables.

```rust
Result<(String, Duration, usize), TranscriptionError>
```

## Lifetimes

- Lifetimes are regions of code that references must be valid for.
- `'a` is a reference lifetime. Which region of the code the reference must be
valid in. 
- But what if instead we think of `'a` as which region of memory it can
point to.

```rust
fn longest<'a>(s1: &'a str, s2: &'a str) -> &'a str {}

fn main() {
    let x: &'x str = "hi";
    let y: &'y str = "hello";
    let l1: str = longest(x, y);
}
```

- We can use 2 different reference lifetimes even though, from the definition of
longest it seems that only one block of memory should be used for both s1 and s2.
- This also means that 'a , 'x and 'y don't have to be unified in the same block
of memory.
