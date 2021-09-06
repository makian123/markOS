#pragma once

#include <include/types.hpp>

#define DECLARE_LOCK(name) volatile int name##Locked

#define LOCK(name)                                             \
	while (!__sync_bool_compare_and_swap(&name##Locked, 0, 1)) \
		;

#define UNLOCK(name) __sync_lock_release(&name##Locked);