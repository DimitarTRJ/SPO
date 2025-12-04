print   START   0
        LDX     #0  

device  TD      devAA            
        JEQ     device           

        LDCH    MSG, X           
        COMP    #0               
        JEQ     newline          

        WD      devAA            
        TIX     #1               
        J       device           

newline TD      devAA            
        JEQ     newline
        LDCH    NL               
        WD      devAA          

halt    J       halt             

.podatki
MSG     BYTE    C'SIC/XE'        
        BYTE    X'00'            
NL      BYTE    X'0A'            
devAA   BYTE    X'AA'            

        END     print
