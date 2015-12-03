//Library for thread scheduler file for project 2 
//for Jon Walpole's CS533 project 2
//by Colleen Toth
//17 October 2015

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define STACK_SIZE 1024*1024
#define current_thread (get_current_thread())

typedef enum {
    RUNNING, // The thread is currently running.
    READY,   // The thread is not running, but is runnable.
    BLOCKED, // The thread is not running, and not runnable.
    DONE     // The thread has finished. 
} state_t;

struct mutex {
	int held;
	struct queue waiting_threads;
};
    
struct condition {
	struct queue waiting_threads;
};
//struct definition for thread
struct thread {
	unsigned char* stack_pointer;
	void (*initial_function)(void*);
	void* initial_argument;
	state_t state;
	struct mutex a_mutex;
	struct condition a_condition;
};


//mutex functions
void mutex_init(struct mutex *);
void mutex_lock(struct mutex *);
void mutex_unlock(struct mutex *);

//condition variable functions
void condition_init(struct condition *);
void condition_wait(struct condition *, struct mutex *);
void condition_signal(struct condition *);
void condition_broadcast(struct condition *);

//API functions
void scheduler_begin();
struct thread * thread_fork(void(*target)(void*), void * arg);
void yield();
void scheduler_end();
void thread_join(struct thread*);
extern void thread_switch(struct thread * old, struct thread * new);
extern void thread_start(struct thread * old, struct thread * new);

struct thread * current_thread; //global access to current_thread declared in scheduler.c
struct queue ready_list;
ssize_t read_wrap(int fd, void * buf, size_t count);
