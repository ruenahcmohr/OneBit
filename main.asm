; This is an example program for the 1 bit processor

START:
  SETB 1    ; set IO bit 1
  CLRB 1    ; clear IO bit 1
  JPS  6 PULSE
  SETB 2
  NOP
  CLRB 2 
  JMP  START
PULSE:
  SETB 3
  CLRB 3
  JMP START
