bonus   START   0

        JSUB    lenght      
        LDA     len

        JSUB    reverse     

        JSUB    palindrom

        LDA     result
        ADD     #48
        WD      output
        
halt    J       halt

.---------------------------------
lenght  LDX     #0
        STL     saveL
loop1   LDCH    MSG, X
        COMP    #0
        JEQ     done
        TIX     #50
        JLT     loop1

done    STX     len
        LDL     saveL
        RSUB

.---------------------------------
reverse LDX     len
        STL     saveL
        LDT     #0
        LDS     #1

        SUBR    S, X

loop2   LDCH    MSG, X

        STX     tempX
        STT     tempT
        LDX     tempT
        
        STCH    RMSG, X

        LDX     tempX

        ADDR    S, T
        SUBR    S, X
        STX     temp
        LDA     temp
        COMP    #0
        JGT     loop2 
        JEQ     loop2   

        LDL     saveL
        RSUB


.---------------------------------
palindrom   STL     saveL
        LDX     #0
        LDS     len

loop3   LDCH    RMSG, X
        RMO     A, T
        LDCH    MSG, X
        TIX     #50
        COMPR   X, S
        JEQ     IsPalindrom
        COMPR   A, T
        JEQ     loop3
        JLT     NotPalindrom
        JGT     NotPalindrom

IsPalindrom  LDL     saveL
            LDS     #1
            STS     result
            RSUB

NotPalindrom    LDL     saveL
            LDS     #0
            STS     result
            RSUB

.----------------------------
.Podatki
MSG     BYTE    C'racecar'        
        BYTE    X'00'

RMSG    RESB    50

len     RESW    1

temp    RESW    1
tempX   RESW    1
tempT   RESW    1
saveL   RESW    1

result  RESW    1

output  BYTE    X'01'