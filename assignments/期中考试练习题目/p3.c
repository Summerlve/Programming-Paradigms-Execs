#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

typedef int (*MultiSetHashFunction)(const void *elem, int numBuckets);
typedef int (*MultiSetCompareFunction)(const void *elem1, const void *elem2);
typedef void (*MultiSetMapFunction)(void *elem, int count, void *auxData);
typedef void (*MultiSetFreeFunction)(void *elem);
typedef void (*HashSetMapFunction)(void *elem);

typedef struct {
    int numBuckets;
    MultiSetHashFunction hash;
    MultiSetCompareFunction compare;
    MultiSetFreeFunction free;
    void *elems;
    int elemSize;
    int logiclLength;
    int allocatedLength;
} hashset;

typedef struct {
    hashset elements;
    int elemSize;
    MultiSetFreeFunction free;
} multiset;

// hashset abouts func
void HashSetNew(hashset *hs, int numBuckets,
                MultiSetHashFunction hash, MultiSetCompareFunction compare, MultiSetFreeFunction free, int elemSize)
{
    hs->numBuckets = numBuckets;
    hs->hash = hash;
    hs->compare = compare;
    hs->free = free;
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

void HashSetDispose(hashset *hs)
{
    HashSetMap(hs, hs->free);
    free(hs->elems);
}

/*multiset abouts func*/
void MultiSetNew(multiset *ms, int elemSize, int numBuckets,
                 MultiSetHashFunction hash, MultiSetCompareFunction compare, MultiSetFreeFunction free)
{
    ms->elemSize = elemSize;
    ms->free = free;
    HashSetNew(&ms->elements, numBuckets, hash, compare, free, ms->elemSize + sizeof(int));
}

void MultiSetDispose(multiset *ms)
{
    HashSetDispose(&ms->elements);
}

void MultiSetEnter(multiset *ms, const void *key)
{
    hashset *hs = &ms->elements;
    int compare = -1;
    void *cur;

    for (int i = 0; i < HashSetLength(hs); i++)
    {
        cur = HashSetNth(hs, i);
        compare = hs->compare(cur, key);

        if (compare == 0) break;
    }

    printf("compare is: %d\n", compare);

    if (compare != 0)
    {
        void *elem = malloc(hs->elemSize);
        memcpy(elem, key, ms->elemSize);

        printf("key: %d\n", *(int *)key);
        *(int *)((char *)elem + ms->elemSize) = 1;
        HashSetAppend(hs, elem);
        printf("hs length: %d\n", HashSetLength(hs));
        printf("hs alloclength: %d\n\n", hs->allocatedLength);
    }

    if (compare == 0)
    {
        if (ms->free != NULL)
        {
            printf("old key needs free\n");
            ms->free(cur);
        }

        memcpy(cur, key, ms->elemSize);
        int count = *(int *)((char *)cur + ms->elemSize) + 1;
        *(int *)((char *)cur + ms->elemSize) = count;
    }
}

void MultiSetMap(multiset *ms, MultiSetMapFunction map, void *auxData)
{
    hashset *hs = &ms->elements;
    for (int i = 0; i < HashSetLength(hs); i++)
    {
        void *cur = HashSetNth(hs, i);
        int count = *(int *)((char *)cur + ms->elemSize);
        map(cur, count, auxData);
    }
}

int hash(const void *elem, int numBuckets)
{
    return 1;
}

int compare(const void *elem1, const void *elem2)
{
    if (*(char *)elem1 == *(char *)elem2)
        return 0;
    return -1;
}

void IntFree(void *elem)
{
    printf("Free: %d\n", *(int *)elem);
}

void map(void *elem, int count, void *auxData)
{
    printf("Map: %d count: %d\n", *(int *)elem, count);
    (*(int *)auxData) += count;
}

int main(int argc, char **argv)
{
    multiset ms;
    MultiSetNew(&ms, sizeof(int), 1, hash, compare, IntFree);

    int value11 = 101;
    int value12 = 200;
    int value13 = 300;
    int value14 = 300;

    MultiSetEnter(&ms, &value11);
    MultiSetEnter(&ms, &value12);
    MultiSetEnter(&ms, &value13);
    MultiSetEnter(&ms, &value14);

    int auxData = 0;
    MultiSetMap(&ms, map, &auxData);
    printf("auxData: %d\n", auxData);

    MultiSetDispose(&ms);

    return 0;
}
