// push constant 3030
@3030
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// pop pointer 0
@SP
AM=M-1
D=M
@THIS
M=D


// push constant 3040
@3040
D=A

// *SP = D
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


// push constant 32
@32
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// pop this 2
@2
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



// push constant 46
@46
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// pop that 6
@6
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



// push pointer 0
@THIS
D=M
// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++



// push pointer 1
@THAT
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


// push this 2
@2
D=A

@THIS
A=D+M // A = THIS + 2
D=M // D = RAM[THIS + 2

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


// push that 6
@6
D=A

@THAT
A=D+M // A = THAT + 6
D=M // D = RAM[THAT + 6

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


