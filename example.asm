; This is an example program for the 1 bit processor

START:
  SETB 1    ; set IO bit 1
  CLRB 1    ; clear IO bit 1
  JMP START ; repeat
