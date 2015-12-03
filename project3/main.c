#include "scheduler.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

void print_nth_prime(void * pn) {
  int n = *(int *) pn;
  int c = 1, i = 1;
  while(c <= n) {
    ++i;
    int j, isprime = 1;
    for(j = 2; j < i; ++j) {
        if(i % j == 0) {
            isprime = 0;
            break;
        }
    }
    if(isprime) {
        ++c;
    }
    yield();
  }
  printf("%dth prime: %d\n", n, i);

  return;
                                                                         
}

void usr_test()
{
	char input[50];
	size_t len = 0;
	ssize_t read;

	printf("Please enter something: \n");
	len = sizeof(input);

	printf("Calling I/O on input. \n");
	read_wrap(0, &input, len);
	printf("Finished I/O\t");
	printf("Bytes read: %zd\n", read);

	return;
}

void japruf_test(char * arg)
{
	char * filename = arg;
	int fd = open(arg, O_RDONLY);
	char file_input[44];
	ssize_t len = 0;
	ssize_t read = -1;
	
	len = sizeof(file_input);
	
	read = read_wrap(fd, file_input, len);
	
	while (read != 0)
	{
	    printf("%s\n", file_input);
	    read = read_wrap(fd, file_input, len);
	} 
	return;                    
}

int main(void) {

	char file[50];

	printf("Enter a filename\n");
	scanf("%s", file);

    scheduler_begin();

    int n1 = 20000, n2 = 10000, n3 = 30000;
    thread_fork(print_nth_prime, &n1);
    thread_fork(usr_test, NULL);
    //thread_fork(japruf_test, &file);
    

    scheduler_end();
}
