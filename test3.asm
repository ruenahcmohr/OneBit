;
;
; test injps, clrbjmp setbjmp
; 
;  output 0 should follow input 0
;
;
;
START:
  INJPS 0 SET
  CLRBJMP 0 START 
  
SET:
  SETBJMP 0 START
