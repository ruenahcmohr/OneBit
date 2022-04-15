
; tests, csetb, cclrb
;
;
; this will copy input 0 to output 0, 
;   and copy the inverse of input 0 to output 1
;
START:
  IN 0
  CSETB 1 0 
  CCLRB 0 1
  JMP START
