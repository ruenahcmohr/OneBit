/*
             CREATOR: DAn williams
            FILENAME: cmnCmdOptions.c
             STARTED: Nov 27
           PLATFORMS: Linux
         DESCRIPTION: Gathering of code to parse command line options. "library"


 cmdLineOptionInit:
   Call this first to initialize variables;
   DO NOT call this to clear added options! (memory leak)

 addCmdLineOption:
   Call a few of these to define options for your command line
   Handler procedures that are called need to return the number of extra 
     options they used, for this implimentation 0 or 1 e.g. specifying a filename (-I foo.foo)
     would take 1 extra paramiter, setting a verbose flag (-V) would take none.
   return:
    0 = success
    1 = failed (out of memory)
    2 = failed (out of index space ( 32767 entries)) 

 parseOptions:
  Call this after all your addCmdLineOption calls to actually evaluate the command line
  If there are multiple declarations for the same option it uses the last one declared.
  Pass this the same paramiters that come in from main e.g. 
      parseOptions(argc, argv);
    where int main(int argc, char** argv);


 cleanupOptionStuff:
  Mine! leave it alone!

*/

#include <stdio.h>
#include <string.h>
#include "cmnCmdOptions2.h"
#include "returnValues.h"

/*
  Take a string and break it down into argv and argc.
  convienience function.
*/
Status_t string2args(char * in, unsigned int * argc, char *** argv) {

  char * tok;
  char ** tempPtr;

  *argc = 0;
  tok   = strtok(in, " \t\n\r");
  while (tok != NULL) {
    
    if ((tempPtr = (char **)realloc(*argv, sizeof(char **)*((*argc)+1))) == NULL) {
      return ErrMalloc;     // if fail return 1
    } else {
      *argv = tempPtr; 
    }
    
    (*argv)[*argc] = tok;
    (*argc)++;
    tok = strtok(NULL, " \t\n\r");
  }
  return OK;
}


/* 
   call this first to reset the variables
   DO NOT call this to clear added options! (memory leak)
*/
Status_t cmdLineOptionInit(CLOSet_t ** this) {
  
  if ((*this = (CLOSet_t *)realloc(*this, sizeof(CLOSet_t))) == NULL) {
    return ErrMalloc;     // if fail return 1
  } 

  (*this)->CmdOptions      = NULL; //no defined options
  (*this)->LineCommands    = 0;    // no defined options
  (*this)->cmdParseVerbose = 1;    // put error to stdout
  (*this)->initialized     = 1;
  
  return OK;
}

/* 
   call this first to reset the variables
   DO NOT call this to clear added options! (memory leak)
*/
Status_t cmdLineOptionFini(CLOSet_t ** this) {

  
  return cleanupOptionStuff( this );
  
}



/* 
   Call a few of these to define options for your command line
   Handler procedures that are called need to return the number of extra 
     options they used, for this implimentation 0 or 1 e.g. specifying a filename (-I foo.foo)
     would take 1 extra paramiter, setting a verbose flag (-V) would take none.
   return:
    0 = success
    1 = failed (out of memory)
    2 = failed (out of index space ( 32767 entries)) 
*/
Status_t addCmdLineOption(CLOSet_t * this, char** optionNames, int optionNameCount, int (*newHandler)(char*)){
    
  CLOption_t* tempPtr;

  // modify paramiter counter
  if ((this->LineCommands)+1 > this->LineCommands) {
    (this->LineCommands)++;
  } else {
    return Full;
  }

  // allocate memory to expand array

  if ((tempPtr = (CLOption_t *)realloc(this->CmdOptions, sizeof(CLOption_t)*(this->LineCommands))) == NULL) {
    return ErrMalloc;     // if fail return 1
  } else {
    this->CmdOptions = tempPtr; 
  }
  
  
  // add entry
  this->CmdOptions[this->LineCommands-1].handler       = newHandler;
  this->CmdOptions[this->LineCommands-1].CallNameCount = optionNameCount;
  this->CmdOptions[this->LineCommands-1].CallName      = optionNames;

  return OK;

}



/*
  Call this after all your addCmdLineOption calls to actually evaluate the command line
  If there are multiple declarations for the same option it uses the last one declared.
  Pass this the same paramiters that come in from main e.g. 
      parseOptions(argc, argv);
    where int main(int argc, char** argv);
*/
Status_t parseOptions(CLOSet_t * this, int argCount, char** argVars) {

  // pardon me lord, for I have copied and pasted code. (but I swear, I made it better!)
  
  int currOpt;
  int cmdIndex;
  int cmdSubIndex;
  int matched;

  /* 
   do some string comparisons to find out what we should do.
   On Intelish hardware, a good compiler should be able to get some extra
     speed from me having made the loops backwards.
   Handler procedures that are called need to return the number of extra 
     options they used, for this implimentation 0 or 1
  */

  currOpt = 0; // start at option 1
  matched = 0; // which is not initially matched to anything

  while (currOpt < argCount) {  // for all the options...

    for (cmdIndex = (this->LineCommands-1); cmdIndex >= 0; cmdIndex--) { // go through each command

      for (cmdSubIndex = (this->CmdOptions[cmdIndex].CallNameCount-1);cmdSubIndex >= 0; cmdSubIndex--) {// by each name

	if (strcmp(argVars[currOpt],this->CmdOptions[cmdIndex].CallName[cmdSubIndex]) == 0) {// and if its a match...
          if ((currOpt+1) <= argCount) {// if were not out of user input
	    currOpt += (*this->CmdOptions[cmdIndex].handler)(argVars[currOpt+1]); // run the handler with givn string
	  } else { 
	    currOpt += (*this->CmdOptions[cmdIndex].handler)(""); // run handler with MT string
	  }
          matched = 1; // found it!
          cmdSubIndex = (-1); // stop for!
          cmdIndex = (-1); // stop for!!
	} // close " if strings matched"
      } // end of for cmdSubIndex loup
    } // end of for cmdIndex loup
    if ((matched == 0)&(this->cmdParseVerbose == 1)) {
      printf("Ignoring unknown option %s\n", argVars[currOpt]);
    }
    matched = 0; //reset match flag.
    currOpt++; // next victim
  } // end of while loup

  // were done, wrap up
  // cleanupOptionStuff();
  return OK;
}


Status_t cleanupOptionStuff(CLOSet_t ** this){
  
  
  // reset variables
  (*this)->LineCommands = 0;
  (*this)->initialized  = 0;
  
  // dealloc memory
  free((*this)->CmdOptions);
  
  return OK;

}



/*


int cmdHi(char * unused) {
  printf( "Hi\n");
  return 0;
}

int cmdHello(char * unused) {
  printf( "Hello\n");
  return 0;
}


void setupParams(CLOSet_t ** this) {

  // this is outright ugly, but we have to because of limits to C
  static char* optsHi[2]    = {"-h","--hi"};
  static char* optsHello[1] = {"-H"};

  // first init 
  cmdLineOptionInit(this);
  // then add your options (note I ignore any errors (BAD! BAD!))
  addCmdLineOption(*this, optsHi, 2, cmdHi);
  addCmdLineOption(*this, optsHello, 1, cmdHello);

}


int main( int argc, char ** argv) {
 
  CLOSet_t * myCommands; 
  myCommands = NULL; 

  setupParams(&myCommands); // define the arguments
  parseOptions(myCommands, argc, argv);   // then parse the command line

}

*/




// Lex? Yacc?? what are those :-)









