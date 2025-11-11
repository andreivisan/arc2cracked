# Build RISC-V

## Logic gates

- In digital circuits wires stabilise to one of the two voltages: a high voltage (VDD)
or a low voltage (VSS or ground). So, a wire(bit) carries a boolean value, where high and low
voltages can be viewed as 1/0, true/false, asserted/deasserted, on/off, etc.

> Note:
> - The small circle on the output of a gate indicates an inverted output.
> - XOR and XNOR are "exclusive" OR and NOR, where "exclusive" means "but not both".

## Aritmetic Logic

- While individual wires (or "bits") hold one of two values in a digital circuit, we can
have a collection of N wires (called a "vector") that represent up to 2^N possible values.

## Clock and Flip-Flops

Sequential logic introduces a clock signal.

![clock](./images/clock_flip_flops.png)

- The clock is driven through the circuit to flip-flops which sequence the logic.
- Flip-flops come in various flavors, but the simplest and most common is "positive-edge-triggered
D-type flip-flop".
- These drive the value at their input to their output, but only when the clock rises.
- They hold their output value until the next rising edge of their clock input.

### Sequencial circuit

- Unlike a combinational circuit, where output values are purely a function of the input values,
sequential circuits have internal state.
- Every sequential circuit needs the ability to get to a known "reset" state.
- Therefore, every sequential has a reset signal.

![fibonacci circuit](./images/fibonacci_circuit.png)

![fibonacci circuit waveforms](./images/fibonacci_circuit_waveforms.png)

- While $reset is asserted, a 1 value is driven into $num.

## Software, Compilers and CPUs

- **Instruction Set Architecture (ISA)** - The ISA defines the programmer-visible behavior of the 
machine—its instructions and their encodings, registers, and rules of execution (plus memory/privilege
model) — independent of implementation.
- The ISA deines a human-readable form of every instruction, as well as the mapping of those *assembly*
instructions into bits.
- A **compiler** does the job of translating a program's source code into a binary file or executable
containing machine instructions for a particular ISA.
- An **Operating System** does the job of loading the binary file into the memory for execution by the
CPU hardware that understands the ISA.

## RISC-V

> # ISA (Instruction Set Architecture) — RISC-V Cheat Sheet (RV64I focus)
>
> ## TL;DR
>
> * **Where do instructions live?** In **memory**. The CPU **fetches** them at the address in **`pc`**.
> * **Where does the ISA “live”?** In the **spec** (a document). Hardware and emulators **implement** it.
> * **What does the ISA do?** Defines the **software-visible contract**: registers, instruction encodings & semantics, traps, memory/privilege rules. It does **not** dictate microarchitecture or the OS/ABI.
>
> ## Mental model (fetch → decode → execute)
>
> 1. **Loader/OS (once):** puts your program in memory and sets initial state (`pc`, stack pointer, etc.).
> 2. **Runtime (every instruction):** **Fetch** 32 bits at `pc` → **Decode** fields (`opcode`, `rd`, `rs1`, `rs2`, `funct3`, `funct7`, immediate) → **Execute** (read regs, ALU/mem op, write result) → **Update `pc`** (sequential or branch/jump).
>
> * **Invariant:** **`x0` is always 0** (writes ignored).
>
> ## What the ISA defines
>
> * **Programmer’s model:** 32 integer regs `x0..x31` (RV64I ⇒ 64-bit), `pc`.
> * **Encodings & formats:** R/I/S/B/U/J layouts; how to rebuild **immediates** (usually sign-extended).
> * **Semantics:** exact effects (e.g., **`JALR` clears bit 0** of the target).
> * **Exceptions/traps:** illegal instruction, misaligned access, etc.
> * **Memory rules:** little-endian in practice; memory ordering model.
> * **Privilege model (privileged spec):** M/S/U modes, CSRs, interrupts, paging.
>
> ## What the ISA doesn’t define
>
> * **Microarchitecture:** pipelines, caches, branch predictors, OOO, etc.
> * **ABI/OS:** calling conventions, ELF, syscalls (that’s ABI/OS territory).
>
> ## RISC-V specifics you’ll use immediately (RV64I)
>
> * **Register width:** 64-bit (`XLEN=64`). **Base instruction width:** 32-bit.
> * **Opcode vs funct fields:** `opcode` = class; `funct3`/`funct7` refine the op (e.g., `ADD` vs `SUB`).
> * **Immediates:** sign-extended; B/J are PC-relative with split fields (effectively **<<1**).
> * **`JALR` gotcha:** target address **bit 0 forced to 0**.
> * **Loads/stores:** addr = `rs1 + imm`. `LD/SD` are 64-bit; `LWU` zero-extends to 64; `LW` sign-extends to 64.
> * **`x0`:** hard-wired zero (great for moves/clears/comparisons).
>
> ## ISA vs Microarchitecture vs ABI vs OS (one-liners)
>
> * **ISA:** what the machine **means** to software (contract).
> * **Microarchitecture:** how the CPU **implements** that contract.
> * **ABI:** how programs **call** each other/OS (calling conv., object format).
> * **OS:** loads programs, manages resources, handles syscalls/interrupts.
>
> ---
>
> # How a CPU “knows” the ISA (RISC-V)
>
> **One instruction, concretely**
>
> 1. **Fetch:** read 32-bit instruction at `pc`.
> 2. **Field split:** wire slices → R-type layout: `[31:25 funct7][24:20 rs2][19:15 rs1][14:12 funct3][11:7 rd][6:0 opcode]`.
> 3. **Decode (pattern match):** hardwired logic (or microcode) checks fields, e.g.: `opcode=0110011`, `funct3=000`, `funct7=0000000` → **ADD**; same but `funct7=0100000` → **SUB**.
> 4. **Immediate gen:** rebuild & **sign-extend** immediates (I/S/B/U/J); B/J are PC-relative (<<1). **`JALR` clears bit 0.**
> 5. **Datapath:** regfile reads `rs1`/`rs2` → ALU/memory does the op → writeback to `rd` (except `x0`).
> 6. **PC update:** `pc+=4`, or branch/jump target as defined.
> 7. **Trap if required:** illegal opcode, misaligned access, etc.
>
> **Two common decoder styles**
>
> * **Hardwired decode:** combinational logic or PLA asserts control lines from `(opcode, funct3, funct7, …)`.
> * **Microcode:** index a tiny ROM of “control words” to step the datapath (less common for simple RISC-V cores).
>
> ---
>
> # Decoder wiring vs masks & shifts (emulator vs hardware)
>
> **Big idea**
>
> * The **ISA is a spec** (contract).
> * **Hardware** embodies it with **wires & logic** that recognize bit patterns and drive the datapath.
> * An **emulator** mimics that with **bit masks, shifts, and match tables** in software.
>
> **Caveats**
>
> * The ISA isn’t “stored in wires”; the **decoder** implements it.
> * Hardware typically doesn’t “shift” to decode; it uses **fixed bit slices** (wiring).
> * Many RISC-V cores are **hardwired decoders**; microcode is another valid approach.
>
> **Rule of thumb**
>
> * **Spec:** defines patterns & meanings.
> * **Hardware:** pattern match → control signals.
> * **Emulator:** masks/shifts/tables → same outcome.
>
> **Example: distinguishing `ADD` vs `SUB` (R-type)**
>
> * Same `opcode = 0110011`, same `funct3 = 000`.
> * **`funct7`** differentiates: `funct7 = 0000000` → **ADD**; `funct7 = 0100000` → **SUB**.




