;
;
; test WAITISJMP, WAITICJMP
; 
;  output 0 should follow input 0
;
;  
;
START:
  IN 0
  FORKF CLEAR SET
  
CLEAR:
  CLRB 0
  WAITISJMP 0 SET
  
FOO:
  HALT  
  
SET:
  SETB 0
  WAITICJMP 0 CLEAR
  
