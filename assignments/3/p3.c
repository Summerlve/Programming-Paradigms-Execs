#include<string.h>
#include<stdlib.h>
#include<assert.h>

typedef int (*MultiTableHashFunction)(const void *keyAddr, int numBuckets);
typedef int (*MultiTableCompareFunction)(const void *keyAddr1, const void *keyAddr2);
typedef int (*MultiTableMapFunction)(void *keyAddr, void *valueAddr, void *auxData);
typedef void (*HashSetMapFunction)(void *curElem);

typedef struct {
    void *elems;
    int elemSize;
    int logiclLength;
    int allocatedLength;
} vector;

typedef struct {
    int numBuckets;
    MultiTableHashFunction hash;
    MultiTableCompareFunction compare;
    void *elems;
    int elemSize;
    int logiclLength;
    int allocatedLength;
} hashset;

typedef struct {
    hashset mappings;
    int keySize;
    int valueSize;
} multitable;

void VectorNew(vector *v, int elemSize)
{
    v->elemSize = elemSize;
    v->logiclLength = 0;
    v->allocatedLength = 4;
    v->elems = malloc(v->elemSize * v->allocatedLength);
    assert(v->elems != NULL);
}

void VectorExpand(vector *v)
{
    v->allocatedLength *= 2;
    v->elems = malloc(v->elemSize * v->allocatedLength);
    assert(v->elems != NULL);
}

int VectorLength(vector *v)
{
    return v->logiclLength;
}

void *VectorNth(vector *v, int index)
{
    return (char *)v->elems + v->elemSize * index;
}

void VectorAdd(vector *v, void *valueAddr)
{
    if (v->logiclLength == v->allocatedLength)
        VectorExpand(v);

    void *dest = VectorNth(v, VectorLength(v));
    memcpy(dest, valueAddr, v->elemSize);
    v->logiclLength++;
}


