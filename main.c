

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "varlib.h"



/*

- initialize program memory with nops
- write instruction values to program

- enable address setting via label format.
- allow instructions on same line as labels.

*/




/*
One bit processor - Rues assembler

NOP    
SETB       o
SETBJMP    o, a
CLRB       o
CLRBJMP    o, a
CPYB    i, o
CPYBJPS i, o, a
CPYBJPC i, o, a
CPYBJMP i, o, a
INVB    i, o
INVBJPS i, o, a
INVBJPC i, o, a
INVBJMP i, o, a
JMP           a
JPS     i     a
JPC     i     a
FORK    i, af, at
WAITS   i
WAITC   i
HALT
SKIPC   i
SKIPS   i
REPTC   i
REPTS   i

*/

// due to other filtering, a semicolon will never be part of the string, so we can use that.
// search for ;NMEOMONIC;
char  * MNEMONICS = ";NOP;SETB;SETBJMP;CLRB;CLRBJMP;CPYB;CPYBJPS;CPYBJPC;CPYBJMP;INVB;INVBJPS;INVBJPC;INVBJMP;JMP;JPS;JPC;FORK;WAITS;WAITC;HALT;SKIPC;SKIPS;REPTC;REPTS;";
int  ParamCount[] = {  0,  1,    2,      1,   2,      2,    3,      3,       3,    2,   3,       3,      3,    1,  2,  2,   3,   1,    1,    0,   1,    1,   1,     1 };
enum InstIdx { NOP = 0,SETB,SETBJMP,CLRB,CLRBJMP,CPYB,CPYBJPS,CPYBJPC,CPYBJMP,INVB,INVBJPS,INVBJPC,INVBJMP,JMP,JPS,JPC,FORK,WAITS,WAITC,HALT,SKIPC,SKIPS,REPTC,REPTS };


int countOff(char *s, unsigned int v) ;
int str2Inst(char * s) ;
int buildInstruction(char ** strings, uint8_t address) ;
int OpAssemble(uint8_t input, uint8_t output, uint8_t D );

#define I_NULL 7
#define O_NULL 7

typedef struct opcode_s {  
      uint8_t FInstruction;
      uint8_t FAddress;
      uint8_t TInstruction;
      uint8_t TAddress;
} opcode_t;


opcode_t  Program[256];
varlist_t labels;
FILE      *output;
unsigned int LineNum;  

int main(void) {
  FILE *input;

  
  char *line = NULL;
  int lineSize;
  char *p;
  
  char *tokLine = NULL;
  char *arg[6];
  
  unsigned int i;
  unsigned int address;
  

  
  
  //open files
  if ((input = fopen("main.asm", "rt")) == NULL) {  //open text file 'param 1' w/ err chk 
      printf("Unable to open main.asm for input.\n");
      return -1;
  }
  if ((output = fopen("output.bin", "wb")) == NULL) {  //open text file 'param 1' w/ err chk 
      printf("Unable to open output.bin for input.\n");
      return -1;
  }
  initVars(&labels);
  
  // the first pass gathers all the labels
  
  address = 0;
  LineNum = 0;
  while (getline(&line, &lineSize, input) != -1) {
    LineNum++;
    
    for(p = line; *p; p++) { // this will get more complex...
      if (*p == ';')  { *p = 0;  break; } 
      if (*p == '\n') { *p = 0;  break; } 
      if (*p == '\r') { *p = 0;  break; } 
    }
    
    tokLine = strtok(line, " ");
    if (tokLine) {
        
      if (0) {
      } else if (tokLine[strlen(tokLine)-1] == ':') {
        tokLine[strlen(tokLine)-1] = '\0';
        if (strlen(tokLine) != 0) {
          setVar(&labels, tokLine, address, 0);
        } else {
          printf("WTF Empty label ??\n" );
        }
      } else {
        if ((++address) == 0) printf( "PROGRAM OVERFLOW\n");
      }
    }
  }
  
  dumpVars(&labels);
  rewind(input);
    
  address = 0;
  LineNum = 0;
  // the second pass builds the program
  while (getline(&line, &lineSize, input) != -1) {
    LineNum++;
    for(p = line; *p; p++) { // this will get more complex...
      if (*p == ';')  { *p = 0;  break; } 
      if (*p == '\n') { *p = 0;  break; } 
      if (*p == '\r') { *p = 0;  break; } 
    }
    
    tokLine = strtok(line, " ");
    if (tokLine) {
        
      if (0) {
      } else if (tokLine[strlen(tokLine)-1] == ':') {
      } else {
    
        for(i = 0; tokLine && (i<5); i++) {
         arg[i] = strdup(tokLine);
         tokLine = strtok(NULL, " "); 
        }
        if (i == 5) printf ("More than 5 params on an instruction? you insane dog, what are you up to?, Line %d\n", LineNum);        
        arg[i] = NULL;
        
        buildInstruction(arg, address); 

        address++;

      }
      // next line here
    }
   
  }
  

  finiVars(&labels);  
  fclose(input);
  fclose(output);  
  
  
  return 0;
}



//  int r=0; while (*s) ';'==*s++ && r++; return r;
// OR sadistic recursion!
int countOff(char *s, unsigned int v) {
  char *n;
  if ((n = strchr(s, ';')) != NULL) { return countOff(n+1, v+1); }
  return v;
}




// convert a string to an instruction number
int str2Inst(char * s) {
  char * offset;
  char * p;
  
  asprintf(&p, ";%s;", s);    // pad token for search, this ensures no partial tokens are found. 
    
  if ((offset = strcasestr(MNEMONICS, p)) == NULL) {
    free(p);
    return -1;
  } else {   
    free(p);         
    return  25-countOff(offset, 0);    
  }

}



int buildInstruction(char ** strings, uint8_t address) {

 opcode_t OP;
 int i;
 double vd;
 uint8_t v8[5];
 int nmNumber;
 int argCount;
 int t;
 
 
 OP.FInstruction = 0;
 OP.TInstruction = 0;
 OP.FAddress     = 0;
 OP.TAddress     = 0;
 
 printf("0x%02X: ", address);
 
 
  if ((nmNumber = str2Inst(strings[0])) == -1) {
    printf("Unknown mnemonic %s on line %d\n", strings[0], LineNum);
  } 
  
  argCount = 0;
  for( i = 1; strings[i] && (i<5); i++) {
   if (!evalVar(&labels, strings[i], &vd)) {
     v8[i] = vd;
   } else {
     v8[i] = strtol(strings[i], NULL, 0);
   }     
   argCount++;   
  }    
      
  printf("%s ",  strings[0] );
  for (i = 0; i < argCount; i++) printf(" 0x%02X ", v8[i+1]);      
  if (argCount != ParamCount[nmNumber]) printf("Wrong number of arguments, Line %d\n", LineNum);


  switch(nmNumber) {
    case NOP:
      if ((t = OpAssemble( I_NULL , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address+1;
      OP.TAddress = OP.FAddress;
      
    break;

    case SETB:
      if ((t = OpAssemble( I_NULL , v8[1], 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address+1;
      OP.TAddress = OP.FAddress;      
            
    break;

    case SETBJMP:
      if ((t = OpAssemble( I_NULL , v8[1], 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = v8[2];
      OP.TAddress = OP.FAddress;   
    break;

    case CLRB:
      if ((t = OpAssemble( I_NULL , v8[1], 0 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address+1;
      OP.TAddress = OP.FAddress;   
    break;

    case CLRBJMP:
      if ((t = OpAssemble( I_NULL , v8[1], 0 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = v8[2];
      OP.TAddress = OP.FAddress; 
    break;

    case CPYB:
      if ((t = OpAssemble( v8[1] , v8[2], 0 )) < 0 ) return -1;      
      OP.FInstruction = t;
      
      if ((t = OpAssemble( v8[1] , v8[2], 1 )) < 0 ) return -1;
      OP.TInstruction = t;
      
      OP.FAddress = address+1;
      OP.TAddress = OP.FAddress;  
    break;

    case CPYBJPS:
      if ((t = OpAssemble( v8[1] , v8[2], 0 )) < 0 ) return -1;      
      OP.FInstruction = t;
      
      if ((t = OpAssemble( v8[1] , v8[2], 1 )) < 0 ) return -1;
      OP.TInstruction = t;
      
      OP.FAddress = address+1;
      OP.TAddress = v8[3];  
    break;

    case CPYBJPC:
      if ((t = OpAssemble( v8[1] , v8[2], 0 )) < 0 ) return -1;      
      OP.FInstruction = t;
      
      if ((t = OpAssemble( v8[1] , v8[2], 1 )) < 0 ) return -1;
      OP.TInstruction = t;

      OP.FAddress = v8[3];        
      OP.TAddress = address+1;
    break;

    case CPYBJMP:
      if ((t = OpAssemble( v8[1] , v8[2], 0 )) < 0 ) return -1;      
      OP.FInstruction = t;
      
      if ((t = OpAssemble( v8[1] , v8[2], 1 )) < 0 ) return -1;
      OP.TInstruction = t;
      
      OP.FAddress = v8[3];
      OP.TAddress = OP.FAddress;   
    break;

    case INVB:
      if ((t = OpAssemble( v8[1] , v8[2], 1 )) < 0 ) return -1;      
      OP.FInstruction = t;
      
      if ((t = OpAssemble( v8[1] , v8[2], 0 )) < 0 ) return -1;
      OP.TInstruction = t;
      
      OP.FAddress = address+1;
      OP.TAddress = OP.FAddress;  
    break;

    case INVBJPS:
      if ((t = OpAssemble( v8[1] , v8[2], 1 )) < 0 ) return -1;      
      OP.FInstruction = t;
      
      if ((t = OpAssemble( v8[1] , v8[2], 0 )) < 0 ) return -1;
      OP.TInstruction = t;
      
      OP.FAddress = address+1;
      OP.TAddress = v8[3];  
    break;

    case INVBJPC:
      if ((t = OpAssemble( v8[1] , v8[2], 1 )) < 0 ) return -1;      
      OP.FInstruction = t;
      
      if ((t = OpAssemble( v8[1] , v8[2], 0 )) < 0 ) return -1;
      OP.TInstruction = t;
      
      OP.FAddress = v8[3];
      OP.TAddress = address+1;  
    break;

    case INVBJMP:
      if ((t = OpAssemble( v8[1] , v8[2], 1 )) < 0 ) return -1;      
      OP.FInstruction = t;
      
      if ((t = OpAssemble( v8[1] , v8[2], 0 )) < 0 ) return -1;
      OP.TInstruction = t;
      
      OP.FAddress = v8[3];
      OP.TAddress = OP.FAddress;  
    break;

    case JMP:
      if ((t = OpAssemble( I_NULL , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = v8[1];
      OP.TAddress = OP.FAddress;  
    break;

    case JPS:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address+1;
      OP.TAddress = v8[2]; 
    break;

    case JPC:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = v8[2]; 
      OP.TAddress = address+1;
    break;

    case FORK:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = v8[2]; 
      OP.TAddress = v8[3];
    break;

    case WAITS:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address;
      OP.TAddress = address+1; 
    break;

    case WAITC:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address+1;
      OP.TAddress = address; 
    break;

    case HALT:      
      if ((t = OpAssemble( I_NULL , O_NULL, 0 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address-1;
      OP.TAddress = OP.FAddress;  
    break;

    case SKIPC:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address+2;
      OP.TAddress = address+1; 
    break;

    case SKIPS:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address+1;
      OP.TAddress = address+2; 
    break;

    case REPTC:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address-1;
      OP.TAddress = address+1; 
    break;

    case REPTS:

      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP.FInstruction = t;
      OP.TInstruction = OP.FInstruction;
      OP.FAddress = address+1;
      OP.TAddress = address-1; 
    break;
  
  }
    printf(" %02X-%02X-%02X-%02X ", OP.FInstruction, OP.FAddress , OP.TInstruction , OP.TAddress );
    
    fputc(OP.FInstruction, output);
    fputc(OP.FAddress, output);
    fputc(OP.TInstruction, output);
    fputc(OP.TAddress, output);    
    
    printf("\n");        
          
          

  
  for( i = 0; strings[i]; i++) {   free(strings[i]); strings[i] = NULL;  }

}




int OpAssemble(uint8_t input, uint8_t output, uint8_t D ) {

  if ( input > 7 ) { printf("Input value out of range (0-7), Line %d\n", LineNum);  return -1;}
  if ( output > 7) { printf("Output value out of range (0-7), Line %d\n", LineNum); return -2;}
  
  return (input | (output << 3) | (((D==0)?0:1) << 6));

}








