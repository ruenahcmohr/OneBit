;
;  tests skipic  skipis
;
; copies input 0 to output 0
;
;
;
;
;
START:
  SKIPIC 0
  SETB 0
  SKIPIS 0
  CLRB 0
  JMP START
