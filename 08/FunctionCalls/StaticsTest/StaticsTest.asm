@256
D=A
@SP
M=D
@SP
D=M
@R14
M=D
@RETURN0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@R14
D=M
@ARG
M=D
@Sys.init
0;JMP
(RETURN0)
(Class1.set)
@SP
D=M
@LCL
M=D
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@Class1.0
D=A
@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D
@SP
M=M-1
@ARG
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@Class1.1
D=A
@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D
@SP
M=M-1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M-1
D=M
@R13
M=D
@ARG
A=M
D=A+1
@SP
M=D
@LCL
A=M
D=A-1
@R15
M=D
@R15
A=M
D=M
@THAT
M=D
@R15
M=M-1
A=M
D=M
@THIS
M=D
@R15
M=M-1
A=M
D=M
@ARG
M=D
@R15
M=M-1
A=M
D=M
@LCL
M=D
@R15
M=M-1
A=M
D=M
@R14
M=D
@R13
D=M
@SP
A=M-1
M=D
@R14
A=M
0;JMP
(Class1.get)
@SP
D=M
@LCL
M=D
@Class1.0
D=M
@SP
A=M
M=D
@SP
M=M+1
@Class1.1
D=M
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
A=M-1
D=M
@R13
M=D
@ARG
A=M
D=A+1
@SP
M=D
@LCL
A=M
D=A-1
@R15
M=D
@R15
A=M
D=M
@THAT
M=D
@R15
M=M-1
A=M
D=M
@THIS
M=D
@R15
M=M-1
A=M
D=M
@ARG
M=D
@R15
M=M-1
A=M
D=M
@LCL
M=D
@R15
M=M-1
A=M
D=M
@R14
M=D
@R13
D=M
@SP
A=M-1
M=D
@R14
A=M
0;JMP
(Sys.init)
@SP
D=M
@LCL
M=D
@6
D=A
@SP
A=M
M=D
@SP
M=M+1
@8
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@R14
M=D
@RETURN1
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@R14
D=M
D=D-1
D=D-1
@ARG
M=D
@Class1.set
0;JMP
(RETURN1)
@R5
D=A
@0
D=D+A
@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D
@SP
M=M-1
@23
D=A
@SP
A=M
M=D
@SP
M=M+1
@15
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@R14
M=D
@RETURN2
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@R14
D=M
D=D-1
D=D-1
@ARG
M=D
@Class2.set
0;JMP
(RETURN2)
@R5
D=A
@0
D=D+A
@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D
@SP
M=M-1
@SP
D=M
@R14
M=D
@RETURN3
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@R14
D=M
@ARG
M=D
@Class1.get
0;JMP
(RETURN3)
@SP
D=M
@R14
M=D
@RETURN4
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@R14
D=M
@ARG
M=D
@Class2.get
0;JMP
(RETURN4)
(Sys.init$WHILE)
@Sys.init$WHILE
0;JMP
(Class2.set)
@SP
D=M
@LCL
M=D
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@Class2.0
D=A
@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D
@SP
M=M-1
@ARG
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@Class2.1
D=A
@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D
@SP
M=M-1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M-1
D=M
@R13
M=D
@ARG
A=M
D=A+1
@SP
M=D
@LCL
A=M
D=A-1
@R15
M=D
@R15
A=M
D=M
@THAT
M=D
@R15
M=M-1
A=M
D=M
@THIS
M=D
@R15
M=M-1
A=M
D=M
@ARG
M=D
@R15
M=M-1
A=M
D=M
@LCL
M=D
@R15
M=M-1
A=M
D=M
@R14
M=D
@R13
D=M
@SP
A=M-1
M=D
@R14
A=M
0;JMP
(Class2.get)
@SP
D=M
@LCL
M=D
@Class2.0
D=M
@SP
A=M
M=D
@SP
M=M+1
@Class2.1
D=M
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
A=M-1
D=M
@R13
M=D
@ARG
A=M
D=A+1
@SP
M=D
@LCL
A=M
D=A-1
@R15
M=D
@R15
A=M
D=M
@THAT
M=D
@R15
M=M-1
A=M
D=M
@THIS
M=D
@R15
M=M-1
A=M
D=M
@ARG
M=D
@R15
M=M-1
A=M
D=M
@LCL
M=D
@R15
M=M-1
A=M
D=M
@R14
M=D
@R13
D=M
@SP
A=M-1
M=D
@R14
A=M
0;JMP