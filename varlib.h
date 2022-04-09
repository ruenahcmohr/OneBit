#ifndef __varlib
#define __varlib

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "returnValues.h"


typedef struct variable_s {
    double value;
    char * name;
    unsigned writable : 1;
} variable_t;


typedef struct varlist_s {
    variable_t * var;
    unsigned int varCount;
} varlist_t;




Status_t writeVar   (variable_t * this, char * name, double value, char  writable);
Status_t findIndex  (varlist_t * this, char * name, unsigned int * index) ;
Status_t addVar     (varlist_t * this, char * name, double value, char  writable) ;
Status_t eraseVar   (varlist_t * this, unsigned int i);

Status_t initVars   (varlist_t * this);
Status_t setVar     (varlist_t * this, char * name, double value, char  writable) ;
Status_t setVarVal  (varlist_t * this, char * name, double value) ;
Status_t setVarPerm (varlist_t * this, char * name, char  writable) ;
Status_t evalVar    (varlist_t * this, char * name, double * value) ;
Status_t delVar     (varlist_t * this, char * name) ;
Status_t finiVars   (varlist_t * this);

Status_t dumpVars   (varlist_t * this) ;
Status_t showVar    (variable_t * this ) ;


#endif
