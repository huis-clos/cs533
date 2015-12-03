#include"mutex.h"

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
	if (m->waiting_threads.head == NULL)
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

	if (c->waiting_threads.head == NULL)
		return;
	
	struct thread * waiting_c = thread_dequeue(&c->waiting_threads);
	waiting_c->state = READY;
	thread_enqueue(&ready_list, waiting_c);

	return;
}


void condition_broadcast(struct condition * c)
{
	if (c->waiting_threads.head == NULL)
		return;

	while(c->waiting_threads.head != NULL)
	{
		condition_signal(c);
	}

	return;
}
