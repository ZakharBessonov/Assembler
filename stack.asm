in
in
in
popreg CX
popreg BX
popreg AX
push 0
pushreg AX
je :1
pop
pushreg BX
pushreg BX
mul
push -4
pushreg AX
pushreg CX
mul
mul
add
popreg DX
pushreg DX
ja :3
je :2
push 2
out
pop
sqr
push -1
mul
pushreg BX
push -1
mul
add
push 2
pushreg AX
dmp
mul
div
out
popreg EX
pushreg CX
pushreg AX
pushreg EX
mul
div
out
dmp
hlt
:3
push 0
out
hlt
:2
push 1
out
pushreg BX
push -1
mul
push 2
pushreg AX
mul
div
out
hlt
:1
push -1
out
hlt

