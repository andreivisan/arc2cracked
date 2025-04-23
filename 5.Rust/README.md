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

- Constants expressions can be evaluated at compile time.

### Shadowing

- When declaring a new variable with the name of a previous variable, the second
variable overshadows the first, the Rust compiler will only use the second. That
is called `Shadowing`.

## 2. Data Types

### Scalar Types

- A scalar type represents a single value. Rust has 4 primary scalar types:
integers, floating-point numbers, Booleans and characters.

**Integer Types**

| Length | Signed | Unsigned |
|--------|--------|----------|
| 8-bit  | i8     | u8       |
| 16-bit | i16    | u16      |
| 32-bit | i32    | u32      |
| 64-bit | i64    | u64      |
| 128-bit| i128   | u128     |
| arch   | isize  | usize    |
---

- Each signed variant can store numbers from –(2n – 1) to 2n – 1 inclusive,
where n is the number of bits that variant uses. So an i8 can store numbers
from –(27) to 27 – 1, which equals –128 to 127.

- The isize and usize types depend on the architecture of
the computer your program is running on, which is denoted in the table
as “arch”: 64 bits if you’re on a 64-bit architecture and 32 bits if you’re on a
32-bit architecture.

- Number literals can also use ```-``` asa visual separator to make the number 
easier to read, such as 1_000, whichwill have the same value as if you had 
specified 1000.

| Number literals | Example |
|----------------|---------|
| Decimal        | 98_222  |
| Hex            | 0xff    |
| Octal          | 0o77    |
| Binary         | 0b1111_0000 |
| Byte (u8 only) | b'A'    |
---

- Rust performs two’s complement wrapping.

- To explicitly handle the possibility of overflow, you can use these families
of methods provided by the standard library for primitive numeric types:

    - Wrap in all modes with the wrapping_* methods, such as wrapping_add.
    - Return the None value if there is overflow with the checked_* methods.
    - Return the value and a Boolean indicating whether there was overflow with
the overflowing_* methods.
    - Saturate at the value’s minimum or maximum values with the saturating_*
methods.

**Floating-Point Types**

- Rust’s floating-point types are f32 and f64, which are 32 bits and 64 bits 
in size, respectively.
- All floating-point types are signed.

**Numeric Operations**

- Integer division truncates toward zero to the nearest integer.

```rust
fn main() {
    // addition
    let sum = 5 + 10;
    // subtraction
    let difference = 95.5 - 4.3;
    // multiplication
    let product = 4 * 30;
    // division
    let quotient = 56.7 / 32.2;
    let truncated = -5 / 3; // Results in -1
    // remainder
    let remainder = 43 % 5;
}
```



