#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "ssls3.h"
#include "cmnCmdOptions2.h"

#define strCount(str) (sizeof(str)/sizeof(str[0]))

void convertByte(uint8_t v, uint8_t latch) ;
void push(uint8_t v) ;
void setupParams(CLOSet_t ** this) ;
void SendBuff(void) ;
void send16(uint16_t v) ;
void writePulse() ;

FILE *Input;
char * InputFileName = NULL;
SSLS_t Port;
uint16_t V;

uint8_t TxBuff[257];
uint8_t BuffPtr;


#define InitV()        V=   0xE300
#define ResetHigh()    V|=  0x8000
#define ResetLow()     V&=(~0x8000)
#define IOEnable()     V|=  0x4000
#define IODisable()    V&=(~0x4000)

// A0 is inverted before it gets to the ROM
// which makes the high address latch trigger on falling edge.
#define A0Low()        V|=  0x2000
#define A0High()       V&=(~0x2000)

#define A1High()       V|=  0x1000
#define A1Low()        V&=(~0x1000)

#define MEMWRHigh()    V|=  0x0200
#define MEMWRLow()     V&=(~0x0200)
#define MEMOEHigh()    V|=  0x0100
#define MEMOELow()     V&=(~0x0100)

#define SetAddr(A)     V=((V&(0xCF00))|((0x0001&A)<<13)|((0x0002&A)<<11)|((A>>2)&0xFF))^0x2000
#define SetDBus(D)     V=(V&(0xFF00))|(D&0xFF)

int main( int argc, char ** argv) {
  int      i, j;
  uint8_t  b;
  uint16_t A;
  
  CLOSet_t * myCommands; 
  myCommands = NULL; 
  BuffPtr = 0;

  SerInit (&Port);

  setupParams(&myCommands); // define the arguments
  parseOptions(myCommands, argc-1, &argv[1]);   // then parse the command line
  

  if (SerOpen (&Port, Port.filename, 9600) != OK) {  //open text file 'param 1' w/ err chk 
      printf(" }:[ Unable to open %s. WHY!?\n", Port.filename);
      return -1;
  }   
    
  if ((Input = fopen(InputFileName, "rb")) == NULL) {  //open text file 'param 1' w/ err chk 
     printf("Unable to open %s for input.\n", InputFileName);
     return -1;
  }   
  
  SerWrite(&Port, (uint8_t[]){0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C}, 8);  // wake up!     
  printf("ACQUIRE-HALT\n"); fflush(stdout);
  InitV();  send16(V);  // set the system up for programming
  A0High(); send16(V);  // make sure higher address bits are latched
  A0Low();  send16(V);  
  printf("UPLOAD\n"); fflush(stdout);
  
  A = 0;
  while( (i = fgetc(Input)) != EOF ) {       
    b = i;
    printf ("\r 0x%02X  ",A); fflush(stdout);
   
    SetDBus(b);
    writePulse(); // 2 writes.
     
    A++;
    
    if (A & 1) {                       // if A0 just went high                
      SetAddr(A); send16(V);           // send address      
    } else {                           // else if A0 is going low we have to latch the upper addreses
      SetAddr(A); A0High(); send16(V); 
      SetAddr(A); A0Low();  send16(V);       
    }        
           
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


void send16(uint16_t v) {

    convertByte((v>>8), 0);          // 
    convertByte((v&0xFF), 1);        // 
    SendBuff();
}


void writePulse() {

   MEMWRLow();
   send16(V);
   MEMWRHigh();
   send16(V);
   
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























































