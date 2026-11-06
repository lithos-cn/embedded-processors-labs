@ Subtraction program: r0 = r2 - r1
MOV r1, #15     @ Load value 15 into register r1
MOV r2, #25     @ Load value 25 into register r2
SUB r0, r2, r1  @ Subtract r1 from r2, store result in r0
MOV r0, #1      @ Load 1 into register r0 (stdout handle)
SWI 0x6b        @ Print integer in register r0 to stdout
SWI 0x11        @ Stop program execution