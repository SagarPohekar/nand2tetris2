// push constant 7
@7
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 8
@8
D=A

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


