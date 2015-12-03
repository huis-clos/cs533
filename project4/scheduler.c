#include "scheduler.h"

extern struct thread * current_thread; //global access to current_thread declared i
extern struct queue ready_list;

void thread_wrap() 
{
    current_thread->initial_function(current_thread->initial_argument);
    mutex_lock(&current_thread->a_mutex);
    current_thread->state = DONE;
    condition_broadcast(&current_thread->a_condition);
    mutex_unlock(&current_thread->a_mutex);
    yield();
}

void yield() 
{
	struct thread * next_thread;
	struct thread * temp;
	struct queue * list = &ready_list;

    if (current_thread->state != DONE && current_thread->state != BLOCKED)
    {
    	current_thread->state = READY;
    	thread_enqueue(list, current_thread);
		
		next_thread = thread_dequeue(list);

		if(next_thread != 0)
		{
			next_thread->state = RUNNING;

			temp = current_thread;
			current_thread = next_thread;

			thread_switch(temp, current_thread);
		}
	}

	else
	{
		next_thread = thread_dequeue(list);
		
		if (next_thread != 0)
		{
			temp = current_thread;
			current_thread = next_thread;

			thread_switch(temp, current_thread);
		}
	}

}

void scheduler_begin()
{	
	current_thread = (struct thread *)malloc(sizeof(struct thread));
	current_thread->state = RUNNING;

	ready_list.head = NULL;
	ready_list.tail = NULL;

}

struct thread * thread_fork(void(*target)(void*), void * arg)
{
	struct thread * temp;
    struct thread * new_thread = (struct thread *)malloc(sizeof(struct thread));
    struct queue * list = &ready_list;

    new_thread->stack_pointer = malloc(STACK_SIZE) + STACK_SIZE;
    new_thread->initial_function = target;
    new_thread->initial_argument = arg;
    mutex_init(&new_thread->a_mutex);
    condition_init(&new_thread->a_condition);

    current_thread->state = READY;
    thread_enqueue(list, current_thread);

    new_thread->state = RUNNING;

    temp = current_thread;

    current_thread = new_thread;

    thread_start(temp, current_thread);

    return new_thread;
}


void scheduler_end()
{
	struct queue * list = &ready_list;
	
	while (is_empty(list) == 0)
	{
		yield();
	}	
}


void thread_join(struct thread * th)
{
   mutex_lock(&th->a_mutex);

   if (th->state != DONE)
   		condition_wait(&th->a_condition, &th->a_mutex);
   mutex_unlock(&th->a_mutex);
	return;
}


void mutex_init(struct mutex * m)
{
	m->held = 0;
    m->waiting_threads.head = NULL;
    m->waiting_threads.tail = NULL;

    return;
}


void mutex_lock(struct mutex * m)
{
    if (m->held == 0)
    	m->held = 1;
    else
    {
        current_thread->state = BLOCKED;
        thread_enqueue(&m->waiting_threads, current_thread);
        yield();
    }

    return;
}


void mutex_unlock(struct mutex * m)
{
    if (is_empty(&m->waiting_threads) == 1)
            m->held = 0;

    else
    {
            struct thread * waiting = thread_dequeue(&m->waiting_threads);
            waiting->state = READY;
            thread_enqueue(&ready_list, waiting);
    }

    return;
}

void condition_init(struct condition * c)
{
    c->waiting_threads.head = NULL;
    c->waiting_threads.tail = NULL;
}


void condition_wait(struct condition * c, struct mutex * m)
{
    mutex_unlock(m);
    current_thread->state = BLOCKED;
    thread_enqueue(&c->waiting_threads, current_thread);
    yield();
    mutex_lock(m);

    return;
}



void condition_signal(struct condition * c)
{

    if (is_empty(&c->waiting_threads) == 1)
            return;

    struct thread * waiting_c = thread_dequeue(&c->waiting_threads);
    waiting_c->state = READY;
    thread_enqueue(&ready_list, waiting_c);

    return;
}


void condition_broadcast(struct condition * c)
{
    if (is_empty(&c->waiting_threads) == 1)
            return;

    while(is_empty(&c->waiting_threads) == 0)
    {
        condition_signal(c);
    }

        return;
}
