Here are my course projects of Computer Organization & Architecture.

1. RISC-V compiler design
I developed a compiler which can run RISC-V assembly.
1) asm.c: assembler for RISC-V ISA
2) riscv-small.c: a full function non-pipeline simulator
3) Makefile: a UNIX makefile that generates binary files asm (assembler) and sim (simulator),
4) simple.s: an example of assembler
5) simple.out: output of simple.s running compiler asm.c
6) sim_simple.output: output of running sim simulator simple.out

2. Cache design
I developed a cache which have the following properties:
1) 2 level, 4KB size, 2-way set associative
2) replacement strategy: LRU
3) Used a controller to switch working state.

3. 32-bit pipeline CPU design based on RISC-V 
- Completed the design via Verilog HDL, including instruction fetching module, decoding module, arithmetic logic unit ALU, hazard detection module, data forwarding module, etc. 
- Implemented a conflict resolution strategy by inserting NOP and stalling the pipeline.

If you have any question, please contact me :)