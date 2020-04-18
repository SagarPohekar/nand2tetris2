// Bootstrap code
@261
D=A
@SP
M=D
@Sys.init
0;JMP

// File: Main.vm

// function Main.fibonacci
(Main.fibonacci)
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


// push constant 2
@2
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// lt
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
D=M-D // D = arg1 - arg2
@_$FALSESTATE$0
D;JGE

@SP
A=M-1
M=-1 // arg1 < arg2
@_$CONTINUESTATE$0
0;JMP

(_$FALSESTATE$0)
  @SP
  A=M-1
  M=0 // !(arg1 < arg2)
(_$CONTINUESTATE$0)



// if-goto Main$IF_TRUE
@SP
AM=M-1
D=M
@Main$IF_TRUE
D;JNE


// goto Main$IF_FALSE
@Main$IF_FALSE
0;JMP

// label Main$IF_TRUE
(Main$IF_TRUE)

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


// label Main$IF_FALSE
(Main$IF_FALSE)

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


// call Main.fibonacci
// call : push Main$ret.0
@Main$ret.0
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
// goto Main.fibonacci
@Main.fibonacci
0;JMP
(Main$ret.0)


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


// call Main.fibonacci
// call : push Main$ret.1
@Main$ret.1
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
// goto Main.fibonacci
@Main.fibonacci
0;JMP
(Main$ret.1)


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


// File: Sys.vm

// function Sys.init
(Sys.init)
@SP
D=M
@LCL
M=D

// push constant 4
@4
D=A

@SP
A=M
M=D // *SP = D
@SP
M=M+1 // SP++


// call Main.fibonacci
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
@1
D=D-A
@5
D=D-A
@ARG
M=D
// goto Main.fibonacci
@Main.fibonacci
0;JMP
(Sys$ret.2)


// label Sys$WHILE
(Sys$WHILE)

// goto Sys$WHILE
@Sys$WHILE
0;JMP

