// File: SimpleFunction.vm

// function SimpleFunction.test
(SimpleFunction.test)
@SP
D=M
@LCL
M=D
D=0
// *(LCL + 0) = 0
@SP
A=M
M=D
// push local 1 = 0
A=A+1
M=D
// SP = top of the stack
A=A+1
D=A
@SP
M=D


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


// push local 1
@1
D=A
@LCL
A=D+M // A = LCL + 1
D=M // D = RAM[LCL + 1]
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


// not
@SP
A=M-1
M=!M


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


// add
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=D+M


// push argument 1
@1
D=A
@ARG
A=D+M // A = ARG + 1
D=M // D = RAM[ARG + 1]
@SP
A=M
M=D
@SP
M=M+1 // SP++


// sub
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=M-D


// return 
// store LCL in R13
@LCL
D=M
@R13
M=D
// store return address (LCL - 5) in R14
@5
A=D-A
D=M
@R14
M=D
//Reposition return value in *(ARG + 0) = *SP
@SP
A=M-1
D=M
@ARG
A=M
M=D
// SP = ARG + 1
@ARG
D=M+1
@SP
M=D
// THAT = *(R13) - 1
@R13
D=M-1
AM=D
D=M
@THAT
M=D
//THIS = *(R13) - 2
@R13
D=M-1
AM=D
D=M
@THIS
M=D
//ARG = *(R13) - 3
@R13
D=M-1
AM=D
D=M
@ARG
M=D
//LCL = *(R13) - 4
@R13
D=M-1
AM=D
D=M
@LCL
M=D
// goto ret address
@R14
A=M
0;JMP


