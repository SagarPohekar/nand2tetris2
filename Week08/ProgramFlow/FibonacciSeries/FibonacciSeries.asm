// File: FibonacciSeries.vm

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


// pop pointer 1
@SP
AM=M-1
D=M
@THAT
M=D


// push constant 0
@0
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop that 0
@0
D=A
@THAT
D=D+M // D = THAT + index
@R13
M=D // R13 = D (SP)
// D = RAM[SP--]
@SP
AM=M-1
D=M
@R13
A=M
M=D // RAM[LCL + index] = *SP


// push constant 1
@1
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// pop that 1
@1
D=A
@THAT
D=D+M // D = THAT + index
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


// push constant 2
@2
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


// label FibonacciSeries$MAIN_LOOP_START
(FibonacciSeries$MAIN_LOOP_START)

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


// if-goto FibonacciSeries$COMPUTE_ELEMENT
@SP
AM=M-1
D=M
@FibonacciSeries$COMPUTE_ELEMENT
D;JNE


// goto FibonacciSeries$END_PROGRAM
@FibonacciSeries$END_PROGRAM
0;JMP

// label FibonacciSeries$COMPUTE_ELEMENT
(FibonacciSeries$COMPUTE_ELEMENT)

// push that 0
@0
D=A
@THAT
A=D+M // A = THAT + 0
D=M // D = RAM[THAT + 0]
@SP
A=M
M=D
@SP
M=M+1 // SP++


// push that 1
@1
D=A
@THAT
A=D+M // A = THAT + 1
D=M // D = RAM[THAT + 1]
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


// pop that 2
@2
D=A
@THAT
D=D+M // D = THAT + index
@R13
M=D // R13 = D (SP)
// D = RAM[SP--]
@SP
AM=M-1
D=M
@R13
A=M
M=D // RAM[LCL + index] = *SP


// push pointer 1
@THAT
D=M
@SP
A=M
M=D // *SP = D
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


// add
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=D+M


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


// goto FibonacciSeries$MAIN_LOOP_START
@FibonacciSeries$MAIN_LOOP_START
0;JMP

// label FibonacciSeries$END_PROGRAM
(FibonacciSeries$END_PROGRAM)

