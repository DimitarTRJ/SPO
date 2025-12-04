screen  START   0

        LDA     SCRCOLS
        MUL     SCRROWS
        STA     SCRLEN

        CLEAR   A

        LDA     SCREEN
        STA     address
        JSUB    scrfill

        LDA     SCREEN
        STA     address
        JSUB    scrclear

halt    J       halt  


.scrfill
scrfill LDX     #0
        LDS     #1
        LDA     SYMBOL

        +LDT    address
        STT     address
        STA     @address

loop    +LDT    address
        ADDR    S, T
        STT     address
        STA     @address

        TIX     SCRLEN
        JLT     loop

        RSUB

.scrclear
scrclear  LDX     #0
        LDS     #1
        LDA     #0

        +LDT    address
        STT     address
        STA     @address

loopc    +LDT    address
        ADDR    S, T
        STT     address
        STA     @address

        TIX     SCRLEN
        JLT     loopc

        RSUB

.podatki
SCREEN   WORD   X'00B800'
SCRCOLS  WORD   80
SCRROWS  WORD   25
SCRLEN   RESW   1
SYMBOL   BYTE   X'50'    . P
address     RESW   1