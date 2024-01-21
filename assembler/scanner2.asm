;
;
; larson scanner
;
; outputs should sequence, 7 will be funkey becasue its NC
;

START:

 SETB 0
 CLRB 1
 NOP
 SETB 1
 CLRB 0
 NOP
 SETB 2
 CLRB 1
 NOP
 SETB 3
 CLRB 2
 NOP
 SETB 4
 CLRB 3
 NOP
 SETB 5
 CLRB 4
 NOP
 SETB 6
 CLRB 5
 NOP 
 SETB 5
 CLRB 6
 NOP
 SETB 4
 CLRB 5
 NOP
 SETB 3
 CLRB 4
 NOP
 SETB 2
 CLRB 3
 NOP
 SETB 1
 CLRB 2
 JMP START
