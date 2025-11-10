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
