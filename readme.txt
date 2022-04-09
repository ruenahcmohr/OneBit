TEST input |  Write output | ( Jump flag set | Jump flag clear )


Label:

; comment

.NOP  
.HALT  

.SETB    o
.SETBJMP o, a

.CLRB    o
.CLRBJMP o, a

.CPYB    i, o
CPYBJPS i, o, a
CPYBJPC i, o, a
CPYBJMP i, o, a

.INVB    i, o
INVBJPS i, o, a
INVBJPC i, o, a
INVBJMP i, o, a

.JMP     a
.JPS     i a
.JPC     i a
.FORK    i, af, at

.WAITS   i
.WAITC   i

.SKIPC   i
.SKIPS   i

REPTC   i
REPTS   i




a: 8 bit address
i: 3 bit input
o: 3 bit output

RESET: 0x00


Instruction:
F false [ I, O ] [ jump ]
F true  [ I, O ] [ jump ]

-------------------------------------------------------------------------

NOP    
  test 0, set NC, jump A+1
  test 0, set NC, jump A+1

  No operation, machine is idle for 1 cycle.
  1 is written to the NC output
    
-------------------------------------------------------------------------

SETB o
  test 0, set O, jump A+1
  test 0, set O, jump A+1

  Set output bit o

-------------------------------------------------------------------------

SETBJMP o, a
  test 0, set O, jump a
  test 0, set O, jump a

  Set output bit o. Jump to a

-------------------------------------------------------------------------

CLRB o
  test 0, clear O, jump A+1
  test 0, clear O, jump A+1

  Clear output bit o

-------------------------------------------------------------------------

CLRBJMP o, a
  test 0, clear O, jump a
  test 0, clear O, jump a

  Clear output bit o and jump to a

-------------------------------------------------------------------------

CPYB i, o
  test i, clear o, jump A+1
  test i, set o,   jump A+1

  Copy input bit i to o

-------------------------------------------------------------------------

CPYBJPS i, o, a
  test i, clear o, jump A+1
  test i, set o,   jump a

  Copy input bit i to o, if it was set, jump to a

-------------------------------------------------------------------------

CPYBJPC i, o, a
  test i, clear o, jump a
  test i, set o,   jump A+1
  
  Copy input bit i to o, if it was clear, jump to a
  
-------------------------------------------------------------------------
  
CPYBJMP i, o, a
  test i, clear o, jump a
  test i, set o,   jump a
  
  Copy input bit i to o, jump to a
  
-------------------------------------------------------------------------  
  
INVB i, o
  test i, set o,   jump A+1
  test i, clear o, jump A+1

  Copy the inverted value of i to o

-------------------------------------------------------------------------

INVBJPS i, o, a
  test i, set o,   jump A+1
  test i, clear o, jump a
  
  Copy the inverted value of i to o, if i was set, jump to a

-------------------------------------------------------------------------
  
INVBJPC i, o, a
  test i, set o,   jump a
  test i, clear o, jump A+1 
  
  Copy the inverted value of i to o, if i was clear, jump to a

-------------------------------------------------------------------------
  
INVBJMP i, o, a
  test i, set o,   jump a
  test i, clear o, jump a
  
  Copy the inverted value of i to o, jump to a
  
-------------------------------------------------------------------------  

JMP a
  test 0, set NC, jump a
  test 0, set NC, jump a
  
  Jump to a
  1 is written to the NC output

-------------------------------------------------------------------------

JPS i a
  test i, set NC, jump A+1
  test i, set NC, jump a
  
  Jump to a if i is set
  1 is written to the NC output
  
-------------------------------------------------------------------------
  
JPC i a
  test i, set NC, jump a
  test i, set NC, jump A+1
  
  Jump to a if i is clear
  1 is written to the NC output

-------------------------------------------------------------------------

FORK i, af, at
  test i, set NC, jump af
  test i, set NC, jump at
  
  If i is clear, jump to af.
  If i is set,  jump to as.
  1 is written to the NC output

-------------------------------------------------------------------------

WAITS i
  test i, set NC, jump A
  test i, set NC, jump A+1
  
  Poll i untill it is set.
  1 is written to the NC output

-------------------------------------------------------------------------  
  
WAITC i
  test i, set NC, jump A+1
  test i, set NC, jump A  
  
  Poll i untill it is clear.
  1 is written to the NC output

-------------------------------------------------------------------------

SKIPC i
  test i, set NC, jump A+2
  test i, set NC, jump A+1
  
  Skip the next instruction if i is clear
  1 is written to the NC output

-------------------------------------------------------------------------

SKIPS i
  test i, set NC, jump A+1
  test i, set NC, jump A+2
  
  Skip the next instruction if i is set
  1 is written to the NC output

-------------------------------------------------------------------------

REPTC i
  test i, set NC, jump A-1
  test i, set NC, jump A+1
  
  Repeat the last instruction if i is clear
  1 is written to the NC output
  
-------------------------------------------------------------------------

REPTS i
  test i, set NC, jump A+1
  test i, set NC, jump A-1
  
  Repeat the last instruction if i is set
  1 is written to the NC output
  
  
-------------------------------------------------------------------------

HALT
  test 0, clear NC, jump A
  test 0, clear NC, jump A

  Halt machine.
  Reset is the only escape
  0 is written to the NC output (machine halted)
  
  


















































