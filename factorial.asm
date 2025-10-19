in
popreg AX
pushreg AX
call :1
mul
out
hlt

:1                  ; recursive factorial
pushreg AX
push 1
jne :2
push 1
ret
:2
pushreg AX
push 1
sub
popreg AX
pushreg AX
call :1
mul
ret
