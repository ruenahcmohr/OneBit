;
;
; test in, out, outi
;
; output 0 should follow input 0
; output 1 should be the inverse
;

NORMPORT EQU 0
INVPORT  EQU 1

START:
  IN 1
  OUT NORMPORT
  OUTI INVPORT
  JMP START
