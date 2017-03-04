#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#ifdef XCHG
#include "xchg.h"
#elif defined(CAS)
#include "cas.h"
#elif defined(XCHG_BACKOFF)
#include "xchg-backoff.h"
#elif defined(MCS)
#include "mcs.h"
mcs_lock cnt_lock = NULL;
#define spin_lock(lock) lock_mcs(&cnt_lock, lock)
#define spin_unlock(lock) unlock_mcs(&cnt_lock, lock);
#elif defined(K42)
#include "k42.h"
#define spin_lock k42_lock
#define spin_unlock k42_unlock
#endif

#define MAX_LOOP 20000000

#ifdef K42
k42lock lock;
#else
unsigned lock;
#endif

int counter = 0 ;

void *
thread(void *args)
{
#ifdef MCS
    mcs_lock_t lock;
#endif
	int i = 0;
	spin_lock(&lock);
	for (i = 0; i < MAX_LOOP; i++) {
		counter++;
	}
	spin_unlock(&lock);
}

int main(int argc, char *argv[]) {
    int            nthread;
    int            i;
    pthread_t     *tids;
    struct timeval start, end;

    if (argc < 2) {
        printf("Please specify the number of threads\n");
        return 1;
    }

    nthread = atoi(argv[1]);
    tids = malloc(nthread * sizeof(pthread_t));
    if (tids == NULL) {
        printf("Out of memory\n");
        return 1;
    }

    gettimeofday(&start, NULL);

    for (i = 0; i < nthread; i++) {
        pthread_create(&tids[i], NULL, thread, NULL);
    }

    for (i = 0; i < nthread; i++) {
        pthread_join(tids[i], NULL);
    }

    gettimeofday(&end, NULL);
    printf("%06f", (end.tv_sec + end.tv_usec / 1000000.0) - ( start.tv_sec + start.tv_usec/ 1000000.0));

    return 0;
}
