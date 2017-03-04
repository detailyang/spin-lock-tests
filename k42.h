#ifndef __SPINLOCK_K42_H__
#define __SPINLOCK_K42_H__

/* Compile read-write barrier */
#define barrier() asm volatile("": : :"memory")

/* Pause instruction to prevent excess processor bus usage */
#define cpu_relax() asm volatile("pause\n": : :"memory")

#define cmpxchg(P, O, N) __sync_val_compare_and_swap((P), (O), (N))

#define SPINLOCK_INITIALIZER { 0, 0 };

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

typedef struct k42lock k42lock;
struct k42lock
{
    k42lock *next;
    k42lock *tail;
};

static inline void k42_lock(k42lock *l)
{
    k42lock me;
    k42lock *pred, *succ;
    me.next = NULL;

    barrier();

    pred = xchg_64(&l->tail, &me);
    if (pred)
    {
        me.tail = (void *) 1;

        barrier();
        pred->next = &me;
        barrier();

        while (me.tail) cpu_relax();
    }

    succ = me.next;

    if (!succ)
    {
        barrier();
        l->next = NULL;

        if (cmpxchg(&l->tail, &me, &l->next) != &me)
        {
            while (!me.next) cpu_relax();

            l->next = me.next;
        }
    }
    else
    {
        l->next = succ;
    }
}

static inline void k42_unlock(k42lock *l)
{
    k42lock *succ = l->next;

    barrier();

    if (!succ)
    {
        if (cmpxchg(&l->tail, &l->next, NULL) == (void *) &l->next) return;

        while (!l->next) cpu_relax();
        succ = l->next;
    }

    succ->tail = NULL;
}

#endif
