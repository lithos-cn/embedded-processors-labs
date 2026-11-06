@ Multiplication program: r0 = r1 * r2
MOV r1, #5      @ Load value 5 into register r1
MOV r2, #4      @ Load value 4 into register r2
MUL r0, r1, r2  @ Multiply r1 by r2, store result in r0
MOV r0, #1      @ Load 1 into register r0 (stdout handle)
SWI 0x6b        @ Print integer in register r0 to stdout
SWI 0x11        @ Stop program execution