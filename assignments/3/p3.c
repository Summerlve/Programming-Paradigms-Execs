#include<string.h>
#include<stdlib.h>
#include<assert.h>

typedef int (*MultiTableHashFunction)(const void *keyAddr, int numBuckets);
typedef int (*MultiTableCompareFunction)(const void *keyAddr1, const void *keyAddr2);
typedef int (*MultiTableMapFunction)(void *keyAddr, void *valueAddr, void *auxData);
typedef void (*HashSetMapFunction)(void *curElemAddr);

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

void VectorAppend(vector *v, const void *valueAddr)
{
    if (v->logiclLength == v->allocatedLength)
        VectorExpand(v);

    void *dest = VectorNth(v, VectorLength(v));
    memcpy(dest, valueAddr, v->elemSize);
    v->logiclLength++;
}

void HashSetNew(hashset *hs, int numBuckets, MultiTableHashFunction hash, MultiTableCompareFunction compare, int elemSize)
{
    hs->numBuckets = numBuckets;
    hs->hash = hash;
    hs->compare = compare;
    hs->elemSize = elemSize;
    hs->logiclLength = 0;
    hs->allocatedLength = 4;
    hs->elems = malloc(hs->elemSize * hs->allocatedLength);
    assert(hs->elems != NULL);
}

void HashSetExpend(hashset *hs)
{
    hs->allocatedLength *= 2;
    hs->elems = realloc(hs->elems, hs->elemSize * hs->allocatedLength);
    assert(hs->elems != NULL);
}

int HashSetLength(hashset *hs)
{
    return hs->logiclLength;
}

void *HashSetNth(hashset *hs, int index)
{
    return (char *)hs->elems + index * hs->elemSize;
}

void HashSetAppend(hashset *hs, void *valueAddr)
{
    if (hs->logiclLength == hs->allocatedLength)
        HashSetExpend(hs);

    void *dest = HashSetNth(hs, HashSetLength(hs));
    memcpy(dest, valueAddr, hs->elemSize);
    hs->logiclLength++;
}

void HashSetMap(hashset *hs, HashSetMapFunction map)
{
    for (int i = 0; i < HashSetLength(hs); i++)
    {
        void *curElemAddr = HashSetNth(hs, i);
        map(curElemAddr);
    }
}

void MultiTableNew(multitable *mt, int keySizeInBytes, int valueSizeInBytes, int numBuckets, MultiTableHashFunction hash, MultiTableCompareFunction compare)
{
    int elemSize = keySizeInBytes + sizeof(vector);
    HashSetNew(&mt->mappings, numBuckets, hash, compare, elemSize);
    mt->keySize = keySizeInBytes;
    mt->valueSize = valueSizeInBytes;
}

void MultiTableEnter(multitable *mt, const void *keyAddr, const void *valueAddr)
{
    hashset *hs = &mt->mappings;
    int compare;
    void *curElemAddr;

    for (int i = 0; i < HashSetLength(hs); i++)
    {
        curElemAddr = HashSetNth(hs, i);
        compare = memcmp(curElemAddr, keyAddr, mt->keySize);

        if (compare == 0) break;
    }

    if (compare != 0)
    {
        vector v;
        VectorNew(&v, mt->valueSize);
        VectorAppend(&v, valueAddr);

        void *value = malloc(hs->elemSize);
        memcpy(value, keyAddr, mt->keySize);
        memcpy((char *)value + mt->keySize, valueAddr, mt->valueSize);
        HashSetAppend(hs, value);
    }

    if (compare == 0)
    {
        vector *v = (vector *)((char *)curElemAddr + mt->keySize);
        VectorAppend(v, valueAddr);
    }
}


void MultiTableMap(multitable *mt, MultiTableMapFunction map, void *auxData)
{

}

int main(void)
{
   return 0;
}
