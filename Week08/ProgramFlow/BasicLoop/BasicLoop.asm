// File: BasicLoop.vm

// push constant 0
@0
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop local 0
@0
D=A
@LCL
D=D+M // D = LCL + index
@R13
M=D // R13 = D (SP)
// D = RAM[SP--]
@SP
AM=M-1
D=M
@R13
A=M
M=D // RAM[LCL + index] = *SP


// label BasicLoop$LOOP_START
(BasicLoop$LOOP_START)

// push argument 0
@0
D=A
@ARG
A=D+M // A = ARG + 0
D=M // D = RAM[ARG + 0]
@SP
A=M
M=D
@SP
M=M+1 // SP++


// push local 0
@0
D=A
@LCL
A=D+M // A = LCL + 0
D=M // D = RAM[LCL + 0]
@SP
A=M
M=D
@SP
M=M+1 // SP++


// add
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=D+M


// pop local 0
@0
D=A
@LCL
D=D+M // D = LCL + index
@R13
M=D // R13 = D (SP)
// D = RAM[SP--]
@SP
AM=M-1
D=M
@R13
A=M
M=D // RAM[LCL + index] = *SP


// push argument 0
@0
D=A
@ARG
A=D+M // A = ARG + 0
D=M // D = RAM[ARG + 0]
@SP
A=M
M=D
@SP
M=M+1 // SP++


// push constant 1
@1
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// sub
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=M-D


// pop argument 0
@0
D=A
@ARG
D=D+M // D = ARG + index
@R13
M=D // R13 = D (SP)
// D = RAM[SP--]
@SP
AM=M-1
D=M
@R13
A=M
M=D // RAM[LCL + index] = *SP


// push argument 0
@0
D=A
@ARG
A=D+M // A = ARG + 0
D=M // D = RAM[ARG + 0]
@SP
A=M
M=D
@SP
M=M+1 // SP++


// if-goto BasicLoop$LOOP_START
@SP
AM=M-1
D=M
@BasicLoop$LOOP_START
D;JNE


// push local 0
@0
D=A
@LCL
A=D+M // A = LCL + 0
D=M // D = RAM[LCL + 0]
@SP
A=M
M=D
@SP
M=M+1 // SP++


