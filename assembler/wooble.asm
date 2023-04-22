;
;
; inverted larson scanner
;
; outputs should sequence, 7 will be funkey becasue its NC
;

 NOP
START:
 SETB 3
 SETB 4
 NOP
 SETB 2
 SETB 5
 NOP 
 SETB 1
 SETB 6 
 NOP 
 SETB 0
 SETB 7  
 NOP
 NOP
 NOP
 NOP  
 CLRB 0
 CLRB 7
 NOP
 CLRB 1
 CLRB 6
 NOP 
 CLRB 2
 CLRB 5
 NOP 
 CLRB 3
 CLRB 4
 NOP   
 JMP START
