#ifndef KSORT_H
#define KSORT_H

#include <linux/types.h>

extern struct workqueue_struct *workqueue;

void sort_main(void *sort_buffer, size_t size, size_t es);
#endif
