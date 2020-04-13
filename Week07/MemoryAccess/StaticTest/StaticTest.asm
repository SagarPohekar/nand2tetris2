// push constant 111
@111
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 333
@333
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 888
@888
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// pop static 8
@SP
AM=M-1
D=M
@StaticTest.8
M=D // @StaticTest.8 = *SP


// pop static 3
@SP
AM=M-1
D=M
@StaticTest.3
M=D // @StaticTest.3 = *SP


// pop static 1
@SP
AM=M-1
D=M
@StaticTest.1
M=D // @StaticTest.1 = *SP


// push static 3
@3
D=A

@StaticTest.3
D=M

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++



// push static 1
@1
D=A

@StaticTest.1
D=M

// *SP = D
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


// push static 8
@8
D=A

@StaticTest.8
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


