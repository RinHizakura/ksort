#include "sort.h"
#include <linux/sort.h>
#include <linux/workqueue.h>

int num_compare(const void *a, const void *b)
{
    return (*(int *) a - *(int *) b);
}

typedef int cmp_t(const void *, const void *);
struct common {
    int swaptype; /* Code to use for swapping */
    size_t es;    /* Element size. */
    cmp_t *cmp;   /* Comparison function */
};

struct qsort {
    struct work_struct w;
    struct common *common;
    void *a;
    size_t n;
};

static void qsort_algo(struct work_struct *w)
{
    /* TODO: Implement the sorting algorithm */
    struct qsort *q = container_of(w, struct qsort, w);

    int *a = q->a;
    size_t n = q->n;

    sort(a, n, sizeof(int), num_compare, NULL);
}

static void init_qsort(struct qsort *q, void *elems, size_t size, size_t es)
{
    INIT_WORK(&q->w, qsort_algo);
    q->a = elems;
    q->n = size / es;
}

void sort_main(void *sort_buffer, size_t size, size_t es)
{
    struct qsort q;

    init_qsort(&q, sort_buffer, size, es);

    queue_work(workqueue, &q.w);
    /* Wait until all the work are completed */
    drain_workqueue(workqueue);
}
