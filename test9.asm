
;
; tests REPTFC REPTFS
;
;
; output 0 follows input 0
;
;
START:
  IN 0
  REPTFC
  SETB 0
  IN 0
  REPTFS
  CLRB 0
  JMP START
