
;
;  sendserial.asm 
;
;
; read inputPort and send the value in serial as an ASCII character to TxDPort
;
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

 NOP            ; delay for next transmitt. 
 NOP
 NOP
 NOP
 NOP
 NOP            
 NOP
 NOP
 NOP
 NOP
 NOP
 NOP
 NOP
 NOP
 NOP            
 NOP
 NOP
 NOP
 NOP
 JMP START






