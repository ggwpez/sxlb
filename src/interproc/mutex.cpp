#include "mutex.hpp"
#include "../memory/memory.hpp"

void mutex_ctor(mutex_t* m)
{
    m->locked = false;
    m->locker = nullptr;
}

void mutex_dtor(mutex_t* m)
{
    m->locked = false;
    m->locker = nullptr;
}

mutex_t* mutex_new()
{
    mutex_t* m = (mutex_t*)memory::k_malloc(sizeof(mutex_t), 0, nullptr);

    if (!m)
        return nullptr;

    mutex_ctor(m);
    return m;
}

void mutex_delete(mutex_t* m)
{
    mutex_dtor(m);

    memory::k_free(m);
}

int mutex_lock(mutex_t* m)
{
    cli;
    while (mutex_trylock(m))
    {
        task::yield();
    }

    m->locked = true;
    m->locker = (void*)-1;          // add owning task_t*
    sti;

    return 0;
}

int mutex_trylock(mutex_t* m)
{
    cli;
    if (m->locked)
    {
        sti;
        return -1;
    }

    m->locked = true;
    m->locker = (void*)-1;
    sti;

    return 0;
}

int mutex_unlock(mutex_t* m)
{
    if (task::get_task() != m->locker)  // only the owning task can unlock
        return -1;

    cli;
    m->locked = false;
    m->locker = nullptr;
    sti;

    return 0;
}
