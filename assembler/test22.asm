;
;
; test COUTVFS, COUTVFC
; 
;  output 0 should follow input 0, output 1 should be the inverse
;
;  Conditional Output Value, F Set/Clear
;
START:
  IN 0
  COUTVFS 0 1
  COUTVFS 1 0
  COUTVFC 0 0
  COUTVFC 1 1
  JMP START
