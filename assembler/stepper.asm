; input 1 is for speed, low for fast mode
; input 2 if for forward, high for run
; input 3 is for reverse, high for run
;
; output 0 is for step
; output 1 is for direction HIGH = FWD
;
;


START:
  INJPC 2 FWD           ; FORWARD?
  FORKI 3 REV START     ; REVERSE? NO, LOOP
  
FWD:
  INJPC 3 START         ; BOTH ARE DOWN, DO NOTHING
  SETBJMP  1 PULSEWAIT  ; FORWARD
  
REV:
  INJPC 2 START         ; BOTH ARE DOWN, DO NOTHING
  CLRBJMP  1 PULSEWAIT  ; REVERSE
  
PULSEWAIT:
  OUTV 0 1              ; PULSE STEP OUTPUT
  OUTV 0 0
  INJPC 1 START         ; SKIP NOPS FOR FAST MODE
  NOP                   ; DELAY
  NOP
  NOP
  NOP
  NOP
  NOP
  NOP
  NOP
  JMP START

  
