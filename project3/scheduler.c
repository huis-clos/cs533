#include "scheduler.h"
#include "queue.h"

struct thread * current_thread;
struct queue ready_list;

void thread_wrap() 
{
    current_thread->initial_function(current_thread->initial_argument);
    current_thread->state = DONE;
    yield();
}

void yield() 
{
	struct thread * next_thread;
	struct thread * temp;
	struct queue * list = &ready_list;

    if (current_thread->state != DONE)
    {
    	current_thread->state = READY;
    	thread_enqueue(list, current_thread);
		
		next_thread = thread_dequeue(list);
		next_thread->state = RUNNING;

		temp = current_thread;
		current_thread = next_thread;

		thread_switch(temp, current_thread);
	}

	else
	{
		next_thread = thread_dequeue(list);
		temp = current_thread;
		current_thread = next_thread;

		thread_switch(temp, current_thread);
	}

}

void scheduler_begin()
{	
	current_thread = (struct thread *)malloc(sizeof(struct thread));
	current_thread->state = RUNNING;

	ready_list.head = NULL;
	ready_list.tail = NULL;

}

void thread_fork(void(*target)(void*), void * arg)
{
	struct thread * temp;
    struct thread * new_thread = (struct thread *)malloc(sizeof(struct thread));
    struct queue * list = &ready_list;

    new_thread->stack_pointer = malloc(STACK_SIZE) + STACK_SIZE;
    new_thread->initial_function = target;
    new_thread->initial_argument = arg;

    current_thread->state = READY;
    thread_enqueue(list, current_thread);

    new_thread->state = RUNNING;

    temp = current_thread;

    current_thread = new_thread;

    thread_start(temp, current_thread);

}


void scheduler_end()
{
	struct queue * list = &ready_list;
	
	while (is_empty(list) == 0)
	{
		yield();
	}	
}
