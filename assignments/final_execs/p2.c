#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef void (*VectorFreeFunction)(void *v);
typedef void (*VectorMapFunction)(void *valueAddr, void *auxData);

// vector impl
typedef struct {
    void *elems;
    int elemSize;
    int logiclLength;
    int allocatedLength;
    VectorFreeFunction free;
} vector;

void VectorNew(vector *v, int elemSize, VectorFreeFunction free, int allocatedLength)
{
    v->elemSize = elemSize;
    v->logiclLength = 0;
    v->allocatedLength = allocatedLength;
    v->elems = malloc(v->elemSize * v->allocatedLength);
    v->free = free;
    assert(v->elems != NULL);
}

void VectorExpand(vector *v)
{
    if (v->allocatedLength == 0)
    { 
        v->allocatedLength += 1;
    }
    else
    {
        v->allocatedLength *= 2;
    }
    v->elems = realloc(v->elems, v->elemSize * v->allocatedLength);
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

void VectorMap(vector *v, VectorMapFunction map, void *auxData)
{
    int vLength = VectorLength(v);

    for (int i = 0; i < vLength; i++)
    {
        void *valueAddr = VectorNth(v, i);
        map(valueAddr, auxData);
    }
}

void VectorFree(vector *v)
{
    int vLength = VectorLength(v);

    for (int i = 0; i < vLength; i++)
    {
        void *valueAddr = VectorNth(v, i);
        v->free(valueAddr);
    }

    free(v->elems);
}

void NumberFree(void *valueAddr)
{
    int *numberPointer = *(int **)valueAddr;
    free(numberPointer);
}

void PrintNumber(void *valueAddr, void *auxData)
{
    int number = **(int **)valueAddr;
    printf("number is: %d\n", number);
}

bool testIfMoreThan10(const void *elemAddr)
{
    int number = **(int **)elemAddr;
    if (number > 10)
    {
        return true;
    }
    else
    {
        return false;
    }
}

typedef bool (*VectorSplitFunction)(const void *elemAddr);
void VectorSplit(vector *original, vector *thoseThatPass, vector *thoseThatFail, VectorSplitFunction test)
{
    for (int i = 0; i < VectorLength(original); i++)
    {
        void *elemAddr = VectorNth(original, i);
        if (test(elemAddr) == true)
        {
            VectorAppend(thoseThatPass, elemAddr);
        }
        else
        {
            VectorAppend(thoseThatFail, elemAddr);
        }
    }
}

int main(int argc, char **argv)
{
    vector numbers, pass, fail;
    VectorNew(&numbers, sizeof(int *), NumberFree, 0);
    VectorNew(&pass, sizeof(int *), NumberFree, 0);
    VectorNew(&fail, sizeof(int *), NumberFree, 0);

    for (int i = 0; i < 25; i++)
    {
        int *number = malloc(sizeof(int));
        *number = i;
        VectorAppend(&numbers, &number);
    }

    VectorSplit(&numbers, &pass, &fail, testIfMoreThan10);
    VectorMap(&numbers, PrintNumber, NULL);
    VectorMap(&pass, PrintNumber, NULL);
    VectorMap(&fail, PrintNumber, NULL);

    return 0;
}