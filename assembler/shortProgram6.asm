
; And gate code.
Start:
 IN 0 ; read input 0 to F register
 JPC Clear ; if IN 0 was clear, clear output
 IN 1 ; read input 1 to F
 JPC Clear; if IN 1 was clear, clear output
SETB 2; set output 2
JMP Start ; do it again!
Clear:
CLRBJMP 2 Start ; do it again!
