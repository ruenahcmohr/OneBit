#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "ssls3.h"
#include "cmnCmdOptions2.h"

#define strCount(str) (sizeof(str)/sizeof(str[0]))

  #define SetBit(BIT, PORT)    (PORT |= (1<<BIT))
  #define ClearBit(BIT, PORT)  (PORT &= ~(1<<BIT))
  
// this can be used for bit remapping.
  #define GetBit(BIT,WORD,AS) ((((WORD) & (1<<(BIT)))!=0)?(AS):0)
  #define nGetBit(BIT,WORD,AS) ((((WORD) & (1<<(BIT)))!=0)?0:(AS))

#define RESET_B 15
#define IOEN_B  14
#define A0_B    13
#define A1_B    12
// nothing on   11
// nothing on   10
#define WR_B    9
#define MEMOE_B 8

void convertByte(uint8_t v, uint8_t latch) ;
void push(uint8_t v) ;
void setupParams(CLOSet_t ** this) ;
void SendBuff(void) ;
void send16(uint16_t v) ;
void send8(uint8_t v) ;
void writePulse() ;

FILE *Input;
char * InputFileName = NULL;
SSLS_t Port;
uint16_t V;

uint8_t TxBuff[257];
uint8_t BuffPtr;



#define ResetHigh()    SetBit(RESET_B, V)
#define ResetLow()     ClearBit(RESET_B, V)

#define IOEnable()     ClearBit(IOEN_B, V)
#define IODisable()    SetBit(IOEN_B, V)

#define A0High()       ClearBit(A0_B, V)
#define A0Low()        SetBit(A0_B, V)

#define A1High()       SetBit(A1_B, V)
#define A1Low()        ClearBit(A1_B, V)

#define MEMWRHigh()    SetBit(WR_B, V)
#define MEMWRLow()     ClearBit(WR_B, V)

#define MEMOEHigh()    SetBit(MEMOE_B, V)
#define MEMOELow()     ClearBit(MEMOE_B, V)

#define SetAddr(A)     V = ( (V & 0xCF00) | GetBit(1,A,(1<<A1_B)) | nGetBit(0,A,(1<<A0_B)) | ((A>>2) & 0xFF) ) 

//#define SetAddr(A)     V=(GetBit(1,A,(1<<A1_B))|nGetBit(0,A,(1<<A0_B))|((A>>2)&0xFF)) 

#define SetDBus(D)     V=(V&(0xFF00))|(D&0xFF)

int main( int argc, char ** argv) {
  int      i, j;
  uint8_t  b;
  uint16_t A;
  
  CLOSet_t * myCommands; 
  myCommands = NULL; 
  BuffPtr = 0;

  SerInit (&Port);

  V = 0xFFFF;

  setupParams(&myCommands); // define the arguments
  parseOptions(myCommands, argc-1, &argv[1]);   // then parse the command line
  
  if (Port.filename == NULL) {
      printf(" I need a port name \n");
      return -1;
  }
    
  if (SerOpen (&Port, Port.filename, 9600) != OK) {  //open text file 'param 1' w/ err chk 
      printf(" }:[ Unable to open %s. WHY!?\n", Port.filename);
      return -1;
  }   
    
  if (V != 0xFFFF) {
    send8(V);
    printf("Sent encoded 0x%02X\n", V);
    return 0;
  }  
    
  if ((Input = fopen(InputFileName, "rb")) == NULL) {  //open text file 'param 1' w/ err chk 
     printf("Unable to open %s for input.\n", InputFileName);
     return -1;
  }   
  
  SerWrite(&Port, (uint8_t[]){0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C}, 8);  // wake up!     
  printf("ACQUIRE-HALT\n"); fflush(stdout);
  A = 0;   V = 0;
  
  ResetHigh();  IODisable();  A0Low();  A1Low();  MEMWRHigh();  MEMOEHigh(); send16(V);  // set the system up for programming
  A0High(); send16(V);  // make sure higher address bits are latched
  A0Low();  send16(V);  
  
  printf("UPLOAD...\n"); fflush(stdout);
  
  while( (i = fgetc(Input)) != EOF ) {     
           
      SetAddr(A); A0Low(); send16(V);           // send address 
      A0High();  send16(V);  
      SetAddr(A);
	
      printf ("\r 0x%02X <- %02X ",A, i); fflush(stdout);
      
      b = i; SetDBus(b);
      writePulse(); // 2 writes.

      A++;                     
  }
  printf("\nRESET-RELEASE\n");
  SetAddr(0); A0High();  send16(V);   // latch address 0
  SetAddr(0); A0Low();   send16(V); 
  ResetLow(); send16(V); // end with a system reset
  
  printf("\a\a\n");
         
  SerFini (&Port);
  fclose(Input);
  
  return 0; 
}

// implicit - ok.
void send8(uint8_t v) {

    convertByte(v, 1);        
    SendBuff();
}

// checked - ok.
void send16(uint16_t v) {

    convertByte((v>>8), 0);          
    convertByte((v&0xFF), 1);        
    SendBuff();
}

// checked - ok.
void writePulse() {

   MEMWRLow();  send16(V);
   MEMWRHigh(); send16(V);

  
  // new method, we allow it to follow whatever it wants and here, we stop it, the address will be set on the
  // next write and at the same time it will start listening again.
  
 //  MEMWRHigh();
 //  send16(V);
  // MEMWRLow();   
   
   
}




/*
  Each set of 2 bits is encoded into a byte
  sending 0x00 latches the data in.
    
  The MSB needs to be sent first
  And were sending two bits at a time  
*/
void convertByte(uint8_t v, uint8_t latch) {
  int i;
  uint8_t s, t;
 
  for ( i = 0; i < 4; i++) {

    s = (v >> (2*(3-i))) & 0x03;    

    switch(s) {  // yes, use a table if you want...      
      case 0:    push(0x8C);    break;
      case 1:    push(0xF3);    break;                  
      case 2:    push(0x3F);    break;
      case 3:    push(0xEF);    break;
    }
  }
  
  if (latch != 0) { push(0x00); }
  
}



void push(uint8_t v) {

  TxBuff[BuffPtr] = v;  BuffPtr++;
  
}

void SendBuff() {

  SerWrite(&Port, TxBuff, BuffPtr);
  BuffPtr = 0;
}



int cmdHelp(char * unused) {
  printf( "  --=== 1-bit program uploader ===--\n\n");
  printf( "-h,--help   Your looking at it.\n");
  printf( "-p          Serial port to connect to\n");
  printf( "-v          Value to send to port, dont use with -f\n");
  printf( "-f          File to send to port, dont use with -v\n");
    
  printf("\n");
  return 0;
}



int cmdPort(char * s) {
  
  Port.filename = s;    
    
  printf("Port is: %s\n", s);
  return 1;
}

int cmdValue(char * s) {
  
  V = strtod(s, NULL);
  
  printf("Value is: 0x%04X\n", V);
  
  return 1;
}

int cmdFile(char *s) {

  InputFileName = s;        
  printf("File to send is: %s\n", InputFileName);

  return 1;
}


void setupParams(CLOSet_t ** this) {

  // this is outright ugly, but we have to because of limits to C
  static char* optsHelp[]    = {"-h","--help"};
  static char* optsPort[]    = {"-p"};
  static char* optsValue[]   = {"-v"};
  static char* optsFile[]    = {"-f"};
 

  // first init 
  cmdLineOptionInit(this);
  
  // then add your options (note I ignore any errors (BAD! BAD!))
  addCmdLineOption(*this, optsHelp, strCount(optsHelp), cmdHelp);
  addCmdLineOption(*this, optsPort, strCount(optsPort), cmdPort);
  addCmdLineOption(*this, optsValue, strCount(optsValue), cmdValue);
  addCmdLineOption(*this, optsFile, strCount(optsFile), cmdFile);

}

// ok NOW can I add the ALU to if???























































