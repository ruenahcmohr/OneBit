


Label:

; comment


--------------------------------------------
** By reading this you are liable for your own mental damage caused by trying to understand the
 repercussions of this instruction set. **


-- machine set

NOP             
HALT  

-- mostly io set

SETB     o       
SETBJMP  o, a  
CSETB    o, o  

CLRB     o       
CLRBJMP  o, a      
CCLRB    o, o

OUTIN    o, i
OUTINJMP o, i, a 
OUT      o
OUTJMP   o, a
COUTF    o, v, o, v

IN       i
INJMP    i, a
INJPS    i, a
INJPC    i, a
CINF     i, i

-- mostly flow control set

JMP      a     
JPS      a       
JPC      a      
 
FORKF    af, at
FORKI    i, af, at

WAITIS   i  
WAITIC   i  

SKIPIC   i
SKIPIS   i

SKIPFC   
SKIPFS  

REPTIC   i
REPTIS   i
 
REPTFC   
REPTFS   
                                                    


a: 8 bit address
i: 3 bit input
o: 3 bit output

RESET: 0x00


Machine Instruction:
  F false [ I, O ] [ jump ]
  F true  [ I, O ] [ jump ]

Order of operations:
  (latch input and outputs), jump
  Therefore an output cannot depend on an input done on the same instruction




-------------------------------------------------------------------------

NOP    
  test 7, set NC, jump A+1
  test 7, set NC, jump A+1

  No operation, machine is idle for 1 cycle.
  1 is written to the NC output
      
-------------------------------------------------------------------------

HALT
  test 7, clear NC, jump A
  test 7, clear NC, jump A

  Halt machine.
  Reset is the only escape
  0 is written to the NC output (machine halted)    
    
-------------------------------------------------------------------------

SETB o
  test 7, set O, jump A+1
  test 7, set O, jump A+1

  Set output bit o

-------------------------------------------------------------------------

SETBJMP o, a
  test 7, set O, jump a
  test 7, set O, jump a

  Set output bit o. Jump to a

-------------------------------------------------------------------------

CSETB of, ot
  test 7, set Of, jump A+1
  test 7, set Ot, jump A+1

  If F is clear, set bit Of
  If F is set,   set bit Ot

-------------------------------------------------------------------------

CLRB o
  test 7, clear O, jump A+1
  test 7, clear O, jump A+1

  Clear output bit o

-------------------------------------------------------------------------

CLRBJMP o, a
  test 7, clear O, jump a
  test 7, clear O, jump a

  Clear output bit o and jump to a

-------------------------------------------------------------------------

CCLRB of, ot
  test 7, clear of, jump A+1
  test 7, clear ot, jump A+1

  If F is clear, clear bit Of
  If F is set,   clear bit Ot  
  
-------------------------------------------------------------------------

OUTIN  o, i
  test i, set o, jump A+1
  test i, clear o, jump A+1
  
  write the value of the F register to output o
  read the value of i to the F register

-------------------------------------------------------------------------

OUTINJMP  o, i, a
  test i, set o, jump a
  test i, clear o, jump a
  
  write the value of the F register to output o
  read the value of i to the F register
  jump to a

-------------------------------------------------------------------------

OUT  o
  test 7, set o, jump A+1
  test 7, clear o, jump A+1
  
  write the value of the F register to output o
    
-------------------------------------------------------------------------

OUTJMP  o, a
  test 7, set o, jump a
  test 7, clear o, jump a
  
  write the value of the F register to output o, and jump to address  a

-------------------------------------------------------------------------

COUTF  of, vf, ot, vt
  test 7, set/clear of, jump A+1
  test 7, set/clear ot, jump A+1
  
  write vf to of if F is set
  write vt to ot if F is clear

-------------------------------------------------------------------------

IN  i
  test i, set NC, jump A+1
  test i, set NC, jump A+1
  
  load the value of i into the F register  

-------------------------------------------------------------------------

INJMP  i, a
  test i, set NC, jump a
  test i, set NC, jump a
    
  load the value of i into the F register and jump to address

-------------------------------------------------------------------------

INJPS  i, a
  test i, set NC, jump a
  test i, set NC, jump A+1
    
  load the value of i into the F register and jump to address a if it is set
    
-------------------------------------------------------------------------

INJPC  i, a
  test i, set NC, jump A+1
  test i, set NC, jump a
    
  load the value of i into the F register and jump to address a if it is clear
    
-------------------------------------------------------------------------
    
CINF if, it
  test if, set NC, jump A+1
  test it, set NC, jump A+1
    
  load the value of if into the F register if the F register is clear;
  else 
  load the value of it into the F register.

-------------------------------------------------------------------------  

JMP a
  test 7, set NC, jump a
  test 7, set NC, jump a
  
  Jump to a
  1 is written to the NC output
      
-------------------------------------------------------------------------

JPS a
  test 7, set NC, jump A+1
  test 7, set NC, jump a
  
  Jump to a if F is set
  1 is written to the NC output
  
-------------------------------------------------------------------------
  
JPC a
  test 7, set NC, jump a
  test 7, set NC, jump A+1
  
  Jump to a if F is clear
  1 is written to the NC output

-------------------------------------------------------------------------

FORKF af, at
  test 7, set NC, jump af
  test 7, set NC, jump at
  
  If F is clear, jump to af.
  If F is set,  jump to as.
  1 is written to the NC output

-------------------------------------------------------------------------

FORKI i, af, at
  test i, set NC, jump af
  test i, set NC, jump at
  
  copy value of i to F
  If F is clear, jump to af.
  If F is set,  jump to as.
  1 is written to the NC output


-------------------------------------------------------------------------

WAITIS i
  test i, set NC, jump A
  test i, set NC, jump A+1
  
  copy value of i to F
  Poll i untill it is set.
  1 is written to the NC output

-------------------------------------------------------------------------  
  
WAITIC i
  test i, set NC, jump A+1
  test i, set NC, jump A  
  
  copy value of i to F
  Poll i untill it is clear.
  1 is written to the NC output

-------------------------------------------------------------------------

SKIPIC i
  test i, set NC, jump A+2
  test i, set NC, jump A+1
  
  copy value of i to F
  Skip the next instruction if i is clear
  1 is written to the NC output

-------------------------------------------------------------------------

SKIPIS i
  test i, set NC, jump A+1
  test i, set NC, jump A+2
  
  copy value of i to F
  Skip the next instruction if i is set
  1 is written to the NC output

-------------------------------------------------------------------------

SKIPFC 
  test 7, set NC, jump A+2
  test 7, set NC, jump A+1
  
  Skip the next instruction if i is clear
  1 is written to the NC output

-------------------------------------------------------------------------

SKIPFS 
  test 7, set NC, jump A+1
  test 7, set NC, jump A+2
  
  Skip the next instruction if i is set
  1 is written to the NC output

-------------------------------------------------------------------------

REPTIC i
  test i, set NC, jump A-1
  test i, set NC, jump A+1
  
  copy value of i to F
  Repeat the last instruction if i is clear
  1 is written to the NC output
  
-------------------------------------------------------------------------

REPTIS i
  test i, set NC, jump A+1
  test i, set NC, jump A-1
  
  copy value of i to F
  Repeat the last instruction if i is set
  1 is written to the NC output
  
-------------------------------------------------------------------------

REPTFC 
  test 7, set NC, jump A-1
  test 7, set NC, jump A+1
  
  Repeat the last instruction if i is clear
  1 is written to the NC output
  
-------------------------------------------------------------------------

REPTFS 
  test 7, set NC, jump A+1
  test 7, set NC, jump A-1
  
  Repeat the last instruction if i is set
  1 is written to the NC output
    



















































