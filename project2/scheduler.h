//Library for thread scheduler file for project 2 
//for Jon Walpole's CS533 project 2
//by Colleen Toth
//17 October 2015

#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 1024*1024

typedef enum {
    RUNNING, // The thread is currently running.
    READY,   // The thread is not running, but is runnable.
    BLOCKED, // The thread is not running, and not runnable.
    DONE     // The thread has finished. 
} state_t;


//struct definition for thread
struct thread {
	unsigned char* stack_pointer;
	void (*initial_function)(void*);
	void* initial_argument;
	state_t state;
};

//API functions
void scheduler_begin();
void thread_fork(void(*target)(void*), void * arg);
void yield();
void scheduler_end();
extern void thread_switch(struct thread * old, struct thread * new);
extern void thread_start(struct thread * old, struct thread * new);

extern struct thread * current_thread; //global access to current_thread declared in scheduler.c
