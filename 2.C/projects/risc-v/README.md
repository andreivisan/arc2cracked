# RISC-V

## 1. Digital Logic and CPU Building Blocks

### 1.1 Combinational vs. Sequential Logic
    - Combinational logic produces outputs purely based on current inputs (e.g., adders, multiplexers, encoders). Think of it like a simple function f(x) = y; there’s no “state” or memory, just a transformation from inputs to outputs.
    - Sequential logic incorporates memory elements (e.g., flip-flops, registers). Output depends on both current inputs and previous states. This is analogous to an object in software that remembers some internal state and updates it each time a method is called.

### 1.2 The CPU “Recipe”

A CPU, at its core, is made of:

1. **Arithmetic-Logic Unit (ALU)**: Performs operations like addition, subtraction, bitwise AND/OR, etc.
2. **Registers**: Fast storage (flip-flops) that hold data and addresses for immediate use.
3. **Control Unit**: Decodes instructions and controls the flow of data between the ALU, registers, and memory.

These three components mirror big software systems with different modules:

- The ALU is like a “utility library” that does computations.
- The registers are local variables (or caches) needed for performance.
- The control unit is like a dispatch or routing mechanism that decides what the system does next.

## 2. RISC-V Overview

### 2.1 RISC vs. CISC

- RISC (Reduced Instruction Set Computing): Emphasizes simpler, more uniform instructions, each taking roughly one clock cycle (in a simplified single-cycle model). This is akin to having many small microservices each doing a single focused task quickly.
- CISC (Complex Instruction Set Computing): Has many complex instructions, some of which might take multiple cycles to complete.

RISC-V is a clean-slate RISC architecture developed with simplicity and modularity in mind. For example, RISC-V has separate instruction groups (base integer instructions, floating-point extensions, atomic extensions, etc.) that you can pick and choose from.

### 2.2 Key Components of RISC-V

1. **Registers**: A typical RISC-V CPU has 32 integer registers, each 32 bits (in RV32) or 64 bits (in RV64). The first register (x0) is always zero.
2. **Load/Store Architecture**: Memory is accessed only via load/store instructions. Other operations happen in registers.
3. **Fixed-Length Instructions**: Basic RISC-V instructions are 32 bits wide (there are also compressed 16-bit instructions in some variants).

We’ll aim to implement a minimal RV32I (the base integer instruction set, 32-bit) processor in C by the end of our lessons. This subset includes arithmetic, logical, comparison, and load/store instructions.

## 3. Mapping Hardware Concepts into C

### 3.1 Data Representation in C

- Registers: Typically modeled as an array or struct of 32-bit integers (e.g., uint32_t). We can index them from 0 to 31.
- Memory: Often represented as a byte array. You can think of it as uint8_t memory[MEM_SIZE].
- Bitwise Operations: In hardware, everything is bits. In C, you’ll use bitwise operators (&, |, ^, <<, >>) to simulate the ALU, setting flags or extracting specific bits for opcode decoding.

### 3.2 Advanced C Considerations

- Unions and Bit Fields: If you want to break down a 32-bit instruction into fields (opcode, funct3, funct7, etc.), you might use a union or a struct with bit fields. But beware of compiler-specific packing or endianness issues.
- Pointers and Pointer Arithmetic: When simulating memory, you might store instructions in an array and use pointer arithmetic to fetch them. You need to be careful with alignment if you simulate instructions as 32-bit chunks rather than bytes.
- Function Pointers: Sometimes you can store references to functions (like an ALU operation) in a table. This approach is reminiscent of a jump table in hardware microcode or a function dispatch in a dynamic environment.

### 3.3 The “Hardware” Mindset in Software

You have deep experience in enterprise software—distributed systems, microservices, large-scale data. Building a CPU in C is similar in that you break a large, complex system into well-defined modules with limited responsibilities. The difference is:
- Instead of a network protocol for communication, you have “wires” or function parameters that move data around.
- Instead of a high-level concurrency or scaling pattern, you have a cycle-by-cycle flow of data through registers, memory, and the ALU.

The same design discipline applies: keep modules (like the ALU, register file, instruction decoder) cohesive and well-defined.