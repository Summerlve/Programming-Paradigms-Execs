#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

typedef struct {
    int *root;
    int elemSize;
    int logicalSize;
    int allocatedSize;
    int (*cmpfn)(const void *, const void *);
} sortedset;

#define NodeSize(clientSize) (clientSize + 2 * sizeof(int))

int *FindNode(sortedset *set, const void *elemPtr)
{
    void *curr;
    int comp, *root = set->root;
    while(*root != -1)
    {
        curr = (char *)(set->root + 1) + *(root) * NodeSize(set->elemSize);
        comp = set->cmpfn(elemPtr, curr);

        if (comp == 0) break;

        root = (int *)((char *)curr + set->elemSize);
        if (comp > 0) root++;
    }

    return root;
}

void SetNew(sortedset *set, int elemSize, int(*cmpfn)(const void *, const void *))
{
    assert(elemSize > 0);
    assert(cmpfn != NULL);

    set->allocatedSize = 4;
    set->root = malloc(sizeof(int) + set->allocatedSize * NodeSize(elemSize));
    assert(set->root != NULL);

    *set->root = -1;
    set->logicalSize = 0;
    set->cmpfn = cmpfn;
    set->elemSize = elemSize;
}

void *SetSearch(sortedset *set, const void *elemPtr)
{
    int *node = FindNode(set, elemPtr);
    if (*node == -1) return NULL;
    return (char *)(set->root + 1) + *node * NodeSize(set->elemSize);
}

void SetExpand(sortedset *set)
{
    set->allocatedSize *= 2;
    set->root = realloc(set->root, sizeof(int) + set->allocatedSize * NodeSize(set->elemSize));
    assert(set->root != NULL);
}

bool SetAdd(sortedset *set, const void *elemPtr)
{
    int *child;
    void *dest;

    child = FindNode(set, elemPtr);
    if (*child != -1) return false;

    if (set->logicalSize == set->allocatedSize) SetExpand(set);
    *child = set->logicalSize++;
    dest = (char *)(set->root + 1) + (*child) * NodeSize(set->elemSize);
    memcpy(dest, elemPtr, set->elemSize);
    child = (int *)((char *)dest + set->elemSize);
    child[0] = -1;
    child[1] = -1;
    return true;
}

int cmpfn(const void *key, const void *current)
{
    int k = *(int *)key;
    int c = *(int *)current;
    return k - c;
}

int main(void)
{
    sortedset s;
    SetNew(&s, sizeof(int), cmpfn);

    int root = 4;
    int left1 = 3;
    int right1 = 5;
    int l1l1 = 1;
    int l1r1 = 2;
    int r1r1 = 6;

    // the bst
    //        4
    //    3      5
    //  1   2      6

    SetAdd(&s, &root);
    printf("logicalSize : %d\n", (&s)->logicalSize);
    SetAdd(&s, &left1);
    printf("logicalSize : %d\n", (&s)->logicalSize);
    SetAdd(&s, &right1);
    printf("logicalSize : %d\n", (&s)->logicalSize);
    SetAdd(&s, &l1l1);
    printf("logicalSize : %d\n", (&s)->logicalSize);
    SetAdd(&s, &l1r1);
    printf("logicalSize : %d\n", (&s)->logicalSize);
    SetAdd(&s, &r1r1);
    printf("logicalSize : %d\n", (&s)->logicalSize);

    void *sleft1 = SetSearch(&s, &left1);
    printf("left1 :%d\n", *(int *)sleft1);

    void *sl1l1 = SetSearch(&s, &l1l1);
    printf("l1l1 :%d\n", *(int *)sl1l1);

    for (int i = 0; i < (&s)->logicalSize; i++)
    {
        int *v = (int *)((char *)((&s)->root + 1) + i * ((&s)->elemSize + 2 * sizeof(int)));
        printf("%d ", *v);
    }
}
