// push constant 17
@17
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 17
@17
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// eq
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
D=M-D // D = arg1 - arg2
@_$FALSESTATE$0
D;JNE

@SP
A=M-1
M=-1 // arg1 == arg2
@_$CONTINUESTATE$0
0;JMP

(_$FALSESTATE$0)
  @SP
  A=M-1
  M=0 // !(arg1 == arg2)
(_$CONTINUESTATE$0)



// push constant 17
@17
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 16
@16
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// eq
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
D=M-D // D = arg1 - arg2
@_$FALSESTATE$1
D;JNE

@SP
A=M-1
M=-1 // arg1 == arg2
@_$CONTINUESTATE$1
0;JMP

(_$FALSESTATE$1)
  @SP
  A=M-1
  M=0 // !(arg1 == arg2)
(_$CONTINUESTATE$1)



// push constant 16
@16
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 17
@17
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// eq
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
D=M-D // D = arg1 - arg2
@_$FALSESTATE$2
D;JNE

@SP
A=M-1
M=-1 // arg1 == arg2
@_$CONTINUESTATE$2
0;JMP

(_$FALSESTATE$2)
  @SP
  A=M-1
  M=0 // !(arg1 == arg2)
(_$CONTINUESTATE$2)



// push constant 892
@892
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 891
@891
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// lt
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
D=M-D // D = arg1 - arg2
@_$FALSESTATE$3
D;JGE

@SP
A=M-1
M=-1 // arg1 < arg2
@_$CONTINUESTATE$3
0;JMP

(_$FALSESTATE$3)
  @SP
  A=M-1
  M=0 // !(arg1 < arg2)
(_$CONTINUESTATE$3)



// push constant 891
@891
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 892
@892
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// lt
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
D=M-D // D = arg1 - arg2
@_$FALSESTATE$4
D;JGE

@SP
A=M-1
M=-1 // arg1 < arg2
@_$CONTINUESTATE$4
0;JMP

(_$FALSESTATE$4)
  @SP
  A=M-1
  M=0 // !(arg1 < arg2)
(_$CONTINUESTATE$4)



// push constant 891
@891
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 891
@891
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// lt
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
D=M-D // D = arg1 - arg2
@_$FALSESTATE$5
D;JGE

@SP
A=M-1
M=-1 // arg1 < arg2
@_$CONTINUESTATE$5
0;JMP

(_$FALSESTATE$5)
  @SP
  A=M-1
  M=0 // !(arg1 < arg2)
(_$CONTINUESTATE$5)



// push constant 32767
@32767
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 32766
@32766
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// gt
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
D=M-D // D = arg1 - arg2
@_$FALSESTATE$6
D;JLE

@SP
A=M-1
M=-1 // arg1 > arg2
@_$CONTINUESTATE$6
0;JMP

(_$FALSESTATE$6)
  @SP
  A=M-1
  M=0 // !(arg1 > arg2)
(_$CONTINUESTATE$6)



// push constant 32766
@32766
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 32767
@32767
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// gt
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
D=M-D // D = arg1 - arg2
@_$FALSESTATE$7
D;JLE

@SP
A=M-1
M=-1 // arg1 > arg2
@_$CONTINUESTATE$7
0;JMP

(_$FALSESTATE$7)
  @SP
  A=M-1
  M=0 // !(arg1 > arg2)
(_$CONTINUESTATE$7)



// push constant 32766
@32766
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 32766
@32766
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// gt
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
D=M-D // D = arg1 - arg2
@_$FALSESTATE$8
D;JLE

@SP
A=M-1
M=-1 // arg1 > arg2
@_$CONTINUESTATE$8
0;JMP

(_$FALSESTATE$8)
  @SP
  A=M-1
  M=0 // !(arg1 > arg2)
(_$CONTINUESTATE$8)



// push constant 57
@57
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 31
@31
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// push constant 53
@53
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


// push constant 112
@112
D=A

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


// neg
@SP
A=M-1
M=-M


// and
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=D&M


// push constant 82
@82
D=A

// *SP = D
@SP
A=M
M=D

@SP
M=M+1 // SP++


// or
@SP
AM=M-1 // SP--
D=M // D = *SP
A=A-1 // A = SP - 1
M=D|M


// not
@SP
A=M-1
M=!M


