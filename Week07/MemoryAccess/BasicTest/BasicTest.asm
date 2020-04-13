// push constant 10
@10
D=A

// *SP = D
@SP
A=M
M=D

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



// push constant 21
@21
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 22
@22
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// pop argument 2
@2
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



// pop argument 1
@1
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



// push constant 36
@36
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// pop this 6
@6
D=A

@THIS
D=D+M // D = THIS + index

@R13
M=D // R13 = D (SP)

// D = RAM[SP--]
@SP
AM=M-1
D=M

@R13
A=M
M=D // RAM[LCL + index] = *SP



// push constant 42
@42
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 45
@45
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// pop that 5
@5
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



// push constant 510
@510
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// pop temp 6
@6
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
D=M // D = RAM[LCL + 0

@SP
A=M
M=D

@SP
M=M+1 // SP++


// push that 5
@5
D=A

@THAT
A=D+M // A = THAT + 5
D=M // D = RAM[THAT + 5

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
D=M // D = RAM[ARG + 1

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


// push this 6
@6
D=A

@THIS
A=D+M // A = THIS + 6
D=M // D = RAM[THIS + 6

@SP
A=M
M=D

@SP
M=M+1 // SP++


// push this 6
@6
D=A

@THIS
A=D+M // A = THIS + 6
D=M // D = RAM[THIS + 6

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


// sub
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=M-D


// push temp 6
@6
D=A

@R5
A=D+A
D=M
// *SP = D
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


