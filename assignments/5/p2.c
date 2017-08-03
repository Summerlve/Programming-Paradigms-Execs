#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>

typedef struct vector {
    void *elems;
    int elemSize;
    int logicalLength;
    int allocatedSize;
    void (*VectorFreeFunction)(void *);
} vector;

typedef struct {
    char *girl;
    char *boy;
} couple;

void VectorNew(vector *v, int elemSize, void (*VectorFreeFunction)(void *), int allocatedSize)
{
    v->elemSize = elemSize;
    v->logicalLength = 0;
    v->allocatedSize = allocatedSize;
    v->VectorFreeFunction = VectorFreeFunction;
    v->elems = malloc(v->elemSize * v->allocatedSize);
}

int VectorLength(vector *v)
{
    return v->logicalLength;
}

void *VectorNth(vector *v, int index)
{
    assert(v->elems != NULL);
    assert(v->allocatedSize > index);
    void *elem = (char *)v->elems + v->elemSize * index;
    return elem;
}

void VectorExpand(vector *v)
{
    assert(v->elems != NULL);
    v->allocatedSize = (v->allocatedSize + 1) * 2;
    v->elems = realloc(v->elems, v->allocatedSize * v->elemSize);
    assert(v->elems != NULL);
}

void VectorAppend(vector *v, void *valueAddr)
{
    if (v->logicalLength == v->allocatedSize)
        VectorExpand(v);
    void *dst = (char *)v->elems + v->logicalLength * v->elemSize;
    memcpy(dst, valueAddr, v->elemSize);
    v->logicalLength++;
}

void CoupleFree(void *couple)
{

}

void StringFree(void *string)
{

}

vector generateAllCouples(vector *boys, vector *girls)
{
    vector couples;
    VectorNew(&couples, sizeof(couple), CoupleFree, 0);

    for(int i = 0; i < VectorLength(girls); i++)
    {
        for (int j = 0; j < VectorLength(boys); j++)
        {
            couple c;
            char *girl = *(char **)VectorNth(girls, i);
            c.girl = malloc(strlen(girl) + 1);
            memcpy(c.girl, girl, strlen(girl) + 1);

            char *boy = *(char **)VectorNth(boys, j);
            c.boy = malloc(strlen(boy) + 1);
            memcpy(c.boy, boy, strlen(boy) + 1);

            VectorAppend(&couples, &c);
        }
    }

    return couples;
}

int main(int argc, char **argv)
{
    vector boys, girls;
    char *boys_name[] = {"a", "b", "c"};
    char *girls_name[] = {"1", "2", "3"};
    VectorNew(&boys, sizeof(char *), StringFree, 1);
    VectorNew(&girls, sizeof(char *), StringFree, 1);

    VectorAppend(&boys, &boys_name[0]);
    VectorAppend(&boys, &boys_name[1]);
    VectorAppend(&boys, &boys_name[2]);
    printf("boys' length: %d\n", VectorLength(&boys));

    VectorAppend(&girls, &girls_name[0]);
    VectorAppend(&girls, &girls_name[1]);
    VectorAppend(&girls, &girls_name[2]);
    printf("girl's length: %d\n", VectorLength(&girls));

    vector result;
    VectorNew(&result, sizeof(couple), CoupleFree, 1);
    result = generateAllCouples(&boys, &girls);
    printf("result's length: %d\n", VectorLength(&result));

    for (int i = 0; i < VectorLength(&result); i++)
    {
        couple *c = VectorNth(&result, i);
        printf("girl: %s, ", c->girl);
        printf("boy: %s\n", c->boy);
    }

    return 0;
}
