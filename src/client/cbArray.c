#include <stdio.h>
#include <stdlib.h>
#include "include/cbArray.h"

void cbInit(CircularBuffer *cb,ElemType *e, int size) {
    int i = 0;
    cb->size  = size + 1; /* include empty elem */
    cb->start = 0;
    cb->end   = 0;
    cb->elems = e;
    for (i=0; i< size; ++i)
        cb->elems[i].value = 0.0;
}

void cbFree(CircularBuffer *cb) {
    cb->elems = NULL; /* OK if null */
}

int cbIsFull(CircularBuffer *cb) {
    return (cb->end + 1) % cb->size == cb->start;
}

int cbIsEmpty(CircularBuffer *cb) {
    return cb->end == cb->start;
}

/* Write an element, overwriting oldest element if buffer is full. App can
 *    choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, ElemType *elem) {
    cb->elems[cb->end] = *elem;
    cb->end = (cb->end + 1) % cb->size;
    if (cb->end == cb->start)
        cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
}

/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, ElemType *elem) {
    *elem = cb->elems[cb->start];
    cb->start = (cb->start + 1) % cb->size;
}
void cbPeep(CircularBuffer *cb, ElemType *elem) {
    *elem = cb->elems[cb->start];
}

