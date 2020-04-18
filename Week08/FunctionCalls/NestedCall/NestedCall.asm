// Bootstrap code
//@256
//D=A
//@SP
//M=D
@Sys.init
0;JMP

// File: Sys.vm

// function Sys.init
(Sys.init)
@SP
D=M
@LCL
M=D

// push constant 4000
@4000
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop pointer 0
@SP
AM=M-1
D=M
@THIS
M=D


// push constant 5000
@5000
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D


// call Sys.main
// call : push Sys$ret.0
@Sys$ret.0
D=A
@SP
A=M
M=D
@SP
M=M+1

// call : push LCL
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1

// call : push ARG
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1

// call : push THIS
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1

// call : push THAT
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1

// set up LCL = SP
@SP
D=M
@LCL
M=D
// set up ARG = LCL - 5 - nArgs
@0
D=D-A
@5
D=D-A
@ARG
M=D
// goto Sys.main
@Sys.main
0;JMP
(Sys$ret.0)


// pop temp 1
@1
D=A

@R5
D=D+A // D = R5 + index

@R13
M=D // R13 = D (SP)

@SP
AM=M-1
D=M
@R13
A=M
M=D // RAM[R5 + index] = *SP


// label Sys$LOOP
(Sys$LOOP)

// goto Sys$LOOP
@Sys$LOOP
0;JMP

// function Sys.main
(Sys.main)
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
// push local 2 = 0
A=A+1
M=D
// push local 3 = 0
A=A+1
M=D
// push local 4 = 0
A=A+1
M=D
// SP = top of the stack
A=A+1
D=A
@SP
M=D


// push constant 4001
@4001
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop pointer 0
@SP
AM=M-1
D=M
@THIS
M=D


// push constant 5001
@5001
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D


// push constant 200
@200
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop local 1
@1
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


// push constant 40
@40
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop local 2
@2
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


// push constant 6
@6
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop local 3
@3
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


// push constant 123
@123
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// call Sys.add12
// call : push Sys$ret.1
@Sys$ret.1
D=A
@SP
A=M
M=D
@SP
M=M+1

// call : push LCL
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1

// call : push ARG
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1

// call : push THIS
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1

// call : push THAT
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1

// set up LCL = SP
@SP
D=M
@LCL
M=D
// set up ARG = LCL - 5 - nArgs
@1
D=D-A
@5
D=D-A
@ARG
M=D
// goto Sys.add12
@Sys.add12
0;JMP
(Sys$ret.1)


// pop temp 0
@0
D=A

@R5
D=D+A // D = R5 + index

@R13
M=D // R13 = D (SP)

@SP
AM=M-1
D=M
@R13
A=M
M=D // RAM[R5 + index] = *SP


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


// push local 2
@2
D=A
@LCL
A=D+M // A = LCL + 2
D=M // D = RAM[LCL + 2]
@SP
A=M
M=D
@SP
M=M+1 // SP++


// push local 3
@3
D=A
@LCL
A=D+M // A = LCL + 3
D=M // D = RAM[LCL + 3]
@SP
A=M
M=D
@SP
M=M+1 // SP++


// push local 4
@4
D=A
@LCL
A=D+M // A = LCL + 4
D=M // D = RAM[LCL + 4]
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


// add
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=D+M


// add
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=D+M


// add
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=D+M


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
//Reposition return value in *(ARG + 0) = *(SP--)
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


// function Sys.add12
(Sys.add12)
@SP
D=M
@LCL
M=D

// push constant 4002
@4002
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop pointer 0
@SP
AM=M-1
D=M
@THIS
M=D


// push constant 5002
@5002
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D


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


// push constant 12
@12
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// add
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=D+M


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
//Reposition return value in *(ARG + 0) = *(SP--)
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


