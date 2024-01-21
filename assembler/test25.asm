;
;
; test CCBFS, CCBFC, CSBFS, CSBFC
; 
;  output 0 should follow input 0, output 1 should be the inverse
;
;  
;
START:
  IN 0
  CSBFS 0
  CSBFC 1
  CCBFC 0
  CCBFS 1
  JMP START
  
