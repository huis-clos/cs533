//Contexting swtitching library file for project 1
//by Colleen Toth
//for Jon Walpole's CS533
// 09 October 2015

#include <stdio.h>
#include <stdlib.h>

struct thread {
	unsigned char* stack_pointer;
	void (*initial_function)(void*);
	void* initial_argument;
};

int factorial(int n);
void fun_with_threads(void * arg);
extern void thread_switch(struct thread * old, struct thread * new);
extern void thread_start(struct thread * old, struct thread * new);
void thread_wrap();
void yield();

