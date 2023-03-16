
;
;  TESTS JPINV 
;
;
; if input 0 is clear, output 0 cycles quickly
;  if input 0 is set, output 0 cycles slowly
;
;
;
;


START:
 JPINV 0 0 SLOW
 SETB 0
 NOP
 CLRB 0
 JMP START
 
SLOW:
 SETB 0
 NOP
 NOP
 NOP
 NOP
 NOP
 NOP
 NOP
 NOP
 CLRB 0
 NOP
 NOP
 NOP
 NOP
 NOP
 NOP
 NOP
 NOP 
 JMP START
