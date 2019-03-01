#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef void (*ElemFreeFunction)(void *v);
typedef void (*VectorMapFunction)(void *valueAddr, void *auxData);

// vector impl
typedef struct {
    void *elems;
    int elemSize;
    int logiclLength;
    int allocatedLength;
    ElemFreeFunction elemFreeFn;
} vector;

void VectorNew(vector *v, int elemSize, ElemFreeFunction elemFreeFn, int allocatedLength)
{
    v->elemSize = elemSize;
    v->logiclLength = 0;
    v->allocatedLength = allocatedLength;
    v->elems = malloc(v->elemSize * v->allocatedLength);
    v->elemFreeFn = elemFreeFn;
    assert(v->elems != NULL);
}

void VectorExpand(vector *v)
{
    v->allocatedLength *= 2;
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

void VectorMap(vector *v, VectorMapFunction map, void *auxData) {
    int vLength = VectorLength(v);

    for (int i = 0; i < vLength; i++) {
        void *valueAddr = VectorNth(v, i);
        map(valueAddr, auxData);
    }
}

void VectorFree(vector *v) {
    int vLength = VectorLength(v);

    for (int i = 0; i < vLength; i++) {
        void *valueAddr = VectorNth(v, i);
        v->elemFreeFn(valueAddr);
    }

    free(v->elems);
    free(v);
}

typedef struct {
    char *girl;
    char *boy;
} couple;

void PrintCouple(void *valueAddr, void *auxData) {
    char *girl = *(char **)valueAddr;
    char *boy = *((char **)valueAddr + 1); 
    printf("couple: (%s, %s)\n", girl, boy);
}

void StringFree(void *v) {
    char *name = *(char **)v;
    printf("name is: %s\n", name);
    free(name);
}

void CoupleFree(void *v) {
    char *girl = *(char **)v;
    char *boy = *((char **)v + 1); 

    printf("freeing couple: (%s, %s)\n", girl, boy);
    
    free(girl);
    free(boy);
}

vector generateAllCouples(vector *boys, vector *girls) {
    vector couples;
    VectorNew(&couples, sizeof(couple), CoupleFree, 0);

    int boysLength = VectorLength(boys);
    int girlsLength = VectorLength(girls);

    for (int i = 0; i < girlsLength; i++) {
        char *girl = *(char **)(VectorNth(girls, i));
        for (int j = 0; j < boysLength; j++) {
            char *boy = *(char **)(VectorNth(boys, j));
            couple c;
            c.girl = malloc(strlen(girl) + 1);
            strcpy(c.girl, girl);
            c.boy = malloc(strlen(boy) + 1);
            strcpy(c.boy, boy);
            VectorAppend(&couples, &c);
        }
    }
    
    return couples; 
}

int main(int argc, char **argv) {
    vector boys, girls;
    VectorNew(&boys, sizeof(char *), StringFree, 0);
    VectorNew(&girls, sizeof(char *), StringFree, 0);

    const char *boy_names[] = {
        "boys_1",
        "boys_2",
        "boys_3",
        "boys_4"
    };  
    const char *girl_names[] = {
        "girl_1",
        "girl_2",
        "girl_3",
        "girl_4"
    };
    
    for (int i = 0; i < 4; i++) {
        char *boy = malloc(strlen(boy_names[i]) + 1);
        memcpy(boy, boy_names[i], strlen(boy_names[i]) + 1);
        VectorAppend(&boys, boy);
        char *girl = malloc(strlen(girl_names[i]) + 1);
        memcpy(girl, girl_names[i], strlen(girl_names[i]) + 1);
        VectorAppend(&girls, girl);
    }

    vector couples = generateAllCouples(&boys, &girls); 
    VectorMap(&couples, PrintCouple, NULL);

    VectorFree(&boys);
    VectorFree(&girls);
    VectorFree(&couples);

    return 0;
}