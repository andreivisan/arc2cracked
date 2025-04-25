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

**Variables and Data Interacting with Move**

```rust
let x = 5;
let y = x;
```

- Because both of the above values are residing on the stack we bind the value
5 to x then make a copy of the value in x and bind it to y.

```rust
let s1 = String::from("hello");
let s2 = s1;
```

- A String is made up of three parts, shown on the left: a pointer to
the memory that holds the contents of the string, a length, and a capacity.
This group of data is stored on the stack. On the right is the memory on the
heap that holds the contents.

![String mem Rust](./images/string_mem_rust.png)

- When performing ```s2 = s1``` we basically create a new pointer that points
to the index 0 of the same heap value.
- Because in Rust, when a variable goes out of scope Rust automatically cleans
the heap which in the case above will cause a *double free* error.
- What Rust will do in the case above, it will invalidate the s1 variable, so
only s2 will now point at the heap memory block. Thus, if after s1 is still used,
Rust will throw an error.

**Variables and Data Interacting with Clone**

- If we do want to deeply copy the heap data of the String, not just the stack
data, we can use a common method called clone.

```rust
let s1 = String::from("hello");
let s2 = s1.clone();
println!("s1 = {s1}, s2 = {s2}");
```

- Rust has a special annotation called the Copy trait that we can place on
types that are stored on the stack, as integers are.
- If a type implements the Copy trait, variables that use
it do not move, but rather are trivially copied, making them still valid after
assignment to another variable.
- Rust won’t let us annotate a type with Copy if the type, or any of its parts,
has implemented the Drop trait.
- Rust does let us return multiple values using a tuple.

### References and Borrowing

- Instead of using functions to pass ownership between variables, in Rust we
can use *references*.
- A *reference* is like a pointer in that it’s an address we can follow to access 
the data stored at that address; that data is owned by some other variable.
- Unlike a pointer, a reference is guaranteed to point to a valid value of a 
particular type for the life of that reference.

```rust
fn main() {
    let s1 = String::from("hello");
    let len = calculate_length(&s1);
    println!("The length of '{s1}' is {len}.");
    fn calculate_length(s: &String) -> usize {
        s.len()
    }
}
```
![Reference in Rust](./images/reference_rust.png)

> Note: *The opposite of referencing by using & is dereferencing, which is 
accomplished with the dereference operator, `*`.*

- We call the action of creating a reference borrowing. As in real life, if a
person owns something, you can borrow it from them. When you’re done,
you have to give it back. You don’t own it.

- Just as variables are immutable by default, so are references. We’re not
allowed to modify something we have a reference to.

**Mutable References**

- We can create mutable references:

```rust
fn main() {
    let mut s = String::from("hello");
    change(&mut s);
    fn change(some_string: &mut String) {
        some_string.push_str(", world");
    }
}
```
- First we change s to be mut. Then we create a mutable reference with
&mut s where we call the change function, and update the function signature
to accept a mutable reference with some_string: &mut String. This makes it
very clear that the change function will mutate the value it borrows.
- Mutable references have one big restriction: if you have a mutable refer-
ence to a value, you can have no other references to that value.
- As always, we can use curly brackets to create a new scope, allowing for
multiple mutable references, just not simultaneous ones:

```rust
let mut s = String::from("hello");
{
    let r1 = &mut s;
} // r1 goes out of scope here, so we can make a new reference with no problems
let r2 = &mut s;
```

- We also cannot have a mutable reference while we have an immutable one to 
the same value.
- Note that a reference’s scope starts from where it is introduced and
continues through the last time that reference is used. For instance, this
code will compile because the last usage of the immutable references, the
println!, occurs before the mutable reference is introduced:

```rust
let mut s = String::from("hello");
let r1 = &s; // no problem
let r2 = &s; // no problem
println!("{r1} and {r2}");
// Variables r1 and r2 will not be used after this point.
let r3 = &mut s; // no problem
println!("{r3}");
```

**The Rules of References**

- At any given time, you can have either one mutable reference or any
number of immutable references.
- References must always be valid.

### The Slice Type

- *Slices* let you reference a contiguous sequence of elements in a collection
rather than the whole collection. A slice is a kind of reference, so it does not
have ownership.

- Here’s a small programming problem: write a function that takes a string
of words separated by spaces and returns the first word it finds in that string.
If the function doesn’t find a space in the string, the whole string must be
one word, so the entire string should be returned.

```rust
fn first_word(s: &String) -> usize {
    let bytes = s.as_bytes();
    for ( i, &item) in bytes.iter().enumerate() {
        if item == b' ' {
            return i;
        }
    }
    s.len()
}
```

- The solution above is problematic if the string changes.

**String Slices**

- A string slice is a reference to part of a String, and it looks like this:

```rust
let s = String::from("hello world");
let hello = &s[0..5];
let world = &s[6..11];
```

- We create slices using a range within brackets by specifying 
[starting_index..ending_index], where starting_index is the first position 
in the slice and ending_index is one more than the last position in the slice.

![Rust Slices](./images/slices_rust.png)

- These are equal:

```rust
let s = String::from("hello");
let slice = &s[0..2];
let slice = &s[..2];
```

>Note: *String slice range indices must occur at valid UTF-8 character boundaries. If you
attempt to create a string slice in the middle of a multibyte character, your program
will exit with an error.*

-Improving the first_word function by using a string slice for the type of the s
parameter, because it allows us to use the same function on both &String values 
and &str values.

```rust
fn first_word(s: &str) -> &str {
    //...
}
```

- Defining a function to take a string slice instead of a reference to
a String makes our API more general and useful without losing any
functionality.

```rust
fn main() {
    let my_string = String::from("hello world");
    // `first_word` works on slices of `String`s, whether partial
    // or whole.
    let word = first_word(&my_string[0..6]);
    let word = first_word(&my_string[..]);
    // `first_word` also works on references to `String`s, which
    // are equivalent to whole slices of `String`s.
    let word = first_word(&my_string);
    let my_string_literal = "hello world";
    // `first_word` works on slices of string literals,
    // whether partial or whole.
    let word = first_word(&my_string_literal[0..6]);
    let word = first_word(&my_string_literal[..]);
    // Because string literals *are* string slices already,
    // this works too, without the slice syntax!
    let word = first_word(my_string_literal);
}
```

**Other Slices**

```rust
let a = [1, 2, 3, 4, 5];
let slice = &a[1..3];
assert_eq!(slice, &[2, 3]);
```

## Using Structs to Structure Related Data

- A struct, or structure, is a custom data type that lets you package together 
and name multiple related values that make up a meaningful group. 

```rust
struct User {
    active: bool,
    username: String,
    email: String,
    sign_in_count: u64,
}
```

- To use a struct after we’ve defined it, we create an instance of that struct by
specifying concrete values for each of the fields.

```rust
fn main() {
    let user1 = User {
        active: true,
        username: String::from("someusername123"),
        email: String::from("someone@example.com"),
        sign_in_count: 1,
    };
}
```

- To get a specific value from a struct, we use dot notation. For example, to 
access this user’s email address, we use user1.email. If the instance is 
mutable, we can change a value by using the dot notation and assigning into a 
particular field.

```rust
fn main() {
    let mut user1 = User {
        active: true,
        username: String::from("someusername123"),
        email: String::from("someone@example.com"),
        sign_in_count: 1,
    };

    user1.email = String::from("anotheremail@example.com");
}
```

- Note that the entire instance must be mutable; Rust doesn’t allow us to mark 
only certain fields as mutable. 

```rust
fn build_user(email: String, username: String) -> User {
    User {
        active: true,
        username: username,
        email: email,
        sign_in_count: 1,
    }
}
```

**Using the Field Init Shorthand**

- Because the parameter names and the struct field names are exactly the same 
in the code above, we can use the field init shorthand syntax to rewrite 
build_user so it behaves exactly the same but doesn’t have the repetition of 
username and email.

```rust
fn build_user(email: String, username: String) -> User {
    User {
        active: true,
        username,
        email,
        sign_in_count: 1,
    }
}
```

**Creating Instances from Other Instances with Struct Update Syntax**

- It’s often useful to create a new instance of a struct that includes most of 
the values from another instance, but changes some. You can do this using 
struct update syntax.

```rust
fn main() {
    // --snip--

    let user2 = User {
        active: user1.active,
        username: user1.username,
        email: String::from("another@example.com"),
        sign_in_count: user1.sign_in_count,
    };
}
```

- Using struct update syntax, we can achieve the same effect with less code.
- The syntax .. specifies that the remaining fields not explicitly set should 
have the same value as the fields in the given instance.

```rust
fn main() {
    // --snip--

    let user2 = User {
        email: String::from("another@example.com"),
        ..user1
    };
}
```






