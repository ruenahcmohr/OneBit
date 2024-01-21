;
;
; test CLRBSKIP
; 
;  output 0 should follow input 0
;
;  clear bit and skip next instruction.
;
START:
  IN 0
  JPS SET
  CLRBSKIP 0
SET:
  SETB 0
  JMP START
  
