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

**Using Tuple Structs Without Named Fields to Create Different Types**

- Rust also supports structs that look similar to tuples, called tuple structs. 
Tuple structs have the added meaning the struct name provides but don’t have 
names associated with their fields; rather, they just have the types of the fields.

```rust
struct Color(i32, i32, i32);
struct Point(i32, i32, i32);

fn main() {
    let black = Color(0, 0, 0);
    let origin = Point(0, 0, 0);
}
```

**Unit-Like Structs Without Any Fields**

- You can also define structs that don’t have any fields! These are called 
unit-like structs because they behave similarly to ().
- Unit-like structs can be useful when you need to implement a trait on some 
type but don’t have any data that you want to store in the type itself.

```rust
struct AlwaysEqual;

fn main() {
    let subject = AlwaysEqual;
}
```

**Example structs & borrowing them**

```rust
struct Rectangle {
    width: u32,
    height: u32,
}

fn main() {
    let rect1 = Rectangle {
        width: 30,
        height: 50,
    };

    println!(
        "The area of the rectangle is {} square pixels.",
        area(&rect1)
    );
}

fn area(rectangle: &Rectangle) -> u32 {
    rectangle.width * rectangle.height
}
```

- Our area function is now defined with one parameter, which we’ve named 
rectangle, whose type is an immutable borrow of a struct Rectangle instance. 
As mentioned in Chapter 4, we want to borrow the struct rather than take 
ownership of it. This way, main retains its ownership and can continue using 
rect1, which is the reason we use the & in the function signature and where we 
call the function.
- The area function accesses the width and height fields of the Rectangle 
instance (note that accessing fields of a borrowed struct instance does not 
move the field values, which is why you often see borrows of structs).

**Adding Useful Functionality with Derived Traits**

- If we want to print a rectangle ``` println!("rect1 is {}", rect1);``` we 
will get an error.
- To fix:

```rust
#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}

fn main() {
    let rect1 = Rectangle {
        width: 30,
        height: 50,
    };

    println!("rect1 is {rect1:?}");
}
```

- Now Rectangle implements Debug traits, and using ```:?```. Putting the 
specifier ```:?``` inside the curly brackets tells println! we want to use an 
output format called Debug.
- Another way to print out a value using the Debug format is to use the dbg! 
macro, which takes ownership of an expression (as opposed to println!, 
which takes a reference), prints the file and line number of where that dbg! 
macro call occurs in your code along with the resultant value of that expression, 
and returns ownership of the value.

> Note: Calling the dbg! macro prints to the standard error console stream 
(stderr), as opposed to println!, which prints to the standard output console 
stream (stdout).

```rust
#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}

fn main() {
    let scale = 2;
    let rect1 = Rectangle {
        width: dbg!(30 * scale),
        height: 50,
    };

    dbg!(&rect1);
}
```

### Method Syntax

- Methods are similar to functions: we declare them with the fn keyword and 
a name, they can have parameters and a return value, and they contain some code 
that’s run when the method is called from somewhere else. Unlike functions, 
methods are defined within the context of a struct (or an enum or a trait 
object, and their first parameter is always self, which represents the instance 
of the struct the method is being called on.

```rust
#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}

impl Rectangle {
    fn area(&self) -> u32 {
        self.width * self.height
    }
}

fn main() {
    let rect1 = Rectangle {
        width: 30,
        height: 50,
    };

    println!(
        "The area of the rectangle is {} square pixels.",
        rect1.area()
    );
}
```

- Often, but not always, when we give a method the same name as a field we 
want it to only return the value in the field and do nothing else. Methods 
like this are called getters, and Rust does not implement them automatically 
for struct fields as some other languages do. Getters are useful because you 
can make the field private but the method public, and thus enable read-only 
access to that field as part of the type’s public API. 

- Rust has a feature called automatic referencing and dereferencing. Calling 
methods is one of the few places in Rust with this behavior.
- Here’s how it works: when you call a method with object.something(), Rust 
automatically adds in &, &mut, or * so object matches the signature of the 
method. In other words, the following are the same:

```rust
p1.distance(&p2);
(&p1).distance(&p2);
```

**Associated Functions**

- All functions defined within an impl block are called associated functions 
because they’re associated with the type named after the impl. We can define 
associated functions that don’t have self as their first parameter (and thus 
are not methods) because they don’t need an instance of the type to work with. 
We’ve already used one function like this: the String::from function that’s 
defined on the String type.
- Associated functions that aren’t methods are often used for constructors that 
will return a new instance of the struct. These are often called new, but new 
isn’t a special name and isn’t built into the language.

```rust
impl Rectangle {
    fn square(size: u32) -> Self {
        Self {
            width: size,
            height: size,
        }
    }
}
```

- To call this associated function, we use the :: syntax with the struct name; 
```let sq = Rectangle::square(3);```

## Enums and Pattern Matching

- Where structs give you a way of grouping together related fields and data, 
like a Rectangle with its width and height, enums give you a way of saying a 
value is one of a possible set of values.

```rust
enum IpAddrKind {
    V4,
    V6,
}
```

**Enum Values**

```rust
let four = IpAddrKind::V4;
let six = IpAddrKind::V6;
```

- Combine with structs so we can store their values.

```rust
    enum IpAddrKind {
        V4,
        V6,
    }

    struct IpAddr {
        kind: IpAddrKind,
        address: String,
    }

    let home = IpAddr {
        kind: IpAddrKind::V4,
        address: String::from("127.0.0.1"),
    };

    let loopback = IpAddr {
        kind: IpAddrKind::V6,
        address: String::from("::1"),
    };
```

- However, representing the same concept using just an enum is more concise: 
rather than an enum inside a struct, we can put data directly into each enum 
variant. This new definition of the IpAddr enum says that both V4 and V6 
variants will have associated String values:

```rust
    enum IpAddr {
        V4(String),
        V6(String),
    }

    let home = IpAddr::V4(String::from("127.0.0.1"));

    let loopback = IpAddr::V6(String::from("::1"));
```

-  If we wanted to store V4 addresses as four u8 values but still express V6 
addresses as one String value, we wouldn’t be able to with a struct. Enums 
handle this case with ease:

```rust
    enum IpAddr {
        V4(u8, u8, u8, u8),
        V6(String),
    }

    let home = IpAddr::V4(127, 0, 0, 1);

    let loopback = IpAddr::V6(String::from("::1"));
```

- Let’s look at another example of an enum in Listing 6-2: this one has a wide 
variety of types embedded in its variants.

```rust
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}
```

- The following structs could hold the same data that the preceding enum 
variants hold:

```rust
struct QuitMessage; // unit struct
struct MoveMessage {
    x: i32,
    y: i32,
}
struct WriteMessage(String); // tuple struct
struct ChangeColorMessage(i32, i32, i32); // tuple struct
```

- There is one more similarity between enums and structs: just as we’re able to 
define methods on structs using impl, we’re also able to define methods on 
enums. Here’s a method named call that we could define on our Message enum:

```rust
    impl Message {
        fn call(&self) {
            // method body would be defined here
        }
    }

    let m = Message::Write(String::from("hello"));
    m.call();
```

**The Option Enum and Its Advantages Over Null Values**

- The Option type encodes the very common scenario in which a value could be 
something or it could be nothing.

```rust
enum Option<T> {
    None,
    Some(T),
}

let some_number = Some(5);
let some_char = Some('e');

let absent_number: Option<i32> = None;
```

- For absent_number, Rust requires us to annotate the overall Option type: 
the compiler can’t infer the type that the corresponding Some variant will hold 
by looking only at a None value.

### The match Control Flow Construct

- Rust has an extremely powerful control flow construct called match that allows 
you to compare a value against a series of patterns and then execute code based 
on which pattern matches.

```rust
enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter,
}

fn value_in_cents(coin: Coin) -> u8 {
    match coin {
        Coin::Penny => 1,
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter => 25,
    }
}
```

- The ```match``` has ``` arms```. An arm has two parts: a pattern and some 
code. The first arm here has a pattern that is the value Coin::Penny and then 
the => operator that separates the pattern and the code to run. The code in 
this case is just the value 1. Each arm is separated from the next with a comma.
- If you want to run multiple lines of code in a match arm, you must use curly 
brackets, and the comma following the arm is then optional.

```rust
fn value_in_cents(coin: Coin) -> u8 {
    match coin {
        Coin::Penny => {
            println!("Lucky penny!");
            1
        }
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter => 25,
    }
}
```

**Patterns That Bind to Values**

- Another useful feature of match arms is that they can bind to the parts of 
the values that match the pattern. This is how we can extract values out of 
enum variants.

```rust
#[derive(Debug)] // so we can inspect the state in a minute
enum UsState {
    Alabama,
    Alaska,
    // --snip--
}

enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter(UsState),
}
```

- In the match expression for this code, we add a variable called state to the 
pattern that matches values of the variant Coin::Quarter. When a Coin::Quarter 
matches, the state variable will bind to the value of that quarter’s state. 
Then we can use state in the code for that arm, like so:
- If we were to call value_in_cents(Coin::Quarter(UsState::Alaska)), coin would 
be Coin::Quarter(UsState::Alaska). When we compare that value with each of the 
match arms, none of them match until we reach Coin::Quarter(state). At that 
point, the binding for state will be the value UsState::Alaska. We can then use 
that binding in the println! expression, thus getting the inner state value out 
of the Coin enum variant for Quarter.
- Basically we can create a match to match the enum value together with the 
binding.

**Matching with Option<T>**

- Let’s say we want to write a function that takes an Option<i32> and, if 
there’s a value inside, adds 1 to that value. If there isn’t a value inside, 
the function should return the None value and not attempt to perform any 
operations.

```rust
    fn plus_one(x: Option<i32>) -> Option<i32> {
        match x {
            None => None,
            Some(i) => Some(i + 1),
        }
    }

    let five = Some(5);
    let six = plus_one(five);
    let none = plus_one(None);
```

**Matches Are Exhaustive**

- There’s one other aspect of match we need to discuss: the arms’ patterns must 
cover all possibilities. Consider this version of our plus_one function, which 
has a bug and won’t compile:

```rust
    fn plus_one(x: Option<i32>) -> Option<i32> {
        match x {
            Some(i) => Some(i + 1),
        }
    }
```

**Catch-All Patterns and the _ Placeholder**

- Using enums, we can also take special actions for a few particular values, 
but for all other values take one default action. 

1. If we want to use the value:

```rust
    let dice_roll = 9;
    match dice_roll {
        3 => add_fancy_hat(),
        7 => remove_fancy_hat(),
        other => move_player(other),
    }

    fn add_fancy_hat() {}
    fn remove_fancy_hat() {}
    fn move_player(num_spaces: u8) {}
```

2. If we do not want to use a value but we want a function to be called for all
other values:

```rust
    let dice_roll = 9;
    match dice_roll {
        3 => add_fancy_hat(),
        7 => remove_fancy_hat(),
        _ => reroll(),
    }

    fn add_fancy_hat() {}
    fn remove_fancy_hat() {}
    fn reroll() {}
```

3. If we do not want to use a value and we want nothing to happen:

```rust
    let dice_roll = 9;
    match dice_roll {
        3 => add_fancy_hat(),
        7 => remove_fancy_hat(),
        _ => (),
    }

    fn add_fancy_hat() {}
    fn remove_fancy_hat() {}
```

**Concise Control Flow with if let and let else**

- The if let syntax lets you combine if and let into a less verbose way to 
handle values that match one pattern while ignoring the rest.

Having:

```rust
    let config_max = Some(3u8);
    if let Some(max) = config_max {
        println!("The maximum is configured to be {max}");
    }
```

We can transform into:

```rust
    let config_max = Some(3u8);
    if let Some(max) = config_max {
        println!("The maximum is configured to be {max}");
    }
```

- We can include an else with an if let. The block of code that goes with the 
else is the same as the block of code that would go with the _ case in the 
match expression that is equivalent to the if let and else.

For:

```rust
    let mut count = 0;
    match coin {
        Coin::Quarter(state) => println!("State quarter from {state:?}!"),
        _ => count += 1,
    }
```

We can have:

```rust
    let mut count = 0;
    if let Coin::Quarter(state) = coin {
        println!("State quarter from {state:?}!");
    } else {
        count += 1;
    }
```

### Staying on the “Happy Path” with let...else

- One common pattern is to perform some computation when a value is present and 
return a default value otherwise.

```rust
impl UsState {
    fn existed_in(&self, year: u16) -> bool {
        match self {
            UsState::Alabama => year >= 1819,
            UsState::Alaska => year >= 1959,
            // -- snip --
        }
    }
}
```

- Then we might use if let to match on the type of coin, introducing a state 
variable within the body of the condition.

```rust
fn describe_state_quarter(coin: Coin) -> Option<String> {
    if let Coin::Quarter(state) = coin {
        if state.existed_in(1900) {
            Some(format!("{state:?} is pretty old, for America!"))
        } else {
            Some(format!("{state:?} is relatively new."))
        }
    } else {
        None
    }
}
```

- That gets the job done, but it has pushed the work into the body of the if 
let statement, and if the work to be done is more complicated, it might be hard 
to follow exactly how the top-level branches relate.

```rust
fn describe_state_quarter(coin: Coin) -> Option<String> {
    let Coin::Quarter(state) = coin else {
        return None;
    };

    if state.existed_in(1900) {
        Some(format!("{state:?} is pretty old, for America!"))
    } else {
        Some(format!("{state:?} is relatively new."))
    }
}
```

## Managing Growing Projects with Packages, Crates, and Modules

### Packages and Crates

- A crate is the smallest amount of code that the Rust compiler considers at a 
time. Even if you run rustc rather than cargo and pass a single source code 
file (as we did all the way back in “Writing and Running a Rust Program” in 
Chapter 1), the compiler considers that file to be a crate.
- Crates can contain modules, and the modules may be defined in other files 
that get compiled with the crate.
- A crate can come in one of two forms: a binary crate or a library crate.

1. *Binary crates* are programs you can compile to an executable that you can 
run, such as a command line program or a server. Each must have a function 
called main that defines what happens when the executable runs. All the crates 
we’ve created so far have been binary crates.

2. *Libraru crates* don’t have a main function, and they don’t compile to an 
executable. Instead, they define functionality intended to be shared with 
multiple projects. For example, the rand crate we used in Chapter 2 provides 
functionality that generates random numbers. Most of the time when Rustaceans 
say “crate”, they mean library crate, and they use “crate” interchangeably with 
the general programming concept of a “library”.

- The *crate root* is a source file that the Rust compiler starts from and 
makes up the root module of your crate.

- A *package* is a bundle of one or more crates that provides a set of 
functionality. A package contains a Cargo.toml file that describes how to build 
those crates. 

- Cargo is actually a package that contains the binary crate for the command 
line tool you’ve been using to build your code. The Cargo package also contains 
a library crate that the binary crate depends on. Other projects can depend on 
the Cargo library crate to use the same logic the Cargo command line tool uses.

> ‼️  A package can contain as many binary crates as you like, but at most only 
one library crate. A package must contain at least one crate, whether that’s a 
library or binary crate.

```bash
$ cargo new my-project
     Created binary (application) `my-project` package
$ ls my-project
Cargo.toml
src
$ ls my-project/src
main.rs
```

Here, we have a package that only contains src/main.rs, meaning it only 
contains a binary crate named my-project. If a package contains src/main.rs and 
src/lib.rs, it has two crates: a binary and a library, both with the same name 
as the package. A package can have multiple binary crates by placing files in 
the src/bin directory: each file will be a separate binary crate.

### Defining Modules to Control Scope and Privacy

**Modules Cheat Sheet**

- **Start from the crate root**: When compiling a crate, the compiler first 
looks in the crate root file (usually src/lib.rs for a library crate or 
src/main.rs for a binary crate) for code to compile.
- **Declaring modules**: In the crate root file, you can declare new modules; 
say you declare a “garden” module with mod garden;. The compiler will look for 
the module’s code in these places:
    - Inline, within curly brackets that replace the semicolon following mod 
    garden
    - In the file src/garden.rs
    - In the file src/garden/mod.rs
- **Declaring submodules**: In any file other than the crate root, you can 
declare submodules. For example, you might declare mod vegetables; in 
src/garden.rs. The compiler will look for the submodule’s code within the 
directory named for the parent module in these places:
    - Inline, directly following mod vegetables, within curly brackets instead 
    of the semicolon
    - In the file src/garden/vegetables.rs
    - In the file src/garden/vegetables/mod.rs
- **Paths to code in modules**: Once a module is part of your crate, you can 
refer to code in that module from anywhere else in that same crate, as long as 
the privacy rules allow, using the path to the code. For example, an Asparagus 
type in the garden vegetables module would be found at 
```crate::garden::vegetables::Asparagus```.
- **Private vs. public**: Code within a module is private from its parent 
modules by default. To make a module public, declare it with pub mod instead of 
mod. To make items within a public module public as well, use pub before their 
declarations.
- **The use keyword**: Within a scope, the use keyword creates shortcuts to 
items to reduce repetition of long paths. In any scope that can refer to 
crate::garden::vegetables::Asparagus, you can create a shortcut with use 
crate::garden::vegetables::Asparagus; and from then on you only need to write 
Asparagus to make use of that type in the scope.

Here, we create a binary crate named backyard that illustrates these rules. 
The crate’s directory, also named backyard, contains these files and 
directories:

```text
backyard
├── Cargo.lock
├── Cargo.toml
└── src
    ├── garden
    │   └── vegetables.rs
    ├── garden.rs
    └── main.rs
```

The crate root file in this case is src/main.rs, and it contains:

Filename: src/main.rs

```rust
use crate::garden::vegetables::Asparagus;

pub mod garden;

fn main() {
    let plant = Asparagus {};
    println!("I'm growing {plant:?}!");
}
```

The pub mod garden; line tells the compiler to include the code it finds in src/garden.rs, which is:

Filename: src/garden.rs

```rust
pub mod vegetables;
```

Here, pub mod vegetables; means the code in src/garden/vegetables.rs is 
included too. That code is:

```rust
#[derive(Debug)]
pub struct Asparagus {}
```

**Grouping Related Code in Modules**

- Modules let us organize code within a crate for readability and easy reuse.
- Modules also allow us to control the privacy of items because code within a 
module is private by default.
- We can choose to make modules and the items within them public, which exposes 
them to allow external code to use and depend on them.

*Example*: Creation of a restaurant project.

```bash
cargo new restaurant --lib
```

Filename: src/lib.rs

```rust
mod front_of_house {
    mod hosting {
        fn add_to_waitlist() {}

        fn seat_at_table() {}
    }

    mod serving {
        fn take_order() {}

        fn serve_order() {}

        fn take_payment() {}
    }
}
```

- Modules can also hold definitions for other items, such as structs, enums, 
constants, traits, and functions.

- Earlier, we mentioned that src/main.rs and src/lib.rs are called crate roots. 
The reason for their name is that the contents of either of these two files form 
a module named crate at the root of the crate’s module structure, known as the 
module tree.

Below the module tree for the structure in the code above:

```text
crate
 └── front_of_house
     ├── hosting
     │   ├── add_to_waitlist
     │   └── seat_at_table
     └── serving
         ├── take_order
         ├── serve_order
         └── take_payment
```

### Paths for Referring to an Item in the Module Tree

- To show Rust where to find an item in a module tree, we use a path in the 
same way we use a path when navigating a filesystem. To call a function, 
we need to know its path.

- A path can take two forms:
    - An *absolute path* is the full path starting from a crate root; for code 
    from an external crate, the absolute path begins with the crate name, and 
    for code from the current crate, it starts with the literal crate.
    - A *relative path* starts from the current module and uses self, super, or 
    an identifier in the current module.
- Both absolute and relative paths are followed by one or more identifiers 
separated by double colons (::).

We’ll show two ways to call the add_to_waitlist function from a new function, 
eat_at_restaurant, defined in the crate root.

```rust
mod front_of_house {
    mod hosting {
        fn add_to_waitlist() {}
    }
}

pub fn eat_at_restaurant() {
    // Absolute path
    crate::front_of_house::hosting::add_to_waitlist();

    // Relative path
    front_of_house::hosting::add_to_waitlist();
}
```








## Useful frameworks

[Web - Actix](https://actix.rs/)

[TUI - ratatui](https://ratatui.rs/)
