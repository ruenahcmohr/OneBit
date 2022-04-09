START:
  NOP
  CPYB 0 0   ; output 0 is a copy of input 0
  INVB 0 1   ; output 1 is an invert of input 0

A:
  JPS  1 TWO ; output 2 is a copy of input 1
  CLRB 2
  JMP THREE
TWO:
  SETB 2
THREE:

  JPC  2 FOUR ; output 3 is a copy of input 2
  SETBJMP 3 FIVE
FOUR:
  CLRBJMP 3 FIVE
FIVE:

 SKIPC  3 ; if input 3 is set, system will halt
 HALT
 
 SKIPS  4 ; if input 4 is clear, system will wait for input 0 to be clear
 WAITC  0

 WAITS  5 ; program will wait for input 5 to be set
  
 

 FORK 6 START A ; input 6 determines if the loop copies input 0 or leaves it.
  
