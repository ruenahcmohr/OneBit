
;
;  sendserial.asm 
;
;
; read inputPort and send the value in serial as an ASCII character to TxDPort
; wait for the value to change before resending
;
;

inputPort EQU 0
TxDPort   EQU 0

START:
 IN inputPort
 
 OUTV TxDPort 0 ; start bit
 OUT  TxDPort   ; data bit 0
 OUTV TxDPort 0 ; data bit 1
 NOP            ; data bit 2
 NOP            ; data bit 3
 OUTV TxDPort 1 ; data bit 4
 NOP            ; data bit 5
 OUTV TxDPort 0 ; data bit 6
 NOP            ; data bit 7
 OUTV TxDPort 1 ; stop bit

 FORKF WaitHigh WaitLow ; its silly to use fork for this, but it makes reading it clearer
 
WaitHigh:   ; if the value is low, wait for a high
; read inputPort, if its low jump to WaitHigh, if its high jump to START
 FORKI     inputPort WaitHigh START 

WaitLow:    ; if the value is high, wait for a low
 FORKI     inputPort START WaitLow






