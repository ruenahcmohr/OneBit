#ifndef __returnValues
#define __returnValues

typedef enum Status_e { OK = 0, ErrMalloc, Full, BadArg, CantOpen, BadData, EarlyEOF, Overflow, NoMatch, Forbidden, ExecFail, NotReady, MultiMatch, NoImp, CantSend } Status_t;

//char * status_s[] = { "OK", "Malloc Error", "Full", "Bad Argument","Can't open", "Bad data", "Early EOF", "Overflow", "No match", "Forbidden", "Failed to start", "not ready", "multiple matches", "Not Implemented" };

extern char * status_s[];

//#define Buckle(A) if((_STATUS_V_ = (A))!=OK)) return _STATUS_V_)

#endif
