; A input
A EQU 0
; B input
B EQU 1
; Carry input
C EQU 2

; Y output
Y EQU 0
; CO output
CO EQU 1


Start: 
 FORKI C C0 C1
C0: 
 FORKI A C0A0 C0A1
 
C1: 
 FORKI A C1A0 C1A1 
 
C0A0: 
 FORKI B ZeroResult OneResult
 
C0A1: 
C1A0:
 FORKI B OneResult TwoResult

C1A1:
 FORKI B TwoRestult ThreeResult
  
 
ZeroResult:
 OUTV    Y  0
 OUTVJMP CO 0 Stop
  
OneResult:
 OUTV    Y  1
 OUTVJMP CO 0 Stop  

TwoResult:
 OUTV    Y  0
 OUTVJMP CO 1 Stop
 
ThreeResult:
 OUTV    Y  1
 OUTVJMP CO 1 Stop
  
Stop: 
 HALT
