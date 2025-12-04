horner  START   0
        
        LDA     x          
        STA     tempX    

.(x + 2)
        ADD     temp2

.(x + 2) * x + 3
        MUL     tempX
        ADD     temp3

.((x + 2)x + 3) * x + 4
        MUL     tempX
        ADD     temp4

.(((x + 2)x + 3)x + 4) * x + 5
        MUL     tempX
        ADD     temp5

        STA     rezultat  

halt    J       halt

.podatki
x        WORD    2
temp2    WORD    2
temp3    WORD    3
temp4    WORD    4
temp5    WORD    5
tempX    RESW    1
rezultat RESW    1
