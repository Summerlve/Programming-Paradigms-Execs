#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>
#include<stdio.h>

typedef struct {
    void *elems;
    int elemSize;
    int allocatedSize;
} vector;

typedef struct {
    bool *bitmap;
    vector strings;
} group;

typedef struct {
    group *groups;
    int numGroups;
    int arrayLength;
    int groupSize;
} sparsestringarray;

void VectorNew(vector *v, int elemSize, int allocatedSize)
{
    v->elemSize = elemSize;
    v->allocatedSize = allocatedSize;
    v->elems = malloc(v->elemSize * v->allocatedSize);
}

typedef void (*DisFn)(void *);

void VectorDispose(vector *v, DisFn disfn)
{
    for (int i = 0; i < v->allocatedSize; i++)
    {
        void *curElem = (char *)v->elems + i * v->elemSize;
        disfn(curElem);
    }

    free(v->elems);
}

void *VectorCurPtr(vector *v, int index)
{
    assert(v->allocatedSize > index);
    void *elem = (char *)v->elems + index * v->elemSize;
    return elem;
}

void VectorInsert(vector *v, int index, void *valuePtr)
{
    assert(v->allocatedSize > index);
    void *elem = VectorCurPtr(v, index);
    memcpy(elem, valuePtr, v->elemSize);
 }

void VectorReplace(vector *v, int index, void *valuePtr, DisFn disfn)
{
    assert(v->allocatedSize > index);
    void *elem = VectorCurPtr(v, index);
    disfn(elem);
    memcpy(elem, valuePtr, v->elemSize);
}

void SSANew(sparsestringarray *ssa, int arrayLength, int groupSize)
{
    int numGroups = arrayLength / groupSize;
    ssa->numGroups = numGroups;
    ssa->arrayLength = arrayLength;
    ssa->groupSize = groupSize;
    ssa->groups = malloc(ssa->numGroups * sizeof(group));
    assert(ssa->groups != NULL);

    for (int i = 0; i < ssa->numGroups; i++)
    {
        group *curGroup = &ssa->groups[i];
        VectorNew(&(curGroup->strings), sizeof(char *), ssa->groupSize);
        curGroup->bitmap = malloc(sizeof(bool) * ssa->groupSize);
        assert(curGroup->bitmap != NULL);

        bool *curBitmap = curGroup->bitmap;
        for (int j = 0; j < ssa->groupSize; j++)
        {
            curBitmap[j] = false;
            char *emptyString = strdup("");
            VectorInsert(&(curGroup->strings), j, &emptyString);
        }
    }
}

void stringDispose(void *string)
{
    free(*(char **)string);
}

void SSADispose(sparsestringarray *ssa)
{
    for (int i = 0; i < ssa->numGroups; i++)
    {
        bool *curBitmap = ssa->groups[i].bitmap;
        free(curBitmap);
        VectorDispose(&(ssa->groups[i].strings), stringDispose);
    }
}

bool SSAInsert(sparsestringarray *ssa, int index, const char *str)
{
    int groupIndex = index / ssa->groupSize;
    int bitmapIndex = index % ssa->groupSize;

    group *curGroup = ssa->groups + groupIndex;
    bool *curBitmap = curGroup->bitmap;

    bool inserted = curBitmap[bitmapIndex];

    if (inserted == true)
    {
        char *strc = strdup(str);
        VectorReplace(&(curGroup->strings), bitmapIndex, &strc, stringDispose);
        return false;
    }
    else
    {
        char *strc = strdup(str);
        VectorInsert(&(curGroup->strings), bitmapIndex, &strc);
        curBitmap[bitmapIndex] = true;
        return true;
    }
}

typedef void (*SSAMapFunction)(int index, const char *str, void *auxData);

void SSAMap(sparsestringarray *ssa, SSAMapFunction mapfn, void *auxData)
{
    for (int i = 0; i < ssa->numGroups; i++)
    {
        group *curGroup = &ssa->groups[i];

        for (int j = 0; j < ssa->groupSize; j++)
        {
            void *elem = VectorCurPtr(&(curGroup->strings), j);
            int index = i * ssa->groupSize + j;
            mapfn(index, *(char **)elem, auxData);
        }
    }
}

static void CountEmptyPrintNonEmpty(int index, const char *str, void *auxData)
{
    if (strcmp(str, "") != 0)
    {
        printf("Oooo! Nonempty string at index %d: \"%s\"\n", index, str);
    }
    else
    {
        (* (int *) auxData)++;
    }
}

int main(void)
{
    sparsestringarray ssa;
    SSANew(&ssa, 70000, 35);

    SSAInsert(&ssa, 33001, "need");
    SSAInsert(&ssa, 58291, "nore");
    SSAInsert(&ssa, 33000, "Eye");
    SSAInsert(&ssa, 33000, "I");
    SSAInsert(&ssa, 67899, "cowbell");

    int numEmptyStrings = 0;
    SSAMap(&ssa, CountEmptyPrintNonEmpty, &numEmptyStrings);
    printf("%d of the strings were empty strings.\n", numEmptyStrings);

    SSADispose(&ssa);
    return 0;
}
