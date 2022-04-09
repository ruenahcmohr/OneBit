#include "varlib.h"


Status_t showVar( variable_t * this ) {
  printf("(%s) %s = %f \n", this->writable==1?"RW":"RO", this->name, this->value );
  return OK;
}

Status_t initVars (varlist_t * this) {
  this->varCount = 0;
  this->var      = NULL;
  return OK;
}

Status_t finiVars (varlist_t * this){
   unsigned int i;
    for(i = 0; this->varCount > 0; i++) {
      eraseVar(this, 0);
    }
    return OK;
}


Status_t writeVar(variable_t * this, char * name, double value, char  writable) {
  this->value    = value;
  if (this->name)  free(this->name);
  this->name     = strdup(name);
  this->writable = writable;
  return OK;
}

Status_t eraseVar(varlist_t * this, unsigned int i) {
  if (this->var[i].name != NULL)   free(this->var[i].name); // free the deleted name
  this->var[i].name     = this->var[(this->varCount)-1].name; //move last item to old slot, if this is the same its ok
  this->var[i].value    = this->var[(this->varCount)-1].value;
  this->var[i].writable = this->var[(this->varCount)-1].writable;
  this->varCount--;                                                   //resize list, killing last item
  this->var             = realloc(this->var, sizeof(variable_t)*this->varCount); 
  return OK;
}

Status_t findIndex(varlist_t * this, char * name, unsigned int * index) {
  unsigned int i;
  for(i = 0; i < this->varCount; i++) {
    if (strcmp(this->var[i].name, name) == 0 ) {
      *index = i;
      return OK;
    }
  }
  
  return NoMatch;

}

Status_t addVar(varlist_t * this, char * name, double value, char  writable) {  //!!!???!!! add error checking  
  variable_t * temp;

  if ((temp = realloc(this->var, sizeof(variable_t)*(this->varCount+1)))==NULL ) {
    return ErrMalloc;
  }
  this->var = temp;
  this->var[this->varCount].name = NULL;
  this->varCount++;
  return writeVar(&this->var[this->varCount-1], name, value, writable);
}

Status_t setVar(varlist_t * this, char * name, double value, char  writable) {
  unsigned int i;
  
  if (findIndex(this, name, &i) == OK) {
    return writeVar(&this->var[i], name, value, writable) ;
  }
  
  return addVar(this, name, value, writable);
  
}

Status_t setVarVal  (varlist_t * this, char * name, double value) {
  unsigned int i;
  
  if (findIndex(this, name, &i) == OK) {
    if (this->var[i].value != value) {    // this is actually a hack, if your trying to write the same value to it, its not a write.
      if (this->var[i].writable == 1) {
        this->var[i].value = value;
        return OK;
      } else {
        return Forbidden;
      }
    } else {
      return OK;
    }
  } else {
    return addVar(this, name, value, 1);
  }
}

Status_t setVarPerm (varlist_t * this, char * name, char  writable) {
  unsigned int i;
  
  if (findIndex(this, name, &i) == OK) {
      this->var[i].writable = writable;
      return OK;
  } else {
    return addVar(this, name, 0.00, writable);
  }
}

Status_t evalVar(varlist_t * this, char * name, double * value) {
   unsigned int i;
   if (findIndex(this, name, &i) == OK) {
    *value = this->var[i].value;
    return OK;
  }
  return NoMatch;
}

Status_t dumpVars(varlist_t * this) {
    unsigned int i;
    for(i = 0; i < this->varCount; i++) {
      printf("%d:   ", i);
      showVar(&this->var[i]);
    }
    return OK;
}


Status_t delVar(varlist_t * this, char * name) {
  unsigned int i;
  
  if (findIndex(this, name, &i) == OK)  return eraseVar(this, i);
  else                                  return NoMatch;
  
}
/*
int main(void) {

  varlist_t myvars;
  unsigned int i;
  double v;
 
  initVars(&myvars);
   
  setVar(&myvars, "dummy", 5.26, 1);
  setVar(&myvars, "xyz", 8.0, 1); 
  setVar(&myvars, "asdf", 6.4, 1); 
  setVar(&myvars, "qewr", 987.345, 1); 
  setVar(&myvars, "gear", 7.02, 1); 
  setVar(&myvars, "exgwa", 5.282, 1); 
  
  if (!evalVar(&myvars, "asdf", &v)) {
    printf("Value is: %f\n", v);
  }
  
  dumpVars(&myvars);
  
  delVar(&myvars, "qewr" );
  delVar(&myvars, "dummy");
  setVar(&myvars, "dan", 25.00, 0);
 
  printf("write to dan -> %s\n",status_s[setVarVal  (&myvars, "dan", 13.13)]);
  
  dumpVars(&myvars);
  finiVars(&myvars);
  printf("empty:\n");
  dumpVars(&myvars);
}
*/

/*
void test() {
  variable_t myvar;
  
  myvar.value    = 4.68;
  myvar.name     = strdup("dummy");
  myvar.writable = 1;

  showVar(&myvar);
  
  myvars.varCount++;
  myvars.var = realloc(myvars.var, sizeof(variable_t)*myvars.varCount);
  
  writeVar(&myvars.var[0], "dummy", 5.26, 1);
 
  addVar(&myvars, "dummy", 5.26, 1);
  
  myvars.varCount++;
  myvars.var = realloc(myvars.var, sizeof(variable_t)*myvars.varCount);
  
  writeVar(&myvars.var[1], "xyz", 8.0, 1);
  
  addVar(&myvars, "xyz", 8.0, 1); 
  
  if (findIndex(&myvars, "dummy", &i) == 0) {
    printf("found: ");
    showVar(&myvars.var[i]);
  }
  
  //showVar(&myvars.var[0]);
  //showVar(&myvars.var[1]);

}

*/
