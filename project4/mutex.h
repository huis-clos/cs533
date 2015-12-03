#include "scheduler.h"


//struct definitions
 struct mutex {
	int held;
    struct queue waiting_threads;
};

struct condition {
	struct queue waiting_threads;
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

