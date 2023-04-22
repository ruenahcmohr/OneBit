
;
; tests INJMP, JPS
;
;
;  output 1 follows input 1
;
;
;
START:
  INJMP 1 TWO
  HALT
  
ONE:  
  SETB 1
  JMP NEXT
  
TWO:
 JPS ONE
 CLRB  1

NEXT:  
 IN 2
 JPC THREE
 SETB 2
 JMP START
THREE:
 CLRB 2
 JMP START
