rec         START   0


            JSUB    stackinit      . inicializiraj sklad

loop        LDA     #4
            .COMP    #0
            .JEQ     done           

            LDT     #1

            JSUB    fact

            STT     result
            LDA     result

            ADD     #48         .output
            WD      FB

            LDCH    newline     .newline      
            WD      FB 

halt        J       halt

.-------------------------------------------------------------------------------------------
fact        STA     n
            COMP    #0
            JLT     basecase
            JEQ     basecase 


            STL     temp        . pusham L na stack
            LDA     temp
            STA     @stackptr 
            JSUB    stackpush

            LDA     n           . pusham n na stack
            STA     @stackptr
            JSUB    stackpush

            LDA     n            . n--
            SUB     #1
            STA     n

            JSUB    fact   .------------------

            JSUB    stackpop
            LDA     @stackptr  
            MULR    A, T

            JSUB    stackpop
            LDL     @stackptr


basecase    RSUB

.-----------------------------------------------------------------------------------------

.-----stackinit – nastavi kazalec stackptr----
stackinit  STL     saveL
           LDA     #stackmem
           STA     stackptr
           LDL     saveL
           RSUB



.------stackpush – poveča kazalec na vrh sklada-----
stackpush  STL     saveL
           LDA     stackptr
           ADD     #3          . vsaka beseda ima 3 bajte
           STA     stackptr
           LDL     saveL
           RSUB


.------stackpop – zmanjša kazalec na vrh sklada-----
stackpop   STL     saveL
           LDA     stackptr
           SUB     #3
           STA     stackptr
           LDL     saveL
           RSUB


.--------------------------------------------------------------------------------
.Podatki

FA      BYTE    X'00'           .input device
FB      BYTE    X'01'           .output device

input     RESW    1
result    RESW    1
n         RESW    1
temp      RESW    1

stackptr  RESW    1
saveL     RESW    1
stackmem  RESW    100          .Stack

newline   BYTE    X'0A'
