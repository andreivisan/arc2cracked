# Rust programming language

## 1. Variables

- By default variables are mutable. 
- Adding `mut` to a variable makes it mutable.

```rust
let mut x = 7;
```
- Constants are also immutable, but unlike vars they can't be made mutable. The
data type of constants MUST be annotated. 

```rust
const THREE_HOURS_IN_SECONDS: u32 = 60 * 60 * 3;
```


