#include "scheduler.h"
#include <aio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

ssize_t read_wrap(int fd, void * buf, size_t count) {	
      // your code here!
      int status = 0; //return value for I/O check
      int counter = 0;
	  off_t position = 0;  //offset retruned by lseek()
	  int read = 0;

      struct aiocb * my_aiocb = malloc(sizeof(struct aiocb));

      memset(my_aiocb, 0, sizeof(struct aiocb)); //initialize struct to zero
      
      my_aiocb->aio_fildes = fd;  //if valid set in aiocb

      my_aiocb->aio_buf = buf;  //set buffer
      my_aiocb->aio_nbytes = count; //set number of bytes to transfer
      my_aiocb->aio_reqprio = 0;  //I assume zero since priority since not discussed

      position = lseek(fd, 0, SEEK_CUR); //find if there is an offset and if a file is seekable
	  if (fd)
	  {
      	if (position == -1)
      		my_aiocb->aio_offset = 0;
      	else
      		my_aiocb->aio_offset = position;
      }
      else
       	my_aiocb->aio_offset = 0;
	
	  my_aiocb->aio_sigevent.sigev_notify = SIGEV_NONE; //set to allow polling

	  status = aio_read(my_aiocb); //attempt to read

	  if (status == -1) //something bad happened
	  {
	  	printf("Error at read");
	  	return -1;
	  }
	
	  while (aio_error(my_aiocb) == EINPROGRESS) //poll until I/O is finished
	  {
	     yield(); //yield while waiting
	     ++counter;
	     //printf("Loop count: %d", counter);
	  }

	  printf("Count is: %d\n", counter); 

	  status = aio_error(my_aiocb);
	  if (status == 0)
	  {
	  	read = aio_return(my_aiocb); //get bytes read
	  	position = lseek(fd, read, SEEK_CUR); //update offset
	  	if (position != -1)
	  		my_aiocb->aio_offset = position; //if seekable set new offset
	  }

	  else
	  	errno = status;
	
	  return read; //return bytes read 
}
