#ifndef __SPINLOCK_MCS_H__
#define __SPINLOCK_MCS_H__

/* Compile read-write barrier */
#define barrier() asm volatile("": : :"memory")

/* Pause instruction to prevent excess processor bus usage */
#define cpu_relax() asm volatile("pause\n": : :"memory")

#define cmpxchg(P, O, N) __sync_val_compare_and_swap((P), (O), (N))

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


typedef struct mcs_lock_t mcs_lock_t;
struct mcs_lock_t
{
    mcs_lock_t *next;
    int spin;
};
typedef struct mcs_lock_t *mcs_lock;

static inline void lock_mcs(mcs_lock *m, mcs_lock_t *me)
{
    mcs_lock_t *tail;

    me->next = NULL;
    me->spin = 0;

    tail = xchg_64(m, me);

    /* No one there? */
    if (!tail) return;

    /* Someone there, need to link in */
    tail->next = me;

    /* Make sure we do the above setting of next. */
    barrier();

    /* Spin on my spin variable */
    while (!me->spin) cpu_relax();

    return;
}

static inline void unlock_mcs(mcs_lock *m, mcs_lock_t *me)
{
    /* No successor yet? */
    if (!me->next)
    {
        /* Try to atomically unlock */
        if (cmpxchg(m, me, NULL) == me) return;

        /* Wait for successor to appear */
        while (!me->next) cpu_relax();
    }

    /* Unlock next one */
    me->next->spin = 1;
}

#endif
