;
;
; test setb, clrb and jmp
;
; outputs should sequence, 7 will be funkey becasue its NC
;

 NOP
START:
 SETB 0
 SETB 1
 SETB 2
 SETB 3
 SETB 4
 SETB 5
 SETB 6
 SETB 7
 CLRB 0
 CLRB 1
 CLRB 2
 CLRB 3
 CLRB 4
 CLRB 5
 CLRB 6
 CLRB 7
 JMP START
