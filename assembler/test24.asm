;
;
; test SETBSKIP
; 
;  output 0 should follow input 0
;
;  set bit and skip next instruction.
;
START:
  IN 0
  JPC SET
  SETBSKIP 0
SET:
  CLRB 0
  JMP START
  
