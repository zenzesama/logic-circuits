# nand2cpu

A software simulation of a complete 8-bit CPU, built from the ground up in C. Starting from individual logic gates and flip-flops, all the way up to a functioning processor with a 64KB address space.

Each layer of abstraction is implemented faithfully: gates are truth tables, adders are built from gates, latches from adders, RAM from latches, and the CPU from everything else.

---

## Architecture

```
                        +---------------------------+
                        |           CPU             |
                        |                           |
          +-------+     |  +----+      +----+       |
          |  RAM  | <---+->| CU | <--> | IR |       |
          | (64KB)|     |  +----+      +----+       |
          +-------+     |    |    \                 |
                        |  +----+  +----+           |
                        |  | ALU|->| PC |           |
                        |  +----+  +----+           |
                        +---------------------------+
```

The simulator is organized into layers, each built on top of the previous:

**Gates** (`gates.c`) - AND, OR, NOT, NAND, NOR, XOR implemented as truth table lookups.

**Arithmetic** (`logic.c`) - Half adder and full adder built from gates. 8-bit adder and subtractor (two's complement) built from full adders. It also has 8-bit latch, 2-to-1 multiplexer, adding machine, and both the 8-bit and 16-bit ripple counters.

**Flip-flops** (`flipFlops.c`) - RS flip-flop built from NOR gates. D flip-flop, edge-triggered D flip-flop (master-slave), and an edge-triggered D flip-flop with preset/clear (used by the program counter for the Jump instruction).

**Utilities** (`hacks.c`, `print.c`) - Hex string <-> byte conversions (`hexToByte`, `byteToDecimal`, `byteToHex`), and terminal printing for bits and bytes.

**RAM** (`ram.c`) - 8×1 RAM built from a 3-to-8 decoder, 8-to-1 selector, and D-latches. Extended to 8×8. Then a 64K×8 (64KB) RAM using genuine flip-flop storage per cell, addressed by two hex byte strings (hi + lo).

**ALU** (`alu.c`) - 8-bit accumulator with a carry latch and zero-flag latch. Supports ADD, SUB, ADC (add with carry), and SBB (subtract with borrow). The zero detector uses a chained NOR tree.

**Instruction Register** (`ir.c`) - Three 8-bit latches holding the current instruction: opcode, addr\_hi, addr\_lo. Each instruction is exactly 3 bytes.

**Control Unit** (`cu.c`) - Sequences the fetch-execute cycle. Hardwired (no microcode) — each opcode maps directly to a fixed sequence of operations driven by a switch statement. No explicit clock model; each call to `stepCU()` represents one complete instruction cycle.

**CPU** (`cpu.c`) - Public API wrapping the control unit. Adds a cycle counter and a debug mode that prints state after every instruction.

---

## How the CPU Executes an Instruction

Every call to `stepCU()` performs one complete fetch-execute cycle, broken into four sub-steps:

**1. Fetch — opcode** `PC → IR.opcode`, then `PC++`

**2. Fetch — address** `PC → IR.addr_hi`, `PC++`, then `PC → IR.addr_lo`, `PC++`

**3. Decode** — the CU reads the opcode from the IR and selects the appropriate operation.

**4. Execute** — depending on the opcode:
- `LOD` / `STO`: the CU reads or writes RAM directly, bypassing the ALU
- `ADD` / `SUB` / `ADC` / `SBB`: the CU reads the operand from RAM and passes it to the ALU along with the opcode; the ALU updates the accumulator, carry latch, and zero latch
- `JMP` / `JZ` / `JC` / `JNZ` / `JNC`: the CU checks the ALU flags and loads the jump address into the PC via preset/clear on its flip-flops if the condition is met
- `HLT`: sets the halted flag and returns `CU_HALTED`

If a jump is not taken, the PC has already advanced past the instruction and execution continues normally on the next call.

---

## Instruction Set

Every instruction is 3 bytes: `[opcode] [addr_hi] [addr_lo]`.

| Opcode | Mnemonic | Operation |
|--------|----------|-----------|
| `0x10` | `LOD`    | Load `RAM[addr]` into accumulator |
| `0x11` | `STO`    | Store accumulator into `RAM[addr]` |
| `0x20` | `ADD`    | `ACC = ACC + RAM[addr]` |
| `0x21` | `SUB`    | `ACC = ACC - RAM[addr]` |
| `0x22` | `ADC`    | `ACC = ACC + RAM[addr] + carry` |
| `0x23` | `SBB`    | `ACC = ACC - RAM[addr] - carry` |
| `0x30` | `JMP`    | Unconditional jump to addr |
| `0x31` | `JZ`     | Jump if zero flag is set |
| `0x32` | `JC`     | Jump if carry flag is set |
| `0x33` | `JNZ`    | Jump if zero flag is clear |
| `0x34` | `JNC`    | Jump if carry flag is clear |
| `0xFF` | `HLT`    | Halt execution |

ADC and SBB use the carry latch from the previous instruction, enabling 16-bit arithmetic across two 8-bit operations.

**Design tradeoffs:** the fixed 3-byte format keeps the fetch cycle uniform every instruction takes the same number of sub-steps regardless of whether it uses its address bytes (HLT ignores them). The tradeoff is some wasted space and no support for immediate values or registers beyond the accumulator. These are intentional simplifications, not oversights.

---

## Building and Running

### Tests

```bash
cd tests
make        # build all test binaries
make run    # build and run all suites
make clean  # remove binaries
```

Each test suite exits with a non-zero code if any assertion fails. `make run` reports how many suites had failures.

### Demos

```bash
cd demo
make all    # build all demos
./cpu
./cu
./alu
./counter8
./counter16
./ram8x8
./ram64k
./ir
make clean  # remove binaries
```

---

## Example: Running a Program

Programs are loaded as a flat array of byte values and written into RAM starting at `0x0000`. Here is a minimal example that computes `0x56 + 0x2A - 0x38` and stores the result:

```c
cpu* c = newCPU();
cpuSetDebug(c, 1);  // print state after each instruction

int program[] = {
    0x10, 0x00, 0x10,   // LOD [0x0010]
    0x20, 0x00, 0x11,   // ADD [0x0011]
    0x21, 0x00, 0x12,   // SUB [0x0012]
    0x11, 0x00, 0x13,   // STO [0x0013]
    0xFF, 0x00, 0x00,   // HLT
    0x00,               // padding
    0x56, 0x2A, 0x38    // data
};

cpuLoad(c, program, sizeof(program) / sizeof(program[0]));
runCPU(c);

byte result = cpuReadRAM(c, 0x0013);  // 0x48
freeCPU(c);
```

With debug on, each instruction prints its opcode, program counter, accumulator value, and flags:

```
[cycle    1] opcode = 0x10  PC = 0x0003  ACC = 0x56  CARRY = 0 ZERO = 0
[cycle    2] opcode = 0x20  PC = 0x0006  ACC = 0x80  CARRY = 0 ZERO = 0
[cycle    3] opcode = 0x21  PC = 0x0009  ACC = 0x48  CARRY = 0 ZERO = 0
[cycle    4] opcode = 0x11  PC = 0x000C  ACC = 0x48  CARRY = 0 ZERO = 0
[cycle    5] opcode = 0xFF  PC = 0x000F  ACC = 0x48  CARRY = 0 ZERO = 0
```

---

## Requirements

- C99 compiler (tested with `gcc`)
- Standard C library

The 64KB RAM allocates ~1MB on the heap at startup (65,536 cells × 8 flip-flops each). This is architecturally faithful to how real RAM is built from storage elements, but it is not optimized for simulation speed. It's fast enough for the included demos and tests, but don't expect to simulate millions of cycles in a hurry.

---

## Development Notes

Much of this code was written with AI assistance (Claude, free tier). That doesn't mean copy-paste. I read everything, made changes in a lot of places, and the core logic and architecture decisions are mine. Tests and demos leaned on AI, still with supervision. The full making is documented in `devlog.md`.

If I had to put a number on it: ~60% AI. I too am an AI slop hater.