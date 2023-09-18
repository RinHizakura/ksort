#include "sort.h"
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

static struct qsort qsort_obj;

static void qsort_algo(struct work_struct *w)
{
    /* TODO: Implement the sorting algorithm */
    struct qsort *q = container_of(w, struct qsort, w);
}

static void init_qsort(struct qsort *q, void *elems, size_t nelem)
{
    INIT_WORK(&q->w, qsort_algo);
    q->a = elems;
    q->n = nelem;
}

void sort_main(void *sort_buffer, size_t size)
{
    /* TODO: Assume the sorting object always becomes an integer array,
     * we may want to allow other types in the future. */
    init_qsort(&qsort_obj, sort_buffer, size);

    queue_work(workqueue, &qsort_obj.w);
    /* Wait until all the work are completed */
    drain_workqueue(workqueue);
}
