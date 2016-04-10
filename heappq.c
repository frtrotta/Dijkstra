#include "heappq.h"

heappq * heapInit(int initialSize) {
    heappq *r;
    r = (heappq *) malloc(sizeof (heappq));
    if (r) {
        r->elements = (int *) malloc(initialSize * sizeof (vertex *));
        if (r->elements) {
            r->size = initialSize;
            r->last = 0;
        } else {
            free(r);
            r = NULL;
        }
    }
    return r;
}

int heapIsEmpty(heappq *h) {
    return h->last == 0;
}

int upHeapBubbling(heappq* h, int position) {
    int current = position;
    int parent = -1;

    if (current % 2) {
        parent = (current - 1) / 2;
    } else {
        parent = (current) / 2;
    }

    while (parent > 0) {
        vertex * temp = NULL;
        if (h->elements[current]->d < h->elements[parent]->d) {
            temp = h->elements[current];
            h->elements[current] = h->elements[parent];
            h->elements[parent] = temp;
            current = parent;
            if (current % 2) {
                parent = (current - 1) / 2;
            } else {
                parent = (current) / 2;
            }
        } else {
            h->elements[current]->position = current;
            parent = 0;
        }
    }
    return current;
}

int heapAdd(heappq *h, vertex *e) {
    int r = -1;
    if (h->last < (h->size - 1)) {
        h->last++;
        h->elements[h->last] = e;

        r = upHeapBubbling(h, h->last);
    } else {
        // increase heap
        int newSize = h->size * 2;
        vertex **newElements = (int *) realloc(h->elements, newSize * sizeof (vertex *));
        if (newElements) {
            h->size = newSize;
            h->elements = newElements;
        } else {
            //
            fprintf(stderr, "No room for the new element");
            r = -1;
        }
        r = heapAdd(h, e);
    }
    return r;
}

int downHeapBubbling(heappq *h, int position) {
    int current = position;
    int left;
    int right;

    //down-heap bubbling
    while (1) {
        left = 2 * current;
        right = 2 * current + 1;
        if (left > h->last && right > h->last) {
            // It has no child
            break;
        } else {
            if (right > h->last) {
                // It only has the left child
                if (h->elements[left]->d < h->elements[current]->d) {
                    vertex *temp = h->elements[left];
                    h->elements[left] = h->elements[current];
                    h->elements[left]->position = current;
                    h->elements[current] = temp;
                    h->elements[current]->position = left;
                    current = left;
                } else {
                    break;
                }
            } else {
                // It has both the left and the right child
                int cmp = left;
                if (h->elements[cmp] > h->elements[right]) {
                    cmp = right;
                }
                if (h->elements[current] > h->elements[cmp]) {
                    vertex *temp = h->elements[current];
                    h->elements[current] = h->elements[cmp];
                    h->elements[current]->position = cmp;
                    h->elements[cmp] = temp;
                    h->elements[cmp]->position = current;
                    current = cmp;
                } else {
                    break;
                }
            }
        }
    }
    return current;
}

vertex * heapRemoveMin(heappq *h) {
    vertex *r = NULL;
    if (!heapIsEmpty(h)) {
        r = h->elements[1]; // position 0 is never used
        h->elements[1] = h->elements[h->last];
        h->last--;
        downHeapBubbling(h, 1);
    }
    return r;
}

/**
 * The d of the element in that position is changed and the heap need top be restored.
 * @param h
 * @param position
 * @return 
 */
int restoreHeap(heappq *h, int position) {
    int r = -1;
    r = upHeapBubbling(h, position);
    r = downHeapBubbling(h, r);
    return r;
}