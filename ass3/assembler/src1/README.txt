Commands(CMD):

        1. g++ -std=c++17 src\AssemblerPass1.cpp src\AssemblerPass2.cpp src\Mnemonic.cpp src\Parser.cpp src\ObjectProgramWriter.cpp src\main.cpp -Iincludes -o assembler.exe

        2. assembler.exe test.asm > test.obj

Programs:

1. ----BONUS----

bonus.ams:
.Preveri, ali je niz palindrom. Če je (1), če ni (0)

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



bonus.obj:

Hbonus0000000000F8
T0000001E4B20150320DF4B202D4B20630320E5190030DF20E23F2FFD0500001720D3
T00001E1E53A08A2900003320062D00323B2FF11320B50B20BE4F00000720AC1720B5
T00003C1E7500006D0001944153A0641320A18720A107209E57A06007209590459441
T00005A1E13208B032088290000372FDE332FDB0B20854F000017207F0500006F206D
T0000781E53A038AC0553A02B2D0032A01433200BA005332FEB3B200F37200C0B205B
T0000961D6D00017F20584F00000B204F6D00007F204C4F00007261636563617200
T0000F70101
E000000


=================================================================================
=================================================================================

2.----REC-----

rec.asm:
rec         START   0


            JSUB    stackinit      

loop        LDA     #4
            .COMP    #0
            .JEQ     done           

            LDT     #1

            JSUB    fact

            STT     result
            LDA     result

            ADD     #48         
            WD      FB

            LDCH    newline          
            WD      FB 

halt        J       halt

.--------------------------------------------------
fact        STA     n
            COMP    #0
            JLT     basecase
            JEQ     basecase 


            STL     temp       
            LDA     temp
            STA     @stackptr 
            JSUB    stackpush

            LDA     n           
            STA     @stackptr
            JSUB    stackpush

            LDA     n          
            SUB     #1
            STA     n

            JSUB    fact   

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
           ADD     #3         
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


.---------------------------------------------------
.Podatki

FA      BYTE    X'00'          
FB      BYTE    X'01'           

input     RESW    1
result    RESW    1
n         RESW    1
temp      RESW    1

stackptr  RESW    1
saveL     RESW    1
stackmem  RESW    100         

newline   BYTE    X'0A'


bonus.obj:
H rec    000000 0001D3
T 000000 1E 4B205C 010004 750001 4B2015 872088 032085 190030 DF207B 5321B7 DF2075
T 00001E 1E 3F2FFD 0F2076 290000 3B2032 33202F 17206D 03206A 0E206A 4B2035 03205E
T 00003C 1E 0E2061 4B202C 032055 1D0001 0F204F 4B2FD3 4B202F 02204C 9805 4B2027 0A
T 00005A 1E 2044 4F0000 172041 012041 0F2038 0B2038 4F0000 172032 03202C 190003 0F
T 000078 1C2026 0B2026 4F0000 172020 03201A 1D0003 0F2014 0B2014 4F0000 00 01 
T 0001D2 01       0A
E 000000


=================================================================================
=================================================================================

3. ----STACK----

stack.asm:
stack   START   0

        JSUB    stackinit      

        LDA     temp11         
        STA     @stackptr
        JSUB    stackpush

        LDA     temp22        
        STA     @stackptr
        JSUB    stackpush

        JSUB    stackpop
        LDA     @stackptr      
        STA     test1
        .LDA     #0              
        .STA     @stackptr       

        JSUB    stackpop
        LDA     @stackptr      
        STA     test2
        .LDA     #0             
        .STA     @stackptr       

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
           ADD     #3          
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

stackmem  RESW    100          

        END     


.cstk EXTDEF sinit, spush, spop, stkp

    .EXTREF sinit, spush, spop, stkp ; To je iz drugi program


stack.obj:
Hstack00000000019B
T0000001E4B20270320630E20544B202D03205D0E204B4B20244B20330220420F2045
T00001E1E4B202A0220390F203F3F2FFD17203301203F0F202A0B202A4F0000172024
T00003C1E03201E1900030F20180B20184F000017201203200C1D00030F20060B2006
T00005A034F0000 
T00006906000011000022 
E000000


=================================================================================
=================================================================================

4.----Program for testing BASE, NOBASE----
    
base.asm:
FIRST   START   1000
        LDA     ALPHA      
        BASE    ALPHA       
        LDA     BETA      
        NOBASE             
        LDA     GAMMA       
ALPHA   WORD    1234
BETA    WORD    5678
GAMMA   WORD    9999
        END    

base.obj:
HFIRST 0003E8000012
T0003E8120320060320060320060004D200162E00270F
E0003E8

=================================================================================
=================================================================================

5.----Program for testing format 4 instructuins----

form.asm:
TEST    START   1000

        +LDA    TARGET
        RSUB

TARGET  WORD    12345

        END     

base.obj:
 HTEST  0003E800000A
T0003E80A031003EF4F0000003039
M0003E905
E0003E8
