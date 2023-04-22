#ifndef __ssls2
#define __ssls2

#include "returnValues.h"

#include <sys/select.h> /* tv and fd_set */
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h> /* ioctl */
#include <malloc.h>  /* free */
#include <string.h>  /* strdup */
#include <sys/timeb.h> /* ftime */
#include <linux/serial.h> /* stuff to set custom baud rate */
#include <pthread.h> /* threading for callbacks */
 /* if there is a header that exists you dont see included, please tell me */


#define SerDataReady(a) (SerBytesIn(a) > 0)

typedef struct SSLS_s {
  int fd;
  int baud;
  pthread_t monitorThread;
  void (*charHandler)(unsigned char*, void *);
  void * callBackPtr;
  char * filename;
} SSLS_t;


Status_t SerRead    (SSLS_t *this, char **data, int * count);
Status_t SerWrite   (SSLS_t *this, char *data, int count);
Status_t SerGetBaud (SSLS_t *this, int  *baud);
Status_t SerSetBaud (SSLS_t *this, int  baud);
Status_t SerOpen    (SSLS_t *this, char *file, int baud);
Status_t SerWriteNoWait(SSLS_t *this, char *data, int count);

int rateToConstant(int baud) ;
int SerBytesIn(SSLS_t *this) ;
Status_t SerStartNotifier(SSLS_t *this, void (*charHandler)(unsigned char*, void *), void * anchor) ;
void *SerNotifierThread(SSLS_t *this) ;

Status_t converse(SSLS_t * port, char **string, int uswait) ;
Status_t waitConverse(SSLS_t * this, char **string, int startWait, int interWait) ;
Status_t exchange(SSLS_t * this, char **data, int *count, int uswait) ;

Status_t SerInit    (SSLS_t *this);
Status_t SerFini    (SSLS_t *this);

#endif
