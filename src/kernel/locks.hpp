#ifndef _LOCKS_HPP
#define _LOCKS_HPP

#include "kernel.hpp"
#include "../include/locktype.h"

bool lock_blockcheck(void *p);

void init_lock(lock &l);
void take_lock_exclusive(lock &l, uint64_t thread = sch_get_id());
void take_lock_recursive(lock &l, uint64_t thread = sch_get_id());
bool try_take_lock_exclusive(lock &l, uint64_t thread = sch_get_id());
void release_lock(lock &l, uint64_t thread=sch_get_id());


class hold_lock{
private:
	lock *l;

public:
	hold_lock(lock &lck, bool exclusive=true): l(&lck) {
        if(exclusive) take_lock_exclusive(*l);
        else take_lock_recursive(*l);
    }
	~hold_lock() { release_lock(*l);}
};

#endif
