// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

// counter for loop, counter = 0
@0
D=M

@i
M=D // i = RAM[0]

// Set RAM[2] = 0
@2
D=M
M=0

//create sign1
@sign1
M=1

@0
D=M
@14
D;JLT

@sign1
M=-1

@sign2
M=1

@1
D=M
@22
D;JLT

@sign2
M=-1

@i
D=M
@ABS
D;JLT

(MUL)
    // Load RAM[0] in D register
    @0
    D=M
    @LOOP
    D;JNE // RAM[0] is +ve/-ve

(LOOP)
    @i
    D=M

    @END
    D;JEQ

    @1
    D=M
    @2
    M=M+D

    @i
    M=M-1

    @LOOP
    0;JMP


(ABS)
    @i
    D=M

    @0
    D=A-D

    @i
    M=D

    @MUL
    0;JMP


(END)
    0;JMP