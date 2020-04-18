// Bootstrap code
@261
D=A
@SP
M=D
@Sys.init
0;JMP

// File: Class1.vm

// function Class1.set
(Class1.set)
@SP
D=M
@LCL
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


// pop static 0
@SP
AM=M-1
D=M
@Class1.0
M=D // @Class1.0 = *SP


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


// pop static 1
@SP
AM=M-1
D=M
@Class1.1
M=D // @Class1.1 = *SP


// push constant 0
@0
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


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


// function Class1.get
(Class1.get)
@SP
D=M
@LCL
M=D

// push static 0
@0
D=A

@Class1.0
D=M

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// push static 1
@1
D=A

@Class1.1
D=M

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


// File: Class2.vm

// function Class2.set
(Class2.set)
@SP
D=M
@LCL
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


// pop static 0
@SP
AM=M-1
D=M
@Class2.0
M=D // @Class2.0 = *SP


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


// pop static 1
@SP
AM=M-1
D=M
@Class2.1
M=D // @Class2.1 = *SP


// push constant 0
@0
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


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


// function Class2.get
(Class2.get)
@SP
D=M
@LCL
M=D

// push static 0
@0
D=A

@Class2.0
D=M

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// push static 1
@1
D=A

@Class2.1
D=M

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


// File: Sys.vm

// function Sys.init
(Sys.init)
@SP
D=M
@LCL
M=D

// push constant 6
@6
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// push constant 8
@8
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// call Class1.set
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
@2
D=D-A
@5
D=D-A
@ARG
M=D
// goto Class1.set
@Class1.set
0;JMP
(Sys$ret.0)


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


// push constant 23
@23
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// push constant 15
@15
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// call Class2.set
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
@2
D=D-A
@5
D=D-A
@ARG
M=D
// goto Class2.set
@Class2.set
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


// call Class1.get
// call : push Sys$ret.2
@Sys$ret.2
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
// goto Class1.get
@Class1.get
0;JMP
(Sys$ret.2)


// call Class2.get
// call : push Sys$ret.3
@Sys$ret.3
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
// goto Class2.get
@Class2.get
0;JMP
(Sys$ret.3)


// label Sys$WHILE
(Sys$WHILE)

// goto Sys$WHILE
@Sys$WHILE
0;JMP

