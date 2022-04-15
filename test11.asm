
;
; tests outin
;
;
; copy input 0 to output 1
;    will be delayed by 1 loop
;
;
START:
  OUTIN 1 0
  JMP START
