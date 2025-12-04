arith   START   0
.sum
        LDA     x
        ADD     y
        STA     sum

.diff   
        LDA     x
        SUB     y
        STA     diff

.prod   
        LDA     x
        MUL     y
        STA     prod

.quot
        LDA     x
        DIV     y
        STA     quot

.mod
        LDA     y
        MUL     quot
        STA     temp
        LDA     x
        SUB     temp
        STA     mod    


halt    J   halt
.podatki
x        WORD   15
y        WORD   7
temp     WORD   1
sum      RESW   1
diff     RESW   1
prod     RESW   1
quot     RESW   1
mod      RESW   1


