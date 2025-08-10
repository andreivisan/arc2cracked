# Personal learnings

## Learning with ChatGPT

### Ownership and Borrowing in Rust



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

```rust
for &mut (ref ekey, ref mut evalue) in bucket.iter_mut() {
    /* ekey: &K, evalue: &mut V */
}
```

- & (or &mut) matches an existing reference, so it can only appear when the value 
you’re matching is already a reference.

- ref (or ref mut) creates a reference binding to a value that is inside what 
you’re destructuring, letting you borrow that part instead of moving it.

Therefore, when you iterate with iter_mut()—which yields &mut (K, V)—and you 
want &K and &mut V inside the tuple, you must peel off the outer &mut 
(with &mut (…)) then use ref/ref mut to borrow the tuple’s fields in place.

### Patterns in Rust

- Patterns live on the left side.

- Everything after for, let, match, etc. is a pattern, whose job is to fit the 
shape of the value it receives and optionally bind names to pieces of it. 
Patterns have their own mini-syntax independent of expressions.

| You have…                                       | You write in the pattern | Resulting binding type |
| ----------------------------------------------- | ------------------------ | ---------------------- |
| `T` and you want to move it                     | `x`                      | `T`                    |
| `T` and you want an immutable borrow            | `ref x`                  | `&T`                   |
| `T` and you want a mutable borrow               | `ref mut x`              | `&mut T`               |
| `&U` and you want to move `U`                   | `&x`                     | `U`                    |
| `&mut U` and you want a *mutable* borrow to `U` | `&mut x`                 | `&mut U`               |


## mem::replace and drain(..)

```rust
mem::replace(evalue, value)
```

- std::mem::replace moves a value into the place pointed-to by a mutable 
reference and gives you the value that used to be there. 

```rust
for (key, value) in self.buckets.iter_mut().flat_map(|bucket| bucket.drain(..))
```

- Vec::drain(range) gives you a draining iterator: it takes a mutable borrow of 
the vector, yields each element in the specified range by value, and removes 
those elements from the vector.

## My understand on when to use lifetimes

1. When having a method that returns something that is connect with one or more
parameters inside the function. The return value has to outlive the parameters
that it's connected to.

2. Whenever there are references in structs.

Rust infers lifetimes most of the time, but you must spell them out whenever the 
compiler needs help proving that one reference may not outlive another. 

In practice that happens in four scenarios: 

    (1) returning a reference tied to input(s), 
    
    (2) storing references inside structs/enums/traits, 
    (3) building higher-order abstractions (iterators, callbacks, async, GATs) 
    whose own signatures contain references, and (4) drawing explicit boundaries 
    such as 'static or for<'a> HRTBs. 

All other explicit lifetimes—including PhantomData markers—are just variations 
of those four.

### Lifetime Elisions

1. Each reference in input position gets its own lifetime.

2. If exactly one input lifetime appears, use it for all elided output lifetimes.

3. When the method has &self / &mut self, that lifetime flows to the output.

### Special lifetimes

| Syntax    | Meaning & Typical Use                                                                | Caveats                                                                       |
| --------- | ------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------- |
| `'static` | Data lives for the whole program (string literals, globals) ([doc.rust-lang.org][1]) | Over-using it can hide bugs or force heap allocation ([stackoverflow.com][2]) |
| `'_`      | *Inferred* anonymous lifetime in patterns or types                                   | Cannot be used in trait object position                                       |
| `for<'a>` | HRTB: callee must work for *any* `'a`                                                | Needed for iterator adapters, async combinators ([doc.rust-lang.org][3])      |

### Trubleshooting checklist

1. Start from the compiler error—it usually prints the missing relationship.

2. Add one lifetime to every reference in the signature, compile, then remove the ones the compiler says are redundant (fastest iterative workflow).

3. Prefer ownership: sometimes returning String or Arc<T> sidesteps lifetimes entirely.

4. PhantomData not Compile-time only?—remember it influences auto-traits and drop order.

### Mental rules of thumb (“single-screen version”)

1. Returning references? — Add a lifetime.

2. Struct field is &T / slice / pointer? — Add a lifetime.

3. Trait or iterator needs to borrow from self longer than one call? — Add a lifetime or an HRTB.

4. Otherwise: try without. If the compiler is happy, the elision rules already did the job.

write about your learning about pointer to array
