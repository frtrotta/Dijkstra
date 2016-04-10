#ifndef HEAPPQ_H
#define HEAPPQ_H

#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

#ifdef __cplusplus
extern "C" {
#endif

    struct s_heappq {
        vertex **elements;
        int size;
        int last;
    };

    typedef struct s_heappq heappq;

    heappq * heapInit(int initialSize);
    int heapIsEmpty(heappq *h);
    int heapAdd(heappq *h, vertex *e);
    vertex * heapRemoveMin(heappq *h);
    int restoreHeap(heappq *h, int position);

#ifdef __cplusplus
}
#endif

#endif /* HEAP_H */

