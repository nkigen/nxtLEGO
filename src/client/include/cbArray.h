#ifndef __CBARRAY_H__
#define __CBARRAY_H__
/* Opaque buffer element type.  This would be defined by the application. */
#define DER_BUFFER	10

typedef struct {
    float value;
} ElemType;

/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         end;    /* index at which to write new element  */
    ElemType   *elems;  /* vector of elements                   */
} CircularBuffer;

void cbInit(CircularBuffer *cb,ElemType *e, int size);
void cbFree(CircularBuffer *cb);
int cbIsFull(CircularBuffer *cb);
int cbIsEmpty(CircularBuffer *cb);
void cbWrite(CircularBuffer *cb, ElemType *elem);
void cbRead(CircularBuffer *cb, ElemType *elem);
void cbPeep(CircularBuffer *cb, ElemType *elem);
#endif
