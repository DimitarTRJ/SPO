stack   START   0

        JSUB    stackinit      . inicializiraj sklad

        LDA     temp11         . testna vrednost 1
        STA     @stackptr
        JSUB    stackpush

        LDA     temp22         . testna vrednost 2
        STA     @stackptr
        JSUB    stackpush

        JSUB    stackpop
        LDA     @stackptr      . preberi z vrha sklada (druga vrednost)
        STA     test1
        .LDA     #0              .pobrisem v stack
        .STA     @stackptr       .pobrisem v stack

        JSUB    stackpop
        LDA     @stackptr      . preberi z vrha sklada (prva vrednost)
        STA     test2
        .LDA     #0              .pobrisem v stack
        .STA     @stackptr       .pobrisem v stack

halt    J       halt


.------------------------------------------
.  stackinit – nastavi kazalec stackptr
.------------------------------------------
stackinit STL     saveL
           LDA     #stackmem
           STA     stackptr
           LDL     saveL
           RSUB


.------------------------------------------
.stackpush – poveča kazalec na vrh sklada
.------------------------------------------
stackpush STL     saveL
           LDA     stackptr
           ADD     #3          . vsaka beseda ima 3 bajte
           STA     stackptr
           LDL     saveL
           RSUB


.------------------------------------------
.stackpop – zmanjša kazalec na vrh sklada
.------------------------------------------
stackpop  STL     saveL
           LDA     stackptr
           SUB     #3
           STA     stackptr
           LDL     saveL
           RSUB


.------------------------------------------
.  Podatki in rezervacija pomnilnika
.------------------------------------------
stackptr  RESW    1
saveL     RESW    1
test1     RESW    1
test2     RESW    1

temp11    WORD    X'000011'
temp22    WORD    X'000022'

stackmem  RESW    100          . prostor za sklad (100 besed = 300 bajtov)

        END     stack



.cstk EXTDEF sinit, spush, spop, stkp

    .EXTREF sinit, spush, spop, stkp ; To je iz drugi program
