#ifndef MUTEX_HPP
#define MUTEX_HPP

#include "../types.hpp"
#include "../task/task.hpp"

typedef struct
{
    task::task_t* locker;
    bool locked;
} mutex_t;


void mutex_ctor(mutex_t *m);
void mutex_dtor(mutex_t* m);

mutex_t* mutex_new();
void mutex_delete(mutex_t* m);

// Blocking
int mutex_lock(mutex_t* m);
// Synchron
int mutex_trylock(mutex_t* m);
int mutex_unlock(mutex_t* m);

#endif // MUTEX_HPP
