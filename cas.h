#ifndef __SPINLOCK_CAS_H__
#define __SPINLOCK_CAS_H__


/* Compile read-write barrier */
#define barrier() asm volatile("": : :"memory")

/* Pause instruction to prevent excess processor bus usage */
#define cpu_relax() asm volatile("pause\n": : :"memory")


static char
cmpxchg(unsigned *lock, unsigned old, unsigned set)
{
    char  res;

    __asm__ volatile (
    "    lock;              "
    "    cmpxchg  %3, %1;   "
    "    sete      %0;      "

    : "=a" (res) : "m" (*lock), "a" (old), "r" (set) : "cc", "memory");

    return res;
}


typedef unsigned spinlock;

static void spin_lock(spinlock *lock)
{
    for(;;) {
        if (*lock == 0 && cmpxchg(lock, 0, 1)) return;

        while (*lock) cpu_relax();
    }
}

static void spin_unlock(spinlock *lock)
{
    barrier();
    *lock = 0;
}

#endif
