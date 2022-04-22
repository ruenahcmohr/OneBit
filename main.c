

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "varlib.h"


/*


- enable address setting via label format.
- create macro for naming constants   .equ 
- allow instructions on same line as labels.
- input and output file names

*/




/*
One bit processor - Rues assembler


NOP    
HALT
SETB      o
SETBJMP   o,  a
CSETB     of, ot
CLRB      o
CLRBJMP   o,  a 
CCLRB     of, ot
OUTIN     o,  i
OUTINJMP  o,  i, a
OUTV      o, v
OUTVJMP   o, v, a
OUT       o
OUTI      o
OUTIJMP   o,  a
OUTJMP    o,  a
COUTF     of, vf, ot, vt
IN        i
INJMP     i,  a
INJPS     i,  a
INJPC     i,  a
CINF      if, it
JMP       a
JPS       a
JPC       a
FORKF     af, at
FORKI     i,  af, at
WAITIS    i
WAITIC    i
SKIPIC    i
SKIPIS    i
SKIPFC 
SKIPFS 
REPTIC    i
REPTIS    i
REPTFC 
REPTFS 

Tips:
  - there are no actual commas in the source
  - dont put instructions on the same line as labels

*/

#define I_NULL 7
#define O_NULL 7

#define INSTCOUNT 38
// due to other filtering, a semicolon will never be part of the string, so we can use that.
// search for ;NMEOMONIC;
char  * MNEMONICS = ";NOP;HALT;SETB;SETBJMP;CSETB;CLRB;CLRBJMP;CCLRB;OUTIN;OUTINJMP;OUTV;OUTVJMP;OUT;OUTI;OUTIJMP;OUTJMP;COUTF;IN;INJMP;INJPS;INJPC;CINF;JMP;JPS;JPC;FORKF;FORKI;WAITIS;WAITIC;SKIPIC;SKIPIS;SKIPFC;SKIPFS;REPTIC;REPTIS;REPTFC;REPTFS;";
int  ParamCount[] = {  0,  0,   1,     2,     2,    1,    2,      2,    2,     3,     2,    3,    1,   1,  2,       2,     4,   1,   2,   2,    2,    2,  1,   1,  1,  2,    3,     1,    1,      1,     1,     0,     0,    1,      1,     0,     0  };
enum InstIdx { NOP = 0,HALT,SETB,SETBJMP,CSETB,CLRB,CLRBJMP,CCLRB,OUTIN,OUTINJMP,OUTV,OUTVJMP,OUT,OUTI,OUTIJMP,OUTJMP,COUTF,IN,INJMP,INJPS,INJPC,CINF,JMP,JPS,JPC,FORKF,FORKI,WAITIS,WAITIC,SKIPIC,SKIPIS,SKIPFC,SKIPFS,REPTIC,REPTIS,REPTFC,REPTFS };


typedef struct opcode_s {  
      uint8_t FInstruction;
      uint8_t FAddress;
      uint8_t TInstruction;
      uint8_t TAddress;
} opcode_t;


int countOff(char *s, unsigned int v) ;
int str2Inst(char * s) ;
int buildInstruction(opcode_t *OP, char ** strings, uint8_t address, varlist_t *llabels) ;
int OpAssemble(uint8_t input, uint8_t output, uint8_t D );
int assemble(FILE *input, FILE *out, varlist_t *llabels );


unsigned int LineNum;  



int main(int argc, char** argv, char *arge[]) {

  FILE *input;
  FILE      *output;
  varlist_t labels;

  initVars(&labels);
  
  if (argc < 2) {                          // do we have input paramiters??   
    puts("Need input filename\n");
    return 0;    
  }
 
  //open files
  if ((input = fopen(argv[1], "rt")) == NULL) {  //open text file 'param 1' w/ err chk 
      printf("Unable to open %s for input.\n", argv[1]);
      return -1;
  }
  if ((output = fopen("output.bin", "wb")) == NULL) {  //open text file 'param 1' w/ err chk 
      printf("Unable to open output.bin for input.\n");
      return -1;
  }
  
  assemble(input, output, &labels ) ;

  finiVars(&labels);  
  fclose(input);
  fclose(output);    
  
  return 0;
}



// turn the details into an binary instruction
int assemble(FILE *input, FILE *out, varlist_t *llabels ) {
  
  char *line = NULL;
  int lineSize;
  char *p;
  
  char *tokLine = NULL;
  char *arg[6];
  
  unsigned int i;
  unsigned int address;
  
  opcode_t inst;
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
          setVar(llabels, tokLine, address, 0);
        } else {
          printf("WTF Empty label ??\n" );
        }
      } else {
        if ((++address) == 0) printf( "PROGRAM OVERFLOW\n");
      }
    }
  }
  
  dumpVars(llabels);
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
    
        for(i = 0; tokLine && (i<6); i++) {
         arg[i] = strdup(tokLine);
         tokLine = strtok(NULL, " "); 
        }
        if (i == 6) printf ("More than 5 params on an instruction? you insane dog, what are you up to?, Line %d\n", LineNum);        
        arg[i] = NULL;
        
        if (buildInstruction(&inst, arg, address, llabels) > 0) {
        
          fputc(inst.FInstruction, out);
          fputc(inst.FAddress, out);
          fputc(inst.TInstruction, out);
          fputc(inst.TAddress, out);  
        
          address++;
        }
      }
      // next line here
    }
   
  }
  


}

/*
    This counts the number of semicolons to the end of the string
*/
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
//    printf(">>%d<<", countOff(offset, 0));      
    return  INSTCOUNT-countOff(offset, 0);    
  }

}



int buildInstruction(opcode_t *OP, char ** strings, uint8_t address, varlist_t *llabels) {

// opcode_t OP;
 int i;
 double vd;
 uint8_t v8[5];
 int nmNumber;
 int argCount;
 int t;
 
 
 OP->FInstruction = 0;
 OP->TInstruction = 0;
 OP->FAddress     = 0;
 OP->TAddress     = 0;
 

  
  argCount = 0;
  for( i = 1; strings[i] && (i<5); i++) {
   if (!evalVar(llabels, strings[i], &vd)) {
     v8[i] = vd;
   } else {
     v8[i] = strtol(strings[i], NULL, 0);
   }     
   argCount++;   
  }    
  
  if (argCount == 0) return -1;
  
  if ((nmNumber = str2Inst(strings[0])) == -1) {
  // check for equ here
    if (argCount > 1) {
      if (strcasecmp("EQU", strings[1]) == 0) {
        if (argCount == 2) { setVar(llabels, strings[0], v8[2], 0); }
	else {               delVar(llabels, strings[0]); }
	return 0;
      }
    }
  
    printf("Unknown mnemonic %s on line %d\n", strings[0], LineNum);
    return -1;
  }   
        
   
  if (argCount != ParamCount[nmNumber]) printf("Wrong number of arguments, Line %d\n", LineNum);

 printf("0x%02X: ", address);

  // default NOP
  OP->FInstruction = OpAssemble( I_NULL , O_NULL, 1 );
  OP->TInstruction = OP->FInstruction;
  OP->FAddress = address+1;             // default to next address
  OP->TAddress = OP->FAddress;
  
  switch(nmNumber) {

    case NOP:
    break;

    case HALT:      
      if ((t = OpAssemble( I_NULL , O_NULL, 0 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->FAddress = address;
      OP->TAddress = OP->FAddress;  
    break;

    case SETB:
      if ((t = OpAssemble( I_NULL , v8[1], 1 )) < 0 ) return -1;      
      
      OP->FInstruction = t;            
      OP->TInstruction = OP->FInstruction;                  
    break;

    case SETBJMP:
      if ((t = OpAssemble( I_NULL , v8[1], 1 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->FAddress = v8[2];
      OP->TAddress = OP->FAddress;   
    break;

    case CSETB:
      if ((t = OpAssemble( I_NULL , v8[1], 1 )) < 0 ) return -1;
      OP->TInstruction = t;  
       
      if ((t = OpAssemble( I_NULL , v8[2], 1 )) < 0 ) return -1;
      OP->FInstruction = t;                           
    break;

    case CLRB:
      if ((t = OpAssemble( I_NULL , v8[1], 0 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
    break;

    case CLRBJMP:
      if ((t = OpAssemble( I_NULL , v8[1], 0 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->FAddress = v8[2];
      OP->TAddress = OP->FAddress; 
    break;

    case CCLRB:
      if ((t = OpAssemble( I_NULL , v8[1], 0 )) < 0 ) return -1;
      OP->TInstruction = t;   
      
      if ((t = OpAssemble( I_NULL , v8[2], 0 )) < 0 ) return -1;
      OP->FInstruction = t;                     
    break;

    case OUTIN:
      if ((t = OpAssemble( v8[2] , v8[1], 0 )) < 0 ) return -1;      
      OP->FInstruction = t;
      
      if ((t = OpAssemble( v8[2] , v8[1], 1 )) < 0 ) return -1;
      OP->TInstruction = t;      
    break;

    case OUTINJMP:
      if ((t = OpAssemble( v8[2] , v8[1], 0 )) < 0 ) return -1;      
      OP->FInstruction = t;
      
      if ((t = OpAssemble( v8[2] , v8[1], 1 )) < 0 ) return -1;
      OP->TInstruction = t;     
      OP->FAddress = v8[3];
      OP->TAddress = OP->FAddress;
    break;

    case OUTV:
      if ((t = OpAssemble( I_NULL , v8[1], v8[2] )) < 0 ) return -1;      
      OP->FInstruction = OP->TInstruction = t;      
    break;
    
    case OUTVJMP:
      if ((t = OpAssemble( I_NULL , v8[1], v8[2] )) < 0 ) return -1;      
      OP->FInstruction = OP->TInstruction = t;   
      OP->FAddress = OP->TAddress = v8[3];         
    break;    

    case OUT:
      if ((t = OpAssemble( I_NULL , v8[1], 0 )) < 0 ) return -1;      
      OP->FInstruction = t;
      
      if ((t = OpAssemble( I_NULL , v8[1], 1 )) < 0 ) return -1;
      OP->TInstruction = t;      
    break;
    
    case OUTI:
      if ((t = OpAssemble( I_NULL , v8[1], 1 )) < 0 ) return -1;      
      OP->FInstruction = t;
      
      if ((t = OpAssemble( I_NULL , v8[1], 0 )) < 0 ) return -1;
      OP->TInstruction = t;      
    break;

    case OUTJMP:
      if ((t = OpAssemble( I_NULL , v8[1], 0 )) < 0 ) return -1;      
      OP->FInstruction = t;
      
      if ((t = OpAssemble( I_NULL , v8[1], 1 )) < 0 ) return -1;
      OP->TInstruction = t;    
      
      OP->FAddress = v8[2];
      OP->TAddress = OP->FAddress;  
    break;
    
    case OUTIJMP:
      if ((t = OpAssemble( I_NULL , v8[1], 1 )) < 0 ) return -1;      
      OP->FInstruction = t;
      
      if ((t = OpAssemble( I_NULL , v8[1], 0 )) < 0 ) return -1;
      OP->TInstruction = t;    
      
      OP->FAddress = v8[2];
      OP->TAddress = OP->FAddress;  
    break;    

    case COUTF: // COUTF  of, vf, ot, vt
     if ((t = OpAssemble( I_NULL , v8[1], v8[2] )) < 0 ) return -1;
     OP->FInstruction = t;
     
     if ((t = OpAssemble( I_NULL , v8[3], v8[4] )) < 0 ) return -1;
     OP->TInstruction = t;       
    break;

    case IN:
     if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
     OP->FInstruction = t;
     OP->TInstruction = OP->FInstruction;
    break;

    case INJMP:
     if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
     OP->FInstruction = t;
     OP->TInstruction = OP->FInstruction;
     OP->FAddress = v8[2];
     OP->TAddress = OP->FAddress; 
    break;

    case INJPS:
     if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
     OP->FInstruction = t;
     OP->TInstruction = OP->FInstruction;
     OP->TAddress     = v8[2];
    break;

    case INJPC:
     if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
     OP->FInstruction = t;
     OP->TInstruction = OP->FInstruction;
     OP->FAddress     = v8[2];
    break;

    case CINF:
     if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
     OP->FInstruction = t;
     
     if ((t = OpAssemble( v8[2] , O_NULL, 1 )) < 0 ) return -1;
     OP->TInstruction = t;
    break;

    case JMP:
      if ((t = OpAssemble( I_NULL , O_NULL, 1 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->FAddress = v8[1];
      OP->TAddress = OP->FAddress;  
    break;

    case JPS:
      OP->TAddress = v8[1]; 
    break;

    case JPC:
      OP->FAddress = v8[1]; 
    break;

    case FORKF:
      OP->FAddress = v8[1]; 
      OP->TAddress = v8[2];
    break;

    case FORKI:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->FAddress = v8[2]; 
      OP->TAddress = v8[3];
    break;

    case WAITIS:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->FAddress = address;
    break;

    case WAITIC:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->TAddress = address; 
    break;

    case SKIPIC:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->FAddress = address+2;
    break;

    case SKIPIS:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->TAddress = address+2; 
    break;

    case SKIPFC:
      OP->FAddress = address+2;
    break;

    case SKIPFS:
      OP->TAddress = address+2;
    break;

    case REPTIC:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->FAddress = address-1;
    break;

    case REPTIS:
      if ((t = OpAssemble( v8[1] , O_NULL, 1 )) < 0 ) return -1;
      
      OP->FInstruction = t;
      OP->TInstruction = OP->FInstruction;
      OP->TAddress = address-1; 
    break;

    case REPTFC:
      OP->FAddress = address-1;
    break;

    case REPTFS:
      OP->TAddress = address-1; 
    break;

  
  }
  printf("%02X %02X %02X %02X  ", OP->FInstruction, OP->FAddress , OP->TInstruction , OP->TAddress );
  
  printf("%9s ",  strings[0] );
  for (i = 0; i < argCount; i++) printf("0x%02X ", v8[i+1]);   
  
  printf("\n");                            
  
  for( i = 0; strings[i]; i++) {   free(strings[i]); strings[i] = NULL;  }
  return 1;
}


/*
  Bit splicer for generating the instruction codes.
  [-Doooiii]
*/
int OpAssemble(uint8_t input, uint8_t output, uint8_t D ) {

  if ( input > 7 ) { printf("Input value out of range (0-7), Line %d\n", LineNum);  return -1;}
  if ( output > 7) { printf("Output value out of range (0-7), Line %d\n", LineNum); return -2;}
  if ( D > 1)      { printf("Data value out of range (0-1), Line %d\n", LineNum);   return -3;}
 // printf("{ IN:%d OUT:%d, %d }", input, output, ((D==0)?0:1) );
  
  return (input | (output << 3) | ((D==0)?0:(1<<6))); // I wanted to make sure D would always interpret correctly ok?.

}








