cat     START   0

MAIN    LDA     #0              
LOOP    .TD     INDEV           
        .JEQ    LOOP            

        RD      INDEV           
        COMP    CHECK          
        JEQ     DONE            

WAITOUT .TD     OUTDEV           
        .JEQ     WAITOUT         
        WD      OUTDEV          

        J       LOOP            

DONE    J       DONE     .CTRL + C       

.podatki
INDEV   BYTE    X'00'           
OUTDEV  BYTE    X'01'           
CHECK   BYTE    X'04'    .CTRL + C

        END     MAIN
