in
in
in
in
push 1
popreg EX
push 2
mul
popreg FX
push 0
popreg GX
pushreg FX
push 0
je :7
:6
pushreg EX
push 10
mul
popreg EX
push 1
pushreg GX
add
popreg GX
pushreg GX
pushreg FX
jb :6
:7
pushreg EX
sqr
popreg HX
popreg CX
popreg BX
popreg AX
push 0
pushreg AX
je :1
push 0
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
push 0
pushreg DX
je :2
push 2
out
pushreg DX
pushreg EX
mul
sqr
push -1
mul
pushreg BX
pushreg HX
push -1
mul
mul
add
push 2
pushreg AX
mul
div
out
pushreg DX
pushreg EX
mul
sqr
pushreg BX
pushreg HX
push -1
mul
mul
add
push 2
pushreg AX
mul
div
out
hlt
:3
push 0
out
hlt
:2
pushreg BX
pushreg HX
push -1
mul
mul
push 2
pushreg AX
mul
div
push 1
out
out
hlt
:1
pushreg BX
push 0
je :4
pushreg CX
pushreg HX
mul
pushreg BX
push -1
mul
div
push 1
out
out
hlt
:4
pushreg CX
push 0
je :5
push 0
out
hlt
:5
push 102 ;f
push 110 ;n
push 105 ;i
outc
outc
outc
hlt

