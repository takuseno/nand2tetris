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
(LOOP)
    @KBD
    D=M
    @WHITE
    D;JEQ
(BLACK)
    @0
    D=A
    D=D-1
    @COLOR
    M=D
    @FILL
    0;JMP
(WHITE)
    @0
    D=A
    @COLOR
    M=D
(FILL)
    @8192
    D=A
    @i
    M=D
(FILLLOOP)
    @i
    D=M
    @SCREEN
    D=D+A
    @INDEX
    M=D
    @COLOR
    D=M
    @INDEX
    A=M
    M=D
    @i
    D=M
    D=D-1
    M=D
    @FILLLOOP
    D;JGT
    @LOOP
    0;JMP
