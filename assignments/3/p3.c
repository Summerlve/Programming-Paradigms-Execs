#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>

typedef int (*MultiTableHashFunction)(const void *keyAddr, int numBuckets);
typedef int (*MultiTableCompareFunction)(const void *keyAddr1, const void *keyAddr2);
typedef void (*MultiTableMapFunction)(const void *keyAddr, void *valueAddr, void *auxData);
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
        compare = hs->compare(curElemAddr, keyAddr);

        if (compare == 0) break;
    }

    if (compare != 0)
    {
        vector v;
        VectorNew(&v, mt->valueSize);
        VectorAppend(&v, valueAddr);

        void *value = malloc(hs->elemSize);
        memcpy(value, keyAddr, mt->keySize);
        memcpy((char *)value + mt->keySize, &v, sizeof(vector));
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
    hashset *hs = &mt->mappings;
    for (int i = 0; i < HashSetLength(hs); i++)
    {
        void *curElem = HashSetNth(hs, i);
        void *keyAddr = curElem;
        vector *v = (vector *)((char *)curElem + mt->keySize);

        for (int j = 0; j < VectorLength(v); j++)
        {
            void *valueAddr = VectorNth(v, j);
            map(keyAddr, valueAddr, auxData);
        }
    }
}

int hash(const void *keyAddr, int numBuckets)
{
    return 1;
}

int compare(const void *keyAddr1, const void *keyAddr2)
{
    int key1, key2;
    memcpy(&key1, keyAddr1, sizeof(int));
    memcpy(&key2, keyAddr2, sizeof(int));

    return key1 - key2;
}

void map(const void *keyAddr, void *valueAddr, void *auxData)
{
    int key;
    memcpy(&key, keyAddr, sizeof(int));

    char value;
    memcpy(&value, valueAddr, sizeof(char));

    printf("auxData is: %d\n", *(int *)auxData);
    printf("key is: %d\n", key);
    printf("value is: %c\n", value);
    (*(int *)auxData)++;
}

int main(void)
{
    multitable mt;
    MultiTableNew(&mt, sizeof(int), sizeof(char), sizeof(int), hash, compare);

    int key1 = 1;
    char value11 = 'a';
    char value12 = 'b';
    char value13 = 'c';

    int key2 = 2;
    char value21 = 'd';
    char value22 = 'e';
    char value23 = 'f';

    MultiTableEnter(&mt, &key1, &value11);
    MultiTableEnter(&mt, &key1, &value12);
    MultiTableEnter(&mt, &key1, &value13);
    MultiTableEnter(&mt, &key2, &value21);
    MultiTableEnter(&mt, &key2, &value22);
    MultiTableEnter(&mt, &key2, &value23);

    int auxData = 1;
    MultiTableMap(&mt, map, &auxData);
    return 0;
}
