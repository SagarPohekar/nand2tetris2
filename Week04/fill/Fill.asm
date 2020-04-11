// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

(MAINLOOP)
    //Load KBD register value
    @KBD
    D=M

    // If KBD == 0 then make screen white
    @WHITE
    D;JEQ

    // If KBD != 0 then make screen black
    @BLACK
    D;JGT

    // Infinte the loop
    @MAINLOOP
    0;JMP

(BLACK)
    // Load scrren base address
    @SCREEN
    D=A

    // store scrren base address in addr
    @addr
    M=D

    // initialize rb = 0
    @rb
    M=0

    // initialize row multiplier, rm = 32
    @32
    D=A
    @rm
    M=D

    (B_ROW_WISE_LOOP)
        @rb
        D=M
        @256
        D=D-A // if rb >= 256
        @MAINLOOP // loop exit condition
        D;JEQ

        // initialize cb = 0
        @0
        D=A
        @cb
        M=D

        (B_COLUMN_WISE_LOOP)
            @cb
            D=M
            @32
            D=D-A // if cb >= 32
            @B_INNERLOOP_AFTER_EXIT
            D;JEQ

            @cb
            D=M

            @addr
            A=D+M
            
            M=-1 // make 16 pixels black

            @cb
            M=M+1 // cb = cb + 1

            @B_COLUMN_WISE_LOOP
            0;JMP
        
        (B_INNERLOOP_AFTER_EXIT)
            @rb
            M=M+1 // rb = rb + 1

            @rm
            D=M

            @addr   // addr = 32 * rb
            M=D+M

            @B_ROW_WISE_LOOP
            0;JMP

    
(WHITE)
    // Load scrren base address
    @SCREEN
    D=A

    // store scrren base address in addrw
    @addrw
    M=D

    // initialize rw = 0
    @rw
    M=0

    // initialize row multiplier, rm = 32
    @32
    D=A
    @rm
    M=D

    (W_ROW_WISE_LOOP)
        @rw
        D=M
        @256
        D=D-A
        @MAINLOOP // loop exit condition
        D;JEQ

        // initialize cw = 0
        @0
        D=A
        @cw
        M=D

        (W_COLUMN_WISE_LOOP)
            @cw
            D=M
            @32
            D=D-A
            @W_INNERLOOP_AFTER_EXIT
            D;JEQ

            @cw
            D=M

            @addrw
            A=D+M
            
            M=0 // make 16 pixels white

            @cw
            M=M+1 // cw = cw + 1

            @W_COLUMN_WISE_LOOP
            0;JMP
        
        (W_INNERLOOP_AFTER_EXIT)
            @rw
            M=M+1 // rw = rw + 1

            @rm
            D=M

            @addrw   // addrw = 32 * rw
            M=D+M

            @W_ROW_WISE_LOOP
            0;JMP