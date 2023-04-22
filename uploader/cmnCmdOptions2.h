#ifndef __cmnCmdOptions2
#define __cmnCmdOptions2

#include "returnValues.h"
#include <stdlib.h>

// descriptor of a command line option
typedef struct CLOption_s {
  int  (*handler)(char*);
  int  CallNameCount;
  char **CallName;
} CLOption_t;

typedef struct CLOSet_s {
  CLOption_t*   CmdOptions      ;  // pointer to an array of command line options
  int           LineCommands    ;
  int           cmdParseVerbose ;
  int           initialized     ;
} CLOSet_t;


Status_t string2args(char * in, unsigned int * argc, char *** argv) ;
Status_t cmdLineOptionInit(CLOSet_t ** this);
Status_t cmdLineOptionFini(CLOSet_t ** this);
Status_t addCmdLineOption(CLOSet_t * this, char** optionNames, int optionNameCount, int (*newHandler)(char*));
Status_t parseOptions(CLOSet_t * this, int argCount, char** argVars);
Status_t cleanupOptionStuff(CLOSet_t ** this);

#endif
