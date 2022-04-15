
;
;
; tests waitc FORKF FORKI 
;
;  if input 0 is set, system will wait for it to clear
;  otherwise output 0 will be a copy of input 1
;
;
START:
  IN 0
  FORKF A B
  HALT
  
C: ; input 1 was clear
  CLRB 0
  JMP START

D: ; input 1 was set
  SETB 0
  JMP START
  
A:  ; input 0 was clear
 FORKI 1 C D


B: ; input 0 was set
  WAITIC 0
  JMP START
