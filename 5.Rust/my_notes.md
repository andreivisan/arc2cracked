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
of memory. But it means both x and y have same lifetime 'a.

If we want different lifetimes:

```rust
fn longest<'a>(s1: &'a str, s2: &'a str) -> &'a str {}
    where 's1: 'out, 's2: 'out // out outlives s1 and s2 or s1 and s2 are a subset of out
```

## MPSC - Multiple-Producer Single-Consumer

- Instead of sharing memory between threads `mpsc` uses channels that let threads
share data by sending messages. (The way I see this is a queue where muliple 
threads add data and then one receiver that reads data one by one in FIFO).
- `std::sync::mpsc::channel()` returns a pair (Sender, Receiver).
- Sender can be cloned to create multiple senders that feed into the same channel.
- Receiver can't be cloned as we can only have one receiver.
- When you send a value through a channel, you move ownership of that value into 
the channel. 

## Difference between &, &mut and ref ref mut

## mem::replace and drain(..)
