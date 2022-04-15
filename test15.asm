
;
;
; tests injpc halt 
;
; if input 0 is clear, system will halt
; otherwise, output 1 will be a copy of input 1
;
;
START:
  INJPC 0 B
  IN 1
  OUTJMP 1, START
  
B:
 HALT
