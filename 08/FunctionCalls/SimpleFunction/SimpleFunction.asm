(nSimpleFunction.test)
@SP
D=M
@LCL
M=D
@SP
A=M
M=0
@SP
M=M+1
@SP
A=M
M=0
@SP
M=M+1
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@1
A=D+A
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
D=D+M
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
@SP
A=M
A=A-1
D=M
A=A-1
D=D+M
@SP
A=M
A=A-1
A=A-1
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
@ARG
A=M
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
0;JMP