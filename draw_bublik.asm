in
popreg DX       ; get radius

in
popreg FX       ; get radius of hole

in
popreg EX       ; get stretch coefficient

push 0          ; draw circle
popreg AX
:1
pushreg AX      ; count x
push 80
div
popreg BX

pushreg AX      ; count y
push 80
mod
popreg CX

pushreg DX      ; check condition of circle
pushreg DX
mul

pushreg BX      ; for x
push 40
sub
pushreg BX
push 40
sub
mul
pushreg EX      ; take into consideration stretch coefficient
mul

pushreg CX      ; for y
push 40
sub
pushreg CX
push 40
sub
mul
add

ja :2
push 45         ; draw '-'
popm [AX]
pushreg AX
push 1
add
popreg AX
pushreg AX
push 6400
jb :1
jmp :4

:2
push 35         ; draw '#'
popm [AX]
pushreg AX
push 1
add
popreg AX
pushreg AX
push 6400
jb :1
jmp :4

:4

push 0          ; now do hole
popreg AX
:5

pushreg AX      ; count x
push 80
div
popreg BX

pushreg AX      ; count y
push 80
mod
popreg CX

pushreg FX      ; check condition of circle
pushreg FX
mul

pushreg BX      ; for x
push 40
sub
pushreg BX
push 40
sub
mul
pushreg EX      ; take into consideration stretch coefficient
mul

pushreg CX      ; for y
push 40
sub
pushreg CX
push 40
sub
mul
add

ja :6
pushreg AX      ; draw nothing
push 1
add
popreg AX
pushreg AX
push 6400
jb :5
jmp :7

:6
push 45         ; draw '-'
popm [AX]
pushreg AX
push 1
add
popreg AX
pushreg AX
push 6400
jb :5
jmp :7

:7

draw
hlt








