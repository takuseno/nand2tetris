@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M-D
@JUMP0
D;JEQ
@0
D=A
@JUMP1
0;JMP
(JUMP0)
@0
D=!A
(JUMP1)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M-D
@JUMP2
D;JEQ
@0
D=A
@JUMP3
0;JMP
(JUMP2)
@0
D=!A
(JUMP3)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M-D
@JUMP4
D;JEQ
@0
D=A
@JUMP5
0;JMP
(JUMP4)
@0
D=!A
(JUMP5)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M-D
@JUMP6
D;JLT
@0
D=A
@JUMP7
0;JMP
(JUMP6)
@0
D=!A
(JUMP7)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M-D
@JUMP8
D;JLT
@0
D=A
@JUMP9
0;JMP
(JUMP8)
@0
D=!A
(JUMP9)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M-D
@JUMP10
D;JLT
@0
D=A
@JUMP11
0;JMP
(JUMP10)
@0
D=!A
(JUMP11)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M-D
@JUMP12
D;JGT
@0
D=A
@JUMP13
0;JMP
(JUMP12)
@0
D=!A
(JUMP13)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M-D
@JUMP14
D;JGT
@0
D=A
@JUMP15
0;JMP
(JUMP14)
@0
D=!A
(JUMP15)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M-D
@JUMP16
D;JGT
@0
D=A
@JUMP17
0;JMP
(JUMP16)
@0
D=!A
(JUMP17)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@57
D=A
@SP
A=M
M=D
@SP
M=M+1
@31
D=A
@SP
A=M
M=D
@SP
M=M+1
@53
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M+D
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@112
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M-D
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@SP
A=M
A=A-1
D=M
M=-D
@SP
A=M
A=A-1
D=M
A=A-1
D=M&D
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@82
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
D=M|D
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@SP
A=M
A=A-1
D=M
M=!D
