# C Programming

## LowLevelCode Zero2Hero C course

### Compilation Process

The process of converting source code to machine code is the following steps.

- Preprocessor
- Compilation
- Assembly
- Linking

### Strings

Strings are special. A string in C has a very special characteristic that makes it amazing, but also dangerous. 
All strings in C are ended with a null byte. If they don't end in a null byte, the operation you'll performing will complete going.

Why does that matter? If you run operations like strcpy, which copies data from the src buffer to the dest buffer. 
It will ONLY STOP COPYING OR PRINTING IF IT ENCOUNTERS THAT ZERO BYTE. So, the code above actually has a pretty major security vulnerability. 
You have to make sure your strings end with a zero, otherwise things will go sideways.

### Structure

If wanted to serialize this data and work on it between two different systems that might have different architectures, 
there's a chance that the structures may change between these systems. By adding special modifiers to the structures, 
we can ensure that the structure does not get modified in this way.

You can create a structure with the following syntax.

```c
struct mystruct {
    int i;
    char c;
};
```

Here, the structure should only be 5 bytes. But, there is a high chance that the structure is actually 8 bytes, or even 16 bytes due to alignment issues.

To fix this, we can add the packed attribute.

```c
struct __attribute__((__packed__)) mystruct {
    int i;
    char c;
};
```

This will create the same struct, but ensure that the compiler doesn't add any special sauce in between the elements 
so we can ensure it's the same size on multiple systems.

### Union

Unions are a field that assign multiple labels of multiple types to the same memory location.

You can create a structure with the following syntax.

```c
union myunion {
    int i;
    char c;
};
```

This will create a type called union myunion that we can use later on in the code. 
This structure has two members i and c, but both of them will contain data at the same location. 
The union is only the size of the largest element, in this case int i, or 4 bytes.

```c
union myunion {
    int i;
    char c;
};

int main() {
    union myunion u;
    u.i = 0x41424344;
    printf("%c"\n, u.c); 
    ...
}
```

Above, u.c will equal 0x44, which is the value in the location of i.

### Pointers

A pointer is a variable who's value equals the address of another variable.

To make a variable, first you need another variable that you want to point to. 
Lets say we have an integer that we'd like to reference.

```c
int x = 3;
```

To create a pointer that points to x, all we'd have to do is create a pointer and set it's value 
equal to the address of x. We can do that with the following syntax.

```c
int x = 3;
int *pX = &x;
```

Here, the syntax is as follows. The * character notes that the type is a pointer. 
The & character gets the "address of", which gets the address of x and puts that value in pX.

To use a pointer, all you have to do is "dereference" it. To dereference a pointer, 
simply add the * to the name of the pointer to get the value at that location.

```c
int x = 3;
int *pX = &x;
printf("%d\n", *pX);
```

## Dynamic Memory Allocation

In previous examples we showed how to allocate memory via an array or create memory space using structures. 
The problem with both of these solutions to allocating memory is that their sizes are static, 
and have to be known at compile time. Because of this, we end up allocating too much space and 
not being able to allocate any more if our program grows.

### The Heap

The heap is a data region in ELF (Linux) memory that is managed by a memory allocator. 
Instead of using statically defined arrays or structures, we can ask the memory allocator 
via a function called malloc to get us a certain amount of memory. For example: this code 
allocates 64 bytes of memory.

```c
malloc(64);
```

### Allocating Memory

To use this memory, we need to assign it to variable for use later. Also, its important that 
we check the return value from malloc to make sure that we got a value from the allocator. 
Sometimes, allocators fail.

```c
struct employee_t *my_employee = malloc(sizeof(struct employee_t));
if (my_employee == NULL) {
    printf("Something went wrong\n");
    return -1;
}
```

### Freeing Memory

We MUST give the memory back to the system. If we fail to free memory that we allocate, 
over time we will leak memory, and could possibly use up all available system memory and 
eventually have our process be killed by the operating system kernel. 
To avoid this, free your memory like so:

```c
...
free(my_employee);
my_employee = NULL;
```

It's not required, but always a smart idea to set used pointers to NULL. By setting pointers to NULL, 
we avoid what is called a dangling pointer, or a pointer that points to memory that is no longer valid.

## Static Memory Allocation

Sometimes we want data to exist in the scope of the lifetime of our entire program. 
For example, what if we want to track the amount of employees that have been initialized in our database 
so far? We have two options: global and static variables.

### Static Variables

A static variable is a variable that exists in a static lifetime, but does not have global scope. 
By creating a static variable, we are able to create variables that out live the lifetime of a function 
call and track data over multiple calls to the same function.

```c
int my_static_test() {
    static int i = 0;
    i++;
}
```

In the above function, i will be set to zero the first time the function is ran. When this happens,
the variable's data is actually stored not on the stack, but somewhere in the data section of the 
program that is running. Later calls to my_static_test will increase the variable every time, 
so it will be 1, then 2, then 3

**Static Use Case**

Let's say for example we wanted to track how many times a particular function was called, maybe the employee_initialize function for example.

```c
int employee_initialize (employee_t) {
    static int n_called = 0;
    ...
    
    return n_called++;
}
```

The above code will increment the variable every time the employee_initialize function was called, and return the current value to the user.
The idea is that when we declare a static var we initialize it in **global** scope but only we have access to it as opposed to a global variable.

### Pointers to pointers

A double pointer is a pointer that points to another pointer. In memory management, double pointers are particularly useful for functions that need to modify the original pointer, such as allocating or resizing dynamic memory.

#### Using Double Pointers with realloc

We want to create a function foo that resizes an array using realloc. The function should accept a double pointer to the array so that it can modify the original pointer.

```c
#include <stdio.h>
#include <stdlib.h>

typedef enum {
	STATUS_GOOD,
	STATUS_BAD,
} status_t;

status_t foo(int **arr, size_t new_size) {
    int *temp = realloc(*arr, new_size * sizeof(int));
    if (temp == NULL) {
        // Handle realloc failure
        printf("Memory allocation failed\n");
        return STATUS_BAD;
    }
    *arr = temp;
    return STATUS_GOOD;
}

int main() {
    size_t initial_size = 5;
    size_t new_size = 10;

    int *arr = malloc(initial_size * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Initialize the array
    for (size_t i = 0; i < initial_size; i++) {
        arr[i] = i;
    }

    // Print the initial array
    printf("Initial array:\n");
    for (size_t i = 0; i < initial_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Call foo to resize the array
    if (STATUS_BAD == foo(&arr, new_size)) {
		printf("uh oh");
		return -1;
	}

    // Initialize the new part of the array
    for (size_t i = initial_size; i < new_size; i++) {
        arr[i] = i;
    }

    // Print the resized array
    printf("Resized array:\n");
    for (size_t i = 0; i < new_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
```

1. Function Definition:

    - ```void foo(int **arr, size_t new_size)```: The function foo takes a double pointer arr and a new size new_size as parameters.

2. Reallocating Memory:

    - int ```*temp = realloc(*arr, new_size * sizeof(int));```: The ```realloc``` function attempts to resize the memory block pointed to by ```*arr``` to ```new_size``` elements.

    - If ```realloc``` fails, it returns ```NULL```. The original memory block is not freed in this case.

    - ```if (temp == NULL)```: Check if ```realloc``` failed.

    - ```*arr = temp;```: If ```realloc``` is successful, update the original pointer ```*arr``` to point to the new memory block.

3. Main Function:

    - Allocate initial memory using ```malloc```.
    - Initialize and print the initial array.
    - Call ```foo``` to resize the array.
    - Initialize and print the resized array.
    - Free the allocated memory.

Double pointers are essential in C for functions that need to modify the original pointer, such as those that allocate or resize dynamic memory. This lesson demonstrated how to use double pointers with ```realloc``` to resize an array in a function.

The idea is that C is pass by value. If we just pass a pointer to a function we basically pass a copy of the original pointer(address). If we pass a pointer to the
original pointer then we can change the original address.

**Visual Analogy**

Imagine a piece of paper (pointer) with an address written on it.

    - Without double pointer: You give someone a photocopy of the paper. They scribble a new address on the copy. Your original paper remains unchanged.
    - With double pointer: You give someone the location of your paper. They can directly edit the original address on your paper.

### Testing for Memory Leaks

#### Valgrind

Valgrind is a program that wraps the calls to malloc and free, tags them, and checks to see where memory is leaked or potentially lost.

To make your code compatible with valgrind, you'll need to compile with debug symbols in your code. Do this the following way.

```bash
gcc -o program code.c -g
```

The -g flag will add the debug symbols to your code. To test your code, run:

```bash
valgrind --leak-check=full ./program
```

Valgrind will run your code and provide a report of what memory did not get freed, and where the allocation happened.

Please note that you need to write tests to make your code execute lines that allocate from the heap to use valgrind correctly. 
If your code never allocates from the heap in your test case, it'll never see the memory leak.

### Glibc

Glibc, or the GNU C library, is the library that gets baked into every program you compile with gcc.

When we write code, we write userland code, or code that exists in the context of a userspace process. To the CPU, this code is unpriveleged, 
and as a result it can't really do anything fancy, like allocate memory, access the filesystem, or access the network.

To do privleged things, we ask the kernel (which runs as privleged code) to do it for us. This is done through what is called a "system call interface" 
where the syscall instruction is ran, and asks the kernel to perform a certain action.

The GNU C library cleanly wraps all of this functionality up into easy to use functions that wrap the otherwise hard to maintain system call functionality.

For example, when we allocate memory from the kernel for our process, we use malloc. Malloc internally calls the sbrk or mmap system calls, 
which asks the kernel through a system call to give us more memory. All of this abstracted away from us as the developer, 
and all we have to do is managed the result.

To see what libraries your program is linked against, try:

```bash
ldd ./myprogram
```

To check the documentation for functions in Glibc we need to use ```man <function_name>```. If the returned documentation is not for a function (could be a desc
of some OS thing) then use ```man 2 <function_name>```.

### File Descriptors

In Linux, every program you run has three file descriptors open by default

    - 0: stdin - a file that represents the input to your program from the command line
    - 1: stdout - a file that represents the output of your program to the command line
    - 2: stderr - a file that represents the output of your program, but containing only errors

### File Output

To write to a file, we need to first open the file. We do this using the open libc function to get a file descriptor to that file. 
We'll use that file descriptor in later functions to describe to the kernel what file we want to perform that action on.

#### open

To open a file, we specify the path that we want to open, as well as the type of open operation that we want to do. All of the types are described in the man page.

```c
...
int fd = open("./a-file", O_RDWR | O_CREAT, 0644);
if (fd == -1) {
    perror("open");
    return -1;
}
...
```

In the above code, we're asking the kernel to open the file ./a-file. 
If the file does not exist, create the file. Open it read write, with the linux octal permissions 0644, or rw-r--r-.

#### write

With the file open and the descriptor checked to be a valid value, we can use that descriptor in other functions. For example, write.

```c
char *a_buf = "some data\n";
write(fd, a_buf, strlen(a_buf));
```

The above function uses the descriptor from before, and writes the buffer at a_buf to that location. Doing this, we can check the file to see the contents.

### File Input

To read from a file, just like before we need to first open the file. We do this using the open libc function to get a file descriptor to that file.

```c
...
int fd = open("./my-db.db", O_RDONLY);
if (fd == -1) {
    perror("open");
    return -1;
}
...
```

#### read

With the file open and the descriptor checked to be a valid value, we can use that descriptor in other functions. 
For example, read. Here, we can use read to read the contents of that file into our database header. 
This is really cool, because instead of reading in some data and then converting the data, we can write it directly 
into our header structure and check the data.

```c
struct database_header head = {0};
...
read(fd, &head, sizeof(head));
printf("Database Version %d", head.version);
```

### Reading File Metadata

#### stat

stat is a linux system call that asks the kernel to report information about a specific file. To learn more about the specific details of stat, try man stat.

```bash
stat ./my-db.db
```

#### Validating User Input

We can use the stat family of functions, specifically fstat to check what the kernel has to say about the file, and then confirm that the metadata reported in the file head is the same.

```c
...
struct stat dbstat = {0};
if (fstat(fd, &stat) < 0) {
    perror("fstat");
    close(fd);
    return -1;
}
...
if (stat.st_size != head.size) {
    printf("HACKER DETECTED!\n");
    close(fd);
    return -1;
}
```

### Variable Types

#### Types

Here are the common types, their bit length, their signedness, and the associated size.

**C Data Type: char**

- Signed/Unsigned: Signed
- Bit Length: 8 bits
- Minimum Value: -128
- Maximum Value: 127

**C Data Type: unsigned char**

- Signed/Unsigned: Unsigned
- Bit Length: 8 bits
- Minimum Value: 0
- Maximum Value: 255

**C Data Type: short**

- Signed/Unsigned: Signed
- Bit Length: 16 bits
- Minimum Value: -32,768
- Maximum Value: 32,767

**C Data Type: unsigned short**

- Signed/Unsigned: Unsigned
- Bit Length: 16 bits
- Minimum Value: 0
- Maximum Value: 65,535

**C Data Type: int**

- Signed/Unsigned: Signed
- Bit Length: 16 or 32 bits
- Minimum Value: -2,147,483,648 (32-bit)
- Maximum Value: 2,147,483,647 (32-bit)

**C Data Type: unsigned int**

- Signed/Unsigned: Unsigned
- Bit Length: 16 or 32 bits
- Minimum Value: 0
- Maximum Value: 4,294,967,295 (32-bit)

**C Data Type: long**

- Signed/Unsigned: Signed
- Bit Length: 32 bits
- Minimum Value: -2,147,483,648
- Maximum Value: 2,147,483,647

**C Data Type: unsigned long**

- Signed/Unsigned: Unsigned
- Bit Length: 32 bits
- Minimum Value: 0
- Maximum Value: 4,294,967,295

**C Data Type: long long**

- Signed/Unsigned: Signed
- Bit Length: 64 bits
- Minimum Value: -9,223,372,036,854,775,808
- Maximum Value: 9,223,372,036,854,775,807

**C Data Type: unsigned long long**

- Signed/Unsigned: Unsigned
- Bit Length: 64 bits
- Minimum Value: 0
- Maximum Value: 18,446,744,073,709,551,615

**C Data Type: float**

- Bit Length: 32 bits
- Minimum Value: ~ -3.4 x 10^38
- Maximum Value: ~ 3.4 x 10^38

**C Data Type: double**

- Bit Length: 64 bits
- Minimum Value: ~ -1.7 x 10^308
- Maximum Value: ~ 1.7 x 10^308

### Type Casting

To type cast a variable, you do the following expression.

```c
int other_var = -1;
unsigned int x = (unsigned int)other_var;
```

However, typecasting variables of different types has effects on the variables that is important to understand.

#### Signedness Casting

```c
int other_var = -1;
unsigned int x = (unsigned int)other_var;
```

In this example, we cast a negative value to an unsigned value. It's important to understand under the hood how signedness will effect the value of this variable in an unsigned fashion. For example, -1 becomes MAX_INT in the following value.

#### Up Casting

```c
short other_var = -1;
int x = (unsigned int)other_var;
```

When you cast up a variable from one size to a larger size, the signedness comes with it. For example, here, despite shorts being 16 bits and integers being 32 bits, the value -1 will be sign extended through the larger variable.

#### Down Casting

```c
int x = 0xfffffefe;
short other_var = (short)int;
```

When you cast a larger variable to a smaller variable, the number will be truncated because you cannot fit the entire size of the larger variable into the smaller variable.

#### Floats

```c
float f = 3.14
int wasafloat = (int)f;
```

Floats or doubles can be cast to other variable types. It's important to understand that under the hood, assembly instructions are generated in the FPU of the processor to execute this conversion. In other casts, the data has not been significantly manipulated to enable the cast. In this case, the IEEE704 type must be manipulated to reveal the scalar value.

### Multi Module Programming

#### Modules 

We can think of our database project as a set of modules that interfaces with the user. Maybe we have:

- the main module: interfaces via stdin and stdout
- the file module: interfaces with the main module via an exposed interface and the filesystem
- the parsing module: reads data from opened files and parses them for the user

#### Header Files

Interfaces between the main module and the other modules are exposed in header files. Header files reveal the declarations, but not necesarily the definitions, for functions that we use in a module. Inside of a C file for a module, we will define the function, but the main module does not need to know about the internal functionality: just the inputs and outputs.

A sample header file could be:

```c
#ifndef FILE_H
#define FILE_H

int open_rw_file(char *path);

#endif
```

In this file, file.h, we expose the interface to the file module, without giving the definition of the function. To access these functions, we use headers in main.c

```c
...
#include "file.h"
```

### Build Systems

#### Make

make is a tool that, when ran, finds a local Makefile and executes the instructions to build a particular target.

Makefiles are organized around targets, rules, and instructions

```make
TARGET = bin/final
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

default: $(TARGET)
```

Here, default is a target, that says to be complete, first the $(TARGET) binary must be complete. Once that is complete, the instructions within default will be ran.

Make will take this logic forward and recursively run the functionality in the Makefile until the target is met.

```make
TARGET = bin/final
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude
```

Wildcard will capture all of our source files as a list in src/

Patsubst will execute a pattern substitution and convert all src/\*.c to obj/\*.o

#### Symbols

- $? represents a list of all input files given as a target
- $< represents a single file given as a target
- $@ represents the target of the rule

### Creating Libraries

A library is a piece of code that is portable as either a static object or a shared object.

#### Static Object

A static object, static library, or .a file is a set of intermediate ELF files that are compiled into your program at compile time. 
The benefit of a static object is that the code is transported inside your final product, you don't need to ship additional libraries. 
The downside is that your binary will increase in size as code is not shared amongst other processes.

#### Compiling

```bash
# compiling the library
gcc -o libteehee.o -c ./lib.c
ar rcs libteehee.a libteehee.o

# adding the library to a project
gcc -o main main.c libteehee.a
```

#### Shared Object

A shared object, shared library, or .so file is a compiled ELF file that is linked into your program at run time. 
The benefit of a shared object is that the code is transported outside your final product meaning your ELF will be smaller 
without the duplicated code inside. However, this means that for your code to run, you will need to install the library in 
the system root library directory, such as /lib or /usr/lib.

```bash
# compiling the library
gcc -o -shared libteehee.so ./lib.c

# adding the library to a project
gcc -o main main.c -lteehee -L$(pwd)/lib
```

### Creating a Unix Command Line Utility with Getopt

Using the getopt library in the following loop, we can parse the command line arguments passed to our program to capture the intent from the user.

```c
#include <getopt.h>

int main(int argc, char *argv[]) {
	int c = 0;

	while ((c = getopt(argc, argv, "nf:") != -1) {
		switch(c) {
			case 'f':
				...
				break;
			case 'n':
				...
				break;
		}
	}
}
```

Based on this, no we can use flags like -n or -f in our program and execute logic based on those flags!

## Cheatsheet

  ![C Cheatsheet](./images/C_cheatsheet_tetsuo.jpeg)

## Data Types

- char c = 'A'; // single character | 1 byte (-128 to 127) | unsigned char (0 to 255)
- char str[] = "Hello"; // array of characters | string

- float f = 3.14; // 4 bytes (32 bits of precision) | 6-7 decimal digits | loses precision after 7 digits
- double d = 3.14; // 8 bytes (64 bits of precision) | 15-16 decimal digits | loses precision after 16 digits

- bool b = true; // 1 byte (0 or 1) | true or false | need to include stdbool.h

- short int si; // 2 bytes (-32,768 to 32,767)
- unsigned short int usi; // 2 bytes (0 to 65,535)

- int i; // 4 bytes (-2,147,483,648 to 2,147,483,647)
- unsigned int ui; // 4 bytes (0 to 4,294,967,295)

- long long int lli; // 8 bytes (-9,223,372,036,854,775,808 to 9,223,372,036,854,775,807)
- unsigned long long int ulli; // 8 bytes (0 to 18,446,744,073,709,551,615)

- defining a constant is done by using the const keyword

- Math library (#include <math.h>, compile with -lm) prefers double.

    const float PI = 3.14;

| Name      | Size    | Description                                           |
|-----------|---------|-------------------------------------------------------|
| char      | 1 byte  | an ASCII value: e.g. 'a' (see: man ascii)             |
| int/long  | 4 bytes | a signed integer: e.g. 97 or hex 0x61, oct 0141       |
| long long | 8 bytes | a longer multi-byte signed integer                    |
| float     | 4 bytes | a floating-point (possibly fractional) value          |
| double    | 8 bytes | a double length float                                 |

## Casting

Preceding a primitive expression with an alternate parenthesized type converts or "casts" value to a new value equivalent in new type:

```c
int a = (int) 3.131;   //assigns a=3 without complaint
```

Preceding any other expression with a cast forces new type for unchanged value.

```c
double b = 3.131;
int a = *(int*)&b; //interprets the double b as an integer (not necessarily 3)
```

## STRUCTS and ARRAYS and POINTERS and ADDRESS COMPUTATION:

### Structs

Collect several fields into a single logical type:

```c
struct { int n; double root;} s; //s has two fields, n and root
s.root = sqrt((s.n=7)); //ref fields (N.B. double parens=>assign OK!)
```

Basic struct definition and initialization:

```c
struct Point {
    int x;
    int y;
};

struct Point p1 = {10, 20}; // Initialize at declaration
struct Point p2;
p2.x = 30;
p2.y = 40;

printf("p1: (%d, %d)\n", p1.x, p1.y);
printf("p2: (%d, %d)\n", p2.x, p2.y);
```

Nested structs:

```c
struct Rectangle {
    struct Point top_left;
    struct Point bottom_right;
};

struct Rectangle rect = {{0, 0}, {100, 100}};
printf("Rectangle: (%d,%d) to (%d,%d)\n", 
       rect.top_left.x, rect.top_left.y, 
       rect.bottom_right.x, rect.bottom_right.y);
```

Array of structs:

```c
struct Student {
    char name[50];
    int age;
    float gpa;
};

struct Student class[3] = {
    {"Alice", 20, 3.8},
    {"Bob", 22, 3.5},
    {"Charlie", 21, 3.9}
};

for (int i = 0; i < 3; i++) {
    printf("Student %d: %s, Age: %d, GPA: %.1f\n", 
           i+1, class[i].name, class[i].age, class[i].gpa);
}
```

===============================================

LEAVES - TOOLS AND EXTRAS FOR C PROGRAMMING

===============================================

## .PHONY

`.PHONY` in a Makefile declares targets that don't produce output files, ensuring their commands always execute 
regardless of file timestamps or existence. This prevents conflicts when a real file shares the target's name and 
optimizes build performance.

### Core Purpose

- Overrides file-based behavior: By default, make checks if a target's output file exists and whether its 
dependencies are newer. .PHONY bypasses this, forcing execution of the target's commands
- Avoids naming conflicts: If a file named clean exists, make clean would do nothing without .PHONY
- Improves performance: make skips implicit rule checks for phony targets (e.g., timestamp comparisons)

### Common Use Cases

- **Standard targets**

```makefile
.PHONY: all clean install
```

- `all`: Builds all targets
- `clean`: Removes build artifacts
- `install`: Copies binaries to system directories

- **Composite Workflows**

```makefile
.PHONY: rebuild
rebuild: clean all
```

Runs clean followed by all.

## Memory Allocators 101 - Write a simple memory allocator

### Pages and virtual memory

In most modern operating systems, memory is divided into ```pages```. A page is simply a 
fixed-size block of memory managed by the OS's virtual memory subsystem, and a common page size
on many systems is 4 KB (4096 bytes).

To align a page boundary, means ensuring that the starting address of the allocated
memory is an exact multiple of the system's page size. 

Why page alignment matters?

- **Performance/Hardware reasons**
- **System calls/OS interactions**: When you request memory directly from the OS 
(e.g., via mmap or a similar low-level call), you typically have to request multiples of the page size.
- **Special use cases**: Sometimes you may need memory that’s aligned for DMA (Direct Memory Access) 
or other low-level hardware operations.

### Virtual memory

Most modern operating systems (Windows, Linux, macOS, etc.) use a “virtual memory” mechanism. 
This means that every process gets its own (seemingly large, contiguous) address space, independent 
of other processes. Internally, the OS works behind the scenes with the CPU to map virtual addresses 
to actual physical addresses.

### Page Tables and the OS

Inside your CPU, there’s a hardware component called the Memory Management Unit (MMU). 
It uses page tables that the operating system sets up. These tables describe how virtual pages 
map to physical pages of RAM. For example:

- Virtual page #5 in a process → Physical page #200.
- Virtual page #6 in a process → Physical page #89.

The OS is in control of these mappings. It can even mark some pages as being located on disk (in swap space) 
or shared between processes, etc.

- The OS configures page tables in the CPU (via the Memory Management Unit, MMU).
- When your code accesses a virtual address, the MMU consults the page tables to figure out 
the corresponding physical address.
- The CPU then places that physical address on the address bus, and the memory controller 
talks to RAM at that location.

Imagine your physical memory as a huge set of boxes, each box can hold 4 KB of data. Your OS is the manager 
who decides how these boxes get handed out. In your process’s “virtual world,” you see a nice row of addresses 
from, say, 0x00000000 to 0xFFFFFFFF (in a 32-bit system), but really each “page” of that virtual world is 
mapped to one of the OS-managed physical boxes.

If you draw a line in that big row of addresses every 4096 bytes, each line you draw corresponds to a “page boundary.” 
Aligning on a page boundary means you’re starting your data exactly at one of those lines.

### Electrical Signals on the RAM Modules

Here’s a simplified view of what happens electrically in, say, a modern DRAM module (e.g., DDR4, DDR5):

- **Address bus**: A set of lines (wires) that carry bits of the physical address.
- **Control signals**: Signals like Row Address Strobe (RAS), Column Address Strobe (CAS), 
Chip Select (CS), Write Enable (WE), etc.
- **Data bus**: The lines used to send or receive the actual data bits.

### How is the OS Involved Here?

- The OS doesn’t typically instruct the memory chip line-by-line. That job is done automatically 
by the CPU’s memory controller.
- The OS’s job is to set up the page tables that tell the MMU how to convert virtual addresses 
to physical addresses.
- Once that’s done, normal load/store instructions in user code automatically end up driving the 
physical signals in the memory hardware.

### Down to the Impulses (Conceptually)

If you want to visualize the “electrical impulses,” think of it like this:

1. Your code runs an instruction like mov eax, [0x7ffdf3c0] (simplifying the syntax):
    - That’s a request to read from the virtual address 0x7ffdf3c0.

2. The CPU’s MMU looks up the page table entry for the page containing 0x7ffdf3c0:
    - Maybe it sees that this virtual page translates to physical page frame #1234, 
    offset 0x3C0 within that page.
    - If page frame #1234 corresponds to a physical address range starting at, say, 0x9ABCD000, 
    then the actual physical address is 0x9ABCD000 + 0x3C0.

3. The CPU then electrically places 0x9ABCD3C0 on the address bus (in binary form across the address lines), 
and sets the read control signals.

4. The memory controller decodes that address. It determines which DRAM module/bank/row/column must be accessed.

5. The memory controller asserts signals on the RAM pins:
    - The row bits (part of 0x9ABCD3C0) go out on the address lines, RAS is pulsed.
    - The column bits go out, CAS is pulsed.
    - WE is not asserted (because it’s a read).

6. The RAM module, seeing these signals, locates the correct row and column in its internal array of cells, 
and places the requested data on the data lines.

7. The CPU reads the data lines, stores the result in eax (completing the mov instruction).

All of those signals on the address bus, data bus, and control lines are literally changes in voltage—electrical impulses. 
That’s as low-level as it gets: voltages and timing signals that DRAM responds to, reading or writing charge 
in tiny capacitors.

### The tutorial

A process runs within its own virtual address space that's distinct from the virtual address spaces of other
processes. This virtual address space typically comprises of 5 sections:

- **Text section**: The part that contains the binary instructions to be executed by the processor.
- **Data section**: Contains non-zero initialized static data.
- **BSS (Block Started by Symbol)**: Contains zero-initialized static data. Static data uninitialized in 
program is initialized 0 and goes here.
- **Heap**: Contains the dynamically allocated data.
- **Stack**: Contains your automatic variables, function arguments, copy of base pointer etc.

```text
+------------------+            +----------------------+
|  Process Space   |            |  Other Process Space |
| (Virtual Memory) |            | (Virtual Memory)     |
+-----+-----+------+            +------+-----+---------+
      |     |                          |
      |     +------------------------------------+
      |                                       (In reality, each process has its
      |                                       own virtual space. This arrow
      |                                       just shows that we could have many.)
      v
   +----------------------------+
   |   Page Tables (per CPU)   |
   +----------------------------+
      |
      | (The OS populates these
      |  tables to map virtual
      |  addresses to physical
      |  addresses.)
      v
   +-----------------------------------------------------+
   |                 Memory Management Unit (MMU)        |
   |  (Inside CPU, uses Page Tables to translate Virtual |
   |   addresses → Physical addresses on each memory op) |
   +-----------------------------------------------------+
      |
      | (Once the MMU knows the physical address,
      |  it sends that address to the memory controller.)
      v
   +-------------------------------------+
   |  CPU / Memory Controller (Northbridge or integrated)  |
   +-------------------------------------+
      |
      | (Physical Address, e.g. 0x9ABCD000 + offset)
      v
   +-------------------------------------------------------+
   |          System Bus / Memory Bus (Electrical)         |
   |   (Address lines, Data lines, Control signals like     |
   |    RAS, CAS, WE, etc.)                                 |
   +-------------------------------------------------------+
      |
      v
   +-----------------+
   |  DRAM Modules   |
   | (Physical RAM)  |
   +-----------------+
      | (Internally: row/column decoding,
      |  storage cells, charge in capacitors.)
      v
  (Electrical impulses flip bits in RAM cells)
```

![memlayout](./images/memlayout.jpg)

- As you can see in the image, the stack and the heap grow in the opposite directions.
- Sometimes the data, bss and heap sections are collectively referred to as the “data segment”,
the end of which is demarcated by a pointer named program break or brk.
- That is, brk points to the end of the heap.
- Now if we want to allocate more memory in the heap, we need to request the system to increment brk. 
Similarly, to release memory we need to request the system to decrement brk.
- Assuming we run Linux (or a Unix-like system), we can make use of sbrk() system call that lets us 
manipulate the program break.
- Calling ```sbrk(0)``` gives the current address of program break.
- Calling ```sbrk(x)``` with a positive value increments brk by x bytes, as a result allocating memory.
- Calling ```sbrk(-x)``` with a negative value decrements brk by x bytes, as a result releasing memory.
- On failure, sbrk() returns ```(void*) -1```.
- sbrk() is not our best buddy in 2015. There are better alternatives like mmap() available today.
- Moreover, we need to understand that the heap memory the operating system has provided is contiguous. 
So we can only release memory which is at the end of the heap. We can’t release a block of memory in the middle to the OS. 
Imagine your heap to be something like a long loaf of bread that you can stretch and shrink at one end, but you have to 
keep it in one piece.
- To address this issue of not being able to release memory that’s not at the end of the heap, we will make a distinction 
between freeing memory and releasing memory.
- From now on, freeing a block of memory does not necessarily mean we release memory back to OS. It just means that we keep 
the block marked as free. This block marked as free may be reused on a later malloc() call. Since memory not at the end 
of the heap can’t be released, this is the only way ahead for us.
- So now, we have two things to store for every block of allocated memory (we store in struct header_t):
    1. size
    2. Whether a block is free or not-free?
- To store this information, we will add a header to every newly allocated memory block.
- The idea is simple. When a program requests for size bytes of memory, we calculate total_size = header_size + size, 
and call sbrk(total_size). We use this memory space returned by sbrk() to fit in both the header and the actual memory block. 
The header is internally managed, and is kept completely hidden from the calling program.
- Now, each one of our memory blocks will look like:

![memblock](./images/node.png)

- We can’t be completely sure the blocks of memory allocated by our malloc is contiguous. Imagine the calling program 
has a foreign sbrk(), or there’s a section of memory mmap()ed in between our memory blocks. 
We also need a way to traverse through our blocks for memory (why traverse? we will get to know when we look at the 
implementation of free()). So to keep track of the memory allocated by our malloc, we will put them in a linked list. 
Our linked list of memory blocks like this:

![nodelist](./images/nodes.png)

- Let’s wrap the entire header struct in a union along with a stub variable of size 16 bytes. 
This makes the header end up on a memory address aligned to 16 bytes. Recall that the size of a union is the 
larger size of its members. So the union guarantees that the end of the header is memory aligned. The end of the header 
is where the actual memory block begins and therefore the memory provided to the caller by the allocator will be aligned 
to 16 bytes.

- To prevent two or more threads from concurrently accessing memory, we will put a basic locking mechanism in place.
- We’ll have a global lock, and before every action on memory you have to acquire the lock, and once you are done you 
have to release the lock.

### Malloc code explanation

We check if the requested size is zero. If it is, then we return NULL.
For a valid size, we first acquire the lock. The we call get_free_block() - 
it traverses the linked list and see if there already exist a block of memory 
that is marked as free and can accomodate the given size. Here, we take a first-fit 
approach in searching the linked list.

If a sufficiently large free block is found, we will simply mark that block as not-free, 
release the global lock, and then return a pointer to that block. In such a case, the 
header pointer will refer to the header part of the block of memory we just found by 
traversing the list. Remember, we have to hide the very existence of the header to an 
outside party. When we do (header + 1), it points to the byte right after the end of the 
header. This is incidentally also the first byte of the actual memory block, the one the 
caller is interested in. This is cast to (void*) and returned.

If we have not found a sufficiently large free block, then we have to extend the heap by 
calling sbrk(). The heap has to be extended by a size that fits the requested size as 
well a header. For that, we first compute the total size: 
total_size = sizeof(header_t) + size;. 
Now, we request the OS to increment the program break: sbrk(total_size).

In the memory thus obtained from the OS, we first make space for the header. 
In C, there is no need to cast a void* to any other pointer type, it is always safely 
promoted. That’s why we don’t explicitly do: ```header = (header_t *)block;
We fill this header with the requested size (not the total size) and mark it as not-free.```
We update the next pointer, head and tail so to reflect the new state of the linked list.
As explained earlier, we hide the header from the caller and hence return 
(void*)(header + 1). We make sure we release the global lock as well.

### free()

Now, we will look at what free() should do. free() has to first deterimine if the 
block-to-be-freed is at the end of the heap. If it is, we can release it to the OS. 
Otherwise, all we do is mark it ‘free’, hoping to reuse it later.

#### Code explanation

Here, first we get the header of the block we want to free. All we need to do is get a 
pointer that is behind the block by a distance equalling the size of the header. So, 
we cast block to a header pointer type and move it behind by 1 unit.
header = (header_t*)block - 1;

sbrk(0) gives the current value of program break. To check if the block to be freed is 
at the end of the heap, we first find the end of the current block. The end can be 
computed as (char*)block + header->s.size. This is then compared with the program break.

If it is in fact at the end, then we could shrink the size of the heap and release 
memory to OS. We first reset our head and tail pointers to reflect the loss of the 
last block. Then the amount of memory to be released is calculated. This the sum of 
sizes of the header and the acutal block: sizeof(header_t) + header->s.size. 
To release this much amount of memory, we call sbrk() with the negative of this value.

In the case the block is not the last one in the linked list, we simply set the is_free 
field of its header. This is the field checked by get_free_block() before actually calling 
sbrk() on a malloc().

### calloc()

The calloc(num, nsize) function allocates memory for an array of num elements of nsize 
bytes each and returns a pointer to the allocated memory. Additionally, the memory is 
all set to zeroes.

### realloc()

realloc() changes the size of the given memory block to the size given.

We first get the block’s header and see if the block already has the size to accomodate 
the requested size. If it does, there’s nothing to be done.

If the current block does not have the requested size, then we call malloc() to get a i
block of the request size, and relocate contents to the new bigger block using memcpy(). 
The old memory block is then freed.

### Compiling and using our memory allocator

```bash
gcc -o memalloc.so -fPIC -shared memalloc.c
```

### Other memory allocators

[liballoc](https://github.com/blanham/liballoc/)
[Doug Lea’s Memory Allocator](http://oswego.edu/dl/html/malloc.html%20dlmalloc)
[TCMalloc](http://goog-perftools.sourceforge.net/doc/tcmalloc.html)
[ptmalloc](http://www.malloc.de/en/)

[The GNU C Library: Malloc Tunable Parameters](https://www.gnu.org/software/libc/manual/html_node/Malloc-Tunable-Parameters.html)
[OSDev - Memory allocation](http://wiki.osdev.org/Memory_Allocation)
[Memory Allocators 101 - James Golick](http://jamesgolick.com/2013/5/15/memory-allocators-101.html)
