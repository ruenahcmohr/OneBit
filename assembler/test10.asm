
;
;
; tests skipfc skipfc
;
;
;output 0 follows input 0
;
;
START:
  IN 0
  SKIPFC   
  SETB 0
  SKIPFS
  CLRB 0
  JMP START  
