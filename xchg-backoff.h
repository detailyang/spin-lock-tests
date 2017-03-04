#ifndef __SPINLOCK_XCHG_BACKOFF_H__
#define __SPINLOCK_XCHG_BACKOFF_H__

/* Compile read-write barrier */
#define barrier() asm volatile("": : :"memory")

/* Pause instruction to prevent excess processor bus usage */
#define cpu_relax() asm volatile("pause\n": : :"memory")

/* Atomic exchange (of various sizes) */
static inline void *xchg_64(void *ptr, void *x)
{
    __asm__ __volatile__("xchgq %0,%1"
                :"=r" ((unsigned long long) x)
                :"m" (*(volatile long long *)ptr), "0" ((unsigned long long) x)
                :"memory");

    return x;
}

static inline unsigned xchg_32(void *ptr, unsigned x)
{
    __asm__ __volatile__("xchgl %0,%1"
                :"=r" ((unsigned) x)
                :"m" (*(volatile unsigned *)ptr), "0" (x)
                :"memory");

    return x;
}

static inline unsigned short xchg_16(void *ptr, unsigned short x)
{
    __asm__ __volatile__("xchgw %0,%1"
                :"=r" ((unsigned short) x)
                :"m" (*(volatile unsigned short *)ptr), "0" (x)
                :"memory");

    return x;
}

/* Test and set a bit */
static inline char atomic_bitsetandtest(void *ptr, int x)
{
    char out;
    __asm__ __volatile__("lock; bts %2,%1\n"
                        "sbb %0,%0\n"
                :"=r" (out), "=m" (*(volatile long long *)ptr)
                :"Ir" (x)
                :"memory");

    return out;
}


#define EBUSY 1
typedef unsigned spinlock;

static void spin_lock(spinlock *lock)
{
    int spin = 1;
    int i = 0;

    for(;;) {
        if (!xchg_32(lock, EBUSY)) {
            return;
        }

        while (*lock) {
            spin <<= 1;
            for (i = 0; i < spin; i++) {
                cpu_relax();
            }
        }
    }
}

static void spin_unlock(spinlock *lock)
{
    barrier();
    *lock = 0;
}

#endif
