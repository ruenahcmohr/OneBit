/* 
   by rue_mohr 2006-2012
   
   - blocking writes
   - default 9600 buad
   
    May 2015 added generic pointer to callback fn
   
*/

#include "ssls3.h"

// use to initialize data structure
Status_t SerInit (SSLS_t *this){

  this->fd   = -1;
  this->baud = 9600;
  this->filename = NULL;

  return OK;
}

// use to initiate communications
Status_t SerOpen (SSLS_t *this, char *file, int baud){

   struct termios options;
     
   /*
   O_NDELAY causes non-blocking read/write   
   <DagoRed>  O_FSYNC         All writes immediately effective, no buffering
   <DagoRed> O_EXLOCK        Get an "exclusive lock" on the file 
   but none of these options do squat
   */
   if ((this->fd = open(file, O_RDWR | O_NOCTTY /*| O_SYNC | O_NDELAY*/)) == -1)   return CantOpen;   
                  
   /* this is supposed to set blocking mode, dosn't do squat    
    fcntl(this->fd, F_SETFL, 0);   */
  
   bzero(&options, sizeof(options)); /* clear struct for new port settings */
   
   // Enable the receiver and set local mode...
   /*  options.c_lflag |= ~(ICANON | ECHO | ECHOE);  some useless options you might consider */
   
   options.c_cflag = B38400 | CS8 | CLOCAL | CREAD;    // B38400 allows us to use a custom baud rate                                                       
   options.c_iflag = IGNPAR | IGNBRK ;
          
   // Set the new options for the port...
   
   tcflush(this->fd, TCIFLUSH);
   tcsetattr(this->fd, TCSANOW, &options);  
        
   SerSetBaud(this, baud);
   
  // if (this->filename != NULL) free(this->filename);
   this->filename = strdup(file);
   
   return OK;
}


int rateToConstant(int baud) {

 switch(baud) {
  case 50:      return B50;
  case 75:      return B75;
  case 110:     return B110;
  case 134:     return B134;
  case 150:     return B150;
  case 200:     return B200;
  case 300:     return B300;
  case 600:     return B600;
  case 1200:    return B1200;
  case 1800:    return B1800;
  case 2400:    return B2400;
  case 4800:    return B4800;
  case 9600:    return B9600;
  case 19200:   return B19200;
  case 38400:   return B38400;
  case 57600:   return B57600;
  case 115200:  return B115200;
  case 230400:  return B230400;
  case 460800:  return B460800;
  case 500000:  return B500000;
  case 576000:  return B576000;
  case 921600:  return B921600;
  case 1000000: return B1000000;
  case 1152000: return B1152000;
  case 1500000: return B1500000;
  default:      return 0;
 }

}


// use to set communication rate
Status_t SerSetBaud(SSLS_t *this, int baud) {

   struct serial_struct ser_info;
   struct termios options;
   int    speed = 0;
 
   if (baud == 0) return BadArg;
   this->baud = baud;   
   if (this->fd == -1) return NotReady;
   
   // check and handle custom baud rates
   if ((speed = rateToConstant(baud)) == 0) {
   /*
     ioctl(this->fd, TIOCGSERIAL, &ser_info);                         // change up baud rate
   ser_info.flags        &= ~ASYNC_SPD_MASK;                        // by turning off important things,
   ser_info.flags         |= ASYNC_SPD_CUST ;                       // by turning on the dangerous switches,
   ser_info.custom_divisor = ser_info.baud_base / baud;             // calculating dangerous numbers,
   ioctl(this->fd, TIOCSSERIAL, &ser_info);                         // and jamming them back into the system
   */
   /* Custom divisor */
     ser_info.reserved_char[0] = 0;
     if (ioctl(this->fd, TIOCGSERIAL, &ser_info) < 0)     return BadArg;
     
     ser_info.flags          &= ~ASYNC_SPD_MASK;
     ser_info.flags          |=  ASYNC_SPD_CUST;
     ser_info.custom_divisor  = (ser_info.baud_base + (baud / 2)) / baud;
     
     if (ser_info.custom_divisor < 1)               ser_info.custom_divisor = 1;
     if (ioctl(this->fd, TIOCSSERIAL, &ser_info) < 0)     return -1;
     if (ioctl(this->fd, TIOCGSERIAL, &ser_info) < 0)     return -1;
   /*  if (ser_info.custom_divisor * rate != ser_info.baud_base) {
	     warnx("actual baudrate is %d / %d = %f",
		   serinfo.baud_base, serinfo.custom_divisor,
		   (float)serinfo.baud_base / serinfo.custom_divisor);
     }*/
        
   }
   

   fcntl(this->fd, F_SETFL, 0);
   tcgetattr(this->fd, &options);
   cfsetispeed(&options, speed != 0 ? speed : B38400);
   cfsetospeed(&options, speed != 0 ? speed : B38400);
   cfmakeraw(&options);
   options.c_cflag |= (CS8 | CLOCAL | CREAD);
   options.c_cflag &= ~CRTSCTS;
   options.c_iflag = IGNPAR | IGNBRK ; // ignore framing and parity errors, ignore break
   tcflush(this->fd, TCIFLUSH);
   if (tcsetattr(this->fd, TCSANOW, &options) != 0)	   return BadArg;
            
   return OK;
}

// use to find out current communication rate
Status_t SerGetBaud(SSLS_t *this, int * baud) {

   struct serial_struct ser_info;
 
   ioctl(this->fd, TIOCGSERIAL, &ser_info);
     
   *baud = ser_info.baud_base / ser_info.custom_divisor;  
   
   return OK;
}

// use to send to it
Status_t SerWrite(SSLS_t *this, char *data, int count){
   int lsr;
   int bytes;
   
 //  printf("Waiting...  \n");
 
   /* // this dosn't work unless (sometimes) there is a printf("\n") before it...
   do {
       ioctl(this->fd, TIOCSERGETLSR, &lsr);
   } while (lsr & TIOCSER_TEMT); // transmitter empty
   */
   
   /* FIONWRITE dosn't exist on linux....
   do {
     ioctl(this->fd, FIONWRITE, &bytes);
   while (bytes != 0);*/
    
   tcdrain(this->fd); // finish write
   
  // printf("      Sending data to port... ");
   if (write(this->fd, data, count)< 0) {
      fprintf(stderr, "write failed! %s (%d)\n", this->filename, this->fd );
      perror("arm write");
      return Forbidden;
   }
   
  // tcdrain(this->fd); // finish write
  /*
   do {
       ioctl(this->fd, TIOCSERGETLSR, &lsr);
   } while (!(lsr & TIOCSER_TEMT));
    */
    
   return OK;
}

Status_t SerWriteNoWait(SSLS_t *this, char *data, int count){

   if (write(this->fd, data, count)< 0) {
      fputs("write failed!\n", stderr);
      return Forbidden;
   }

   return OK;
}


// use to get 0 or more bytes that are ready from it now
Status_t SerRead(SSLS_t *this, char **data, int * count){

   int bytes, retbytes;
   
   if (*data) { 
      free(*data); // free old buffers
      *data = NULL;
   }
   if (count) *count = 0;   // preset byte count;
   
   ioctl(this->fd, FIONREAD, &bytes);
   if (bytes == 0) return OK;
   
   if ((*data = malloc(bytes+1)) == NULL) {
     *data = NULL;
     return ErrMalloc;
   }
   
   retbytes = read(this->fd, *data, bytes);
 //  printf(">>> %s\n", *data);
   if (count) *count = retbytes;   
     
   if (retbytes < 0) {
      if (errno == EAGAIN) {
         printf("Access problem with serial port\n");
         return Forbidden;
      } else {
         printf("SERIAL read error %d %s\n", errno, strerror(errno));
         return Forbidden;
      }
   } else {
     (*data)[retbytes] = 0;   // use the extra byte for null termination
   }
   
                  
   return OK;
}

// polling mechanism
// use to know how much information is has for you
int SerBytesIn(SSLS_t *this) {
   int bytes;
   ioctl(this->fd, FIONREAD, &bytes);
   return bytes; 
}


/*

 use timeout to 
  9600 is 104 us per character
  
*/
// use when you know a command will have a more-or-less immediate reply
Status_t converse(SSLS_t * this, char **string, int uswait) {

   int retval; 
   int bytes;

 //  printf("    Writing data...\n");
   if ((retval = SerWrite(this, *string, strlen(*string))) != OK) 
       return retval;
                 
   do {
     bytes = SerBytesIn(this);
   //  printf("    --> %d bytes\n", bytes);
     usleep(uswait);     
   }  while(SerBytesIn(this) > bytes);         
       
   return SerRead(this, string, NULL);
   
}

/*

 use timeout to 
  9600 is 104 us per character
  
*/
// use when you know a command will have a more-or-less immediate reply, but with a long delay to the first char.
Status_t waitConverse(SSLS_t * this, char **string, int startWait, int interWait) {

   int retval; 
   int bytes;
   fd_set rfds;
   struct timeval tv;

   //printf("    Writing data...\n");
   //printf("\n");
   if ((retval = SerWrite(this, *string, strlen(*string))) != OK) 
       return retval;
     
   //printf("    Waiting for first character...\n"); 
   // can I do a select?  
   FD_ZERO(&rfds);
   FD_SET(this->fd, &rfds);
  // FD_SET(0, &rfds);
   tv.tv_sec  = 0 ;
   tv.tv_usec = startWait;
   retval = select((this->fd)+1, &rfds, NULL, NULL, &tv); 
   if (SerBytesIn(this) == 0)
     return SerRead(this, string, NULL);
       
   //printf("    Waiting for successive characters...\n");     
   do {
     bytes = SerBytesIn(this);
   //  printf("    --> %d bytes\n", bytes);
     usleep(interWait);     
   }  while(SerBytesIn(this) > bytes);         
       
   return SerRead(this, string, NULL);
   
}





// use when you know a command will have a more-or-less immediate reply
// this one is for binary data
Status_t exchange(SSLS_t * this, char **data, int *count, int uswait) {

   int retval; 
   int bytes;

   if ((retval = SerWrite(this, *data, *count)) != OK) 
       return retval;
                 
   do {   // rake up the data
     bytes = SerBytesIn(this);
     usleep(uswait);     
   }  while(SerBytesIn(this) > bytes);         
       
   return SerRead(this, data, count);
   
}

/*
NO, I'm not gonna be gracefull and put the port back like it was before we started,
it took a LOT OF WORK to mess it up that much!
*/
// use to clean up when done
Status_t SerFini    (SSLS_t *this) {
  if (!this) return BadArg;
  close(this->fd);
  this->fd = -1;
  return OK;
}

//callback mechanism
// use when you want to hear what it says without knowing when it may speak
Status_t SerStartNotifier(SSLS_t *this, void (*charHandler)(unsigned char*, void*), void * anchor) {
   
   // set callback fn.
   this->charHandler = charHandler;
   
   // set the callbacks pointer
   this->callBackPtr = anchor;
   
   // start thread
   if( (pthread_create( &(this->monitorThread), NULL, (void * (*)(void *))&SerNotifierThread, this)) ){
      printf("Failed to start serial monitor thread \n");
   }
}


void *SerNotifierThread(SSLS_t *this) {

  unsigned char data;

  while(1) {
    if (read(this->fd, &data, 1) > 0)   this->charHandler(&data, this->callBackPtr);
  }

}

/*
// this isn't for you, dont use it
void *SerNotifierThread(SSLS_t *this) {
   fd_set rfds;
   struct timeval tv;
   int retval;

   while(1) {
      FD_ZERO(&rfds);
      FD_SET(this->fd, &rfds);
      FD_SET(0, &rfds);
      tv.tv_sec  = 0 ;
      tv.tv_usec = 250000;
      retval = select((this->fd)+1, &rfds, NULL, NULL, &tv);

      
      if (retval == -1) {
            perror("select()");
      } else if (retval) {              
         if ( FD_ISSET(this->fd, &rfds) ) {      
               
          // read input
          /*
             if ((*data = malloc(bytes+1)) == NULL) {
     *data = NULL;
     return ErrMalloc;
   }
   
   bytes = read(this->fd, *data, bytes);
   if (count) *count = bytes;   
     
   if (bytes < 0) {
      if (errno == EAGAIN) {
         printf("Access problem with serial port\n");
         return Forbidden;
      } else {
         printf("SERIAL read error %d %s\n", errno, strerror(errno));
         return Forbidden;
      }
   } else {
     (*data)[bytes] = 0;   // use the extra byte for null termination
   }
          
          // call callback with data
            // charHandler(
         }  
      }                       
   }
} 

*/














