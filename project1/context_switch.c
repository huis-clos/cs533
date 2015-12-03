//This is the context switching implementation file
//for projexct 1 for Jon Walpole's CS533
//written by Colleen Toth
//09 Octiber 2015

#include "context_switch.h"

struct thread * current_thread; 
struct thread * inactive_thread;

#define STACK_SIZE 1024*1024

int main()
{
	current_thread = (struct thread *)malloc(sizeof(struct thread));

	inactive_thread = malloc(sizeof(struct thread));

	current_thread->stack_pointer = malloc(STACK_SIZE) + STACK_SIZE;

	current_thread->initial_function = fun_with_threads;

	int * p = malloc(sizeof(int));
	*p = 5;
	current_thread->initial_argument = p;

	//current_thread->initial_function(current_thread->initial_argument);

	thread_start(inactive_thread, current_thread);

	return 0;
}

int factorial(int n) 
{
    return n == 0 ? 1 : n * factorial(n-1);
}

void fun_with_threads(void * arg)
{
	int n = *(int*) arg;
	printf("%d! = %d\n", n, factorial(n));
}

void thread_wrap() {
    current_thread->initial_function(current_thread->initial_argument);
    yield();
}

void yield() 
{
    struct thread * temp = current_thread;
    current_thread = inactive_thread;
    inactive_thread = temp;
    thread_switch(inactive_thread, current_thread);
}
