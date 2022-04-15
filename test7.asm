;
; tests, cinf outjmp
;
; input 0 selects active input
;  when input 0 is clear, input 1 controls output 0
;  when input 0 is ser,  input 2 controls output 0
;
;

START:
  IN 0
  CINF 1 2
  OUTJMP 0 START
