poly    START   0
        LDA     zero
.x^4
        LDS     x
        LDT     x
        LDX     zero
loop    MULR    T, S     
        TIX     #3
        JLT     loop
        ADDR    S, A
.       STA     rezultat

.2 * x^3
        LDS     x
        LDT     x
        LDX     zero
loop2    
        MULR    T, S     
        TIX     #2
        JLT     loop2
        LDT     #2
        MULR    T, S
        ADDR    S, A

.       STA     rezultat

.3 * x^2
        LDS     x
        LDT     x
        LDX     zero
loop3    
        MULR    T, S     
        TIX     #1
        JLT     loop3
        LDT     #3
        MULR    T, S
        ADDR    S, A

.       STA     rezultat

.4 * x
        LDS     x
        LDT     #4
        MULR    T, S
        ADDR    S, A

        ADD     #5
        STA     rezultat

halt    J       halt



.podatki
x           WORD    2
zero        WORD    0
rezultat    RESW    1