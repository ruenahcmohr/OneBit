
;
; test waitis, coutf
;
;  waits until input 0 is clear
;  copies input 1 to output 1
;  copies the inverse of input 1 to output 0
;
;
START:
  WAITIS 0
  IN 1
  COUTF   0 0 1 0
  COUTF   1 1 0 1
  JMP START   
