
/**
 * *此程序在 mac 环境下可以运行
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define    MAXNITEMS        1000000
#define    MAXNTHREADS            100

#define    min(a, b)    ((a) < (b) ? (a) : (b))

int nitems;            /* read-only by producer and consumer */
struct {
    pthread_mutex_t mutex;
    int buff[MAXNITEMS];
    int nput;
    int nval;
} shared = {PTHREAD_MUTEX_INITIALIZER};

void *produce(void *), *consume(void *);

int main(int argc, char **argv) {
    int i, nthreads, count[MAXNTHREADS];
    pthread_t tid_produce[MAXNTHREADS], tid_consume;

    if (argc != 3)
        printf("usage: prodcons1 <#items> <#threads>");
    nitems = min(atoi(argv[1]), MAXNITEMS);
    nthreads = min(atoi(argv[2]), MAXNTHREADS);

    pthread_setconcurrency(nthreads);       // 告知线程系统，我们希望并发运行多少系统。 这个函数与 set_concurrency 相似。
    for (i = 0; i < nthreads; i++) {
        count[i] = 0;
        pthread_create(&tid_produce[i], NULL, produce, &count[i]);
    }

    for (i = 0; i < nthreads; i++) {
        pthread_join(tid_produce[i], NULL);
        printf("count[%d] = %d\n", i, count[i]);
    }

    pthread_create(&tid_consume, NULL, consume, NULL);
    pthread_join(tid_consume, NULL);

    exit(0);
}
/* end main */

/* include produce */
void *
produce(void *arg) {
    for (;;) {
        if (shared.nput >= nitems) {
            return (NULL);        /* array is full, we're done */
        }
        shared.buff[shared.nput] = shared.nval;
        shared.nput++;
        shared.nval++;
        *((int *) arg) += 1;
    }
}

void *
consume(void *arg) {
    int i;

    for (i = 0; i < nitems; i++) {
        if (shared.buff[i] != i)
            printf("buff[%d] = %d\n", i, shared.buff[i]);
    }
    return (NULL);
}
