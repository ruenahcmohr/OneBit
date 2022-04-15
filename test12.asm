;
; tests reptic  reptis NOP
;
; copies input 0 to output 0
;
;
;
;
;
;
START:
  NOP
  REPTIC 0
  SETB 0
  NOP
  REPTIS 0
  CLRB 0
  JMP START
