;
;
; test JMPT
; 
;  output 0 should follow input 0
;
;  Jump Table - F is added to the jump address.
;
START:
  IN 0
  JMPT DOOUT
  
DOOUT:
  CLRBJMP 0 START
  SETBJMP 0 START
