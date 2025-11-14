arithr  START   0
        LDS     x
        LDT     y

.sum
        LDA     zero
        ADDR    S, A
        ADDR    T, A
        STA     sum
        SUBR    T, A

.diff     
        SUBR    T, A
        STA     diff
        ADDR    T, A

.prod
        MULR    T, A
        STA     prod
        DIVR    T, A

.quot
        DIVR    T, A
        STA     quot

.mod
        MULR    T, A
        SUBR    A, S
        .RMO     S, A
        STS     mod

halt    J   halt

.podatki
x        WORD   15
y        WORD   7
zero     WORD   0
sum      RESW   1
diff     RESW   1
prod     RESW   1
quot     RESW   1
mod      RESW   1