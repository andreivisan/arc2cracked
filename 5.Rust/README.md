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

**The Character Type**

- Rust’s char type is four bytes in size and represents a Unicode scalar value, 
which means it can represent a lot more than just ASCII. Accented letters; 
Chinese, Japanese, and Korean characters; emoji; and zero-width spaces are all 
valid char values in Rust. 
- Unicode scalar values range from U+0000 to U+D7FF and U+E000 to U+10FFFF 
inclusive.

In Rust, there are several ways to declare and create a String. The String type is a growable, heap-allocated data structure that owns its contents, as opposed to string literals (&str) which are fixed-size and often stored in the program's read-only memory.

### Creating Strings in Rust

**Using String::from()**

The most common way to create a String is by using the `String::from()` method, which converts a string literal into an owned String:

```rust
let word = String::from("Hello, World!");
```

This creates a new String with the content "Hello, World!" and assigns it to the variable `word`.

**Using to_string() Method**

Another common approach is to use the `to_string()` method, which is available on any type that 
implements the `Display` trait, including string literals:

```rust
let greeting = "Hello, Rust!".to_string();
```

This converts the string literal into an owned String.

**Using String::new()**

You can create an empty String and then add content to it later:

```rust
let mut empty_string = String::new();
empty_string.push_str("Adding content");
```

This creates a new empty String and then appends text to it.

**Using String::with_capacity()**

If you know the approximate size your String will grow to, you can optimize memory allocation by 
pre-allocating capacity:

```rust
let mut s = String::with_capacity(25);
s.push_str("Hello");
```

This creates a String with an initial capacity of 25 bytes, which can help avoid reallocations as the 
String grows.

**Adding Content to Strings**

Once you have a String, you can modify it if it's declared as mutable:

**Using push_str() to Add Strings**

```rust
let mut s = String::from("Hello");
s.push_str(", world!");  // s now contains "Hello, world!"
```

**Using push() to Add a Single Character**

```rust
let mut s = String::from("Hello");
s.push('!');  // s now contains "Hello!"
```

**Using the + Operator or format! Macro for Concatenation**

```rust
let s1 = String::from("Hello, ");
let s2 = String::from("world!");
let s3 = s1 + &s2;  // Note: s1 has been moved here and can no longer be used

// Alternatively, using format! macro (which doesn't take ownership)
let s1 = String::from("Hello, ");
let s2 = String::from("world!");
let s3 = format!("{}{}", s1, s2);
```

**String vs &str**

It's important to understand the distinction between String and &str:

- **String**: Owned, mutable, heap-allocated string type that can grow or shrink in size.
- **&str**: String slice, immutable reference to a sequence of UTF-8 characters, often used for string 
literals or when you want to borrow a portion of a String.

When writing functions, it's common to accept &str parameters for flexibility, as both string literals 
and String can be passed (the latter via deref coercion).

**Examples of String Declaration and Usage**

**Basic String Creation**

```rust
fn main() {
    // Using String::from()
    let s1 = String::from("Hello");
    
    // Using to_string()
    let s2 = "World".to_string();
    
    // Using String::new() and then adding content
    let mut s3 = String::new();
    s3.push_str("Rust");
    
    println!("{}, {} {}!", s1, s2, s3);  // Outputs: Hello, World Rust!
}
```

**Working with Unicode**

Strings in Rust are UTF-8 encoded, so they can contain any valid Unicode characters:

```rust
let hello = String::from("こんにちは");  // Japanese
let heart = String::from("💖");         // Emoji
```

**Raw String Literals**

For strings with special characters that you don't want to escape, you can use raw string literals and 
then convert to String:

```rust
let raw_str = r#"C:\Program Files\Rust"#.to_string();
```

This is particularly useful for strings containing quotes or backslashes.

Remember that all Strings in Rust are valid UTF-8, and the String type is implemented as a wrapper around 
a vector of bytes with additional guarantees about UTF-8 encoding.

### Compound Types

- Compound types can group multiple values into one type. Rust has two primitive 
compound types: tuples and arrays.

**The Tuple Type**

- Groups together a number of values with a variety of types into one compound
type.

```rust
fn main() {
    let tup: (i32, f64, u8) = (500, 6.4, 1);
}
```

- The type annotations are optional.

- To get the individual values from a tuple we can use pattern mattching to 
destructure a touple value:

```rust
fn main() {
    let tup = (500, 6.4, 1);
    let (x, y, z) = tup;
    println!("The value of y is: {y}");
}
```

- We can also access a tuple element by using ```.``` followed by the index
of the value we want to access.

```rust
fn main() {
    let x: (i32, f64, u8) = (500, 6.4, 1);
    let five_hundred = x.0;
    let six_point_four = x.1;
    let one = x.2;
}
```

**The Array Type**

- Arrays in Rust have a fixed length.

```rust
fn main() {
    let a = [1, 2, 3, 4, 5];
}
```

- Arrays are useful when you want your data allocated on the stack rather than 
the heap.
- A vector is a similar collection type provided by the standard library that 
is allowed to grow or shrink in size.
- You write an array’s type using square brackets with the type of each
element, a semicolon, and then the number of elements in the array, like so:

```rust
let a: [i32; 5] = [1, 2, 3, 4, 5];
```

- You can also initialize an array to contain the same value for each element 
by specifying the initial value, followed by a semicolon, and then the length 
of the array in square brackets, as shown here:

```rust
// array of 5 elements all having the value of 3
let a = [3; 5];
```

- This is how vectors are used:

```rust
// Create a vector with initial values
let mut vec = vec![1, 2, 3];

// Add elements dynamically
vec.push(4); // Now contains [1, 2, 3, 4]

// Create with specified capacity
let mut vec_with_capacity = Vec::with_capacity(100);
```

- Memory Allocation

    - Arrays: Stack-allocated, which means they're stored directly in the 
    program's stack memory.
    - Vectors: Heap-allocated, with memory managed dynamically on the heap.

## Functions

- ```snake case``` convention for naming functions and vars.

### Parameters

```rust
fn another_function(x: i32) {
    println!("The value of x is: {x}");
}
```

- In function signatures, you must declare the type of each parameter.
- The return type for a function is declared using ```->```.

```rust
fn five() -> i32 {
    5
}
```

- In Rust there is no ```;``` for the return statement.

## Control Flow

- if-else statements are the same as in Java.
- The logical operands are the same as in Java only that we are not using
parenthesis.

**Using if in a let Statement**

```rust
let number = if condition { 5 } else { 6 };
```

**Repetition with Loops**

- It's done using ```loop``` keyword.

```rust
fn main() {
    loop {
        println!("again!");
    }
}
```

- ```break``` and ```continue``` inside a loop act like in Java.
- We can return value from a loop with break:

```rust
fn main() {
    let mut counter = 0;
    let result = loop {
        counter += 1;
        if counter == 10 {
            break counter * 2;
        }
    };
    println!("The result is {result}");
}
```

**Loop Labels to Disambiguate Between Multiple Loops**

- If you have loops within loops, break and continue apply to the innermost
loop at that point. You can optionally specify a loop label on a loop that you
can then use with break or continue to specify that those keywords apply to
the labeled loop instead of the innermost loop. Loop labels must begin with
a single quote. Here’s an example with two nested loops:

```rust
fn main() {
    let mut count = 0;
    'counting_up: loop {
        println!("count = {count}");
        let mut remaining = 10;
        loop {
            println!("remaining = {remaining}");
            if remaining == 9 {
                break;
            }
            if count == 2 {
                break 'counting_up;
            }
            remaining -= 1;
        }
        count += 1;
    }
    println!("End count = {count}");
}
```

**Conditional Loops with while**

```rust
fn main() {
    let mut number = 3;
    while number != 0 {
        println!("{number}!");
        number -= 1;
    }
    println!("LIFTOFF!!!");
}
```

**Looping Through a Collection with for**

```rust
fn main() {
    let a = [10, 20, 30, 40, 50];
    for element in a {
        println!("the value is: {element}");
    }
}
```

or 

```rust
fn main() {
    for number in (1..4).rev() {
        println!("{number}!");
    }
    println!("LIFTOFF!!!");
}
```
## Ownership

- Set of rules that govern how a Rust program manages memory.
- Data with fixed size, stored on the stack.
- Data with variable size stored on the Heap. When storing on the Heap a block
of memory has to be allocated an the pointer to this block is returned. The
pointer is of fixed size so it can be store on the stack.
- Pushing to the stack is faster than allocating on the heap because the allcator
has to search for a place to store the data. Also the stack is closer to the
processor than the heap.
- Keeping track of what parts of code are using what data on the heap,
minimizing the amount of duplicate data on the heap, and cleaning up unused
data on the heap so you don’t run out of space are all problems that ownership
addresses.

### Ownership rule

- Each value in Rust has an owner.
- There can only be an owner at a time.
- When the owner goes out of scope, the value will be dropped.
