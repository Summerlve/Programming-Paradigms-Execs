#include<assert.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>

typedef struct {
    int *elems;
    int elemSize;
    int logicalLength;
    int allocatedLength;
    int (*cmpfn)(const void *, const void *);
} sortedset;

void SetGrow(sortedset *s)
{
    s->allocatedLength *= 2;
    s->elems = realloc(s->elems, sizeof(int) + s->allocatedLength * (s->elemSize + 2 * sizeof(int)));
    assert(s->elems != NULL);
}

void *NodePos(const sortedset *s, const int index)
{
    assert(index < s->logicalLength);
    void *node = (void *)((char *)(s->elems + 1) + index * (s->elemSize + 2 * sizeof(int)));
    return node;
}

void NodeAppend(const sortedset *s, const void *elemAddr)
{
    if (s->logicalLength == 0)
    {
        // if set is empty
        void *root = (void *)(s->elems + 1);
        memcpy(root, elemAddr, s->elemSize);
        *(int *)((char *)root + s->elemSize) = -1;
        *(int *)((char *)root + s->elemSize + sizeof(int)) = -1;
        return ;
    }

    void *lastNode = NodePos(s, s->logicalLength - 1);
    void *append = (void *)((char *)lastNode + s->elemSize + 2 * sizeof(int));

    memcpy(append, elemAddr, s->elemSize);
    *(int *)((char *)append + s->elemSize) = -1;
    *(int *)((char *)append + s->elemSize + sizeof(int)) = -1;
}

void *NodeSearch(const sortedset *s, void *node, const void *elemAddr)
{
    int cmp = s->cmpfn(elemAddr, node);

    if (cmp < 0)
    {
        // current node's left sub tree
        int index = *(int *)((char *)node + s->elemSize);
        if (index == -1) return NULL;

        void *left = NodePos(s, index);
        return NodeSearch(s, left, elemAddr);
    }

    if (cmp > 0)
    {
        // right sub tree
        int index = *(int *)((char *)node + s->elemSize + sizeof(int));
        if (index == -1) return NULL;

        void *right = NodePos(s, index);
        return NodeSearch(s, right, elemAddr);
    }

    return node;
}

bool NodeInsert(sortedset *s, void *node, const void *elemAddr)
{
    int cmp = s->cmpfn(elemAddr, node);

    if (cmp < 0)
    {
        // left
        int *indexAddr = (int *)((char *)node + s->elemSize);

        if (*indexAddr == -1)
        {
            *indexAddr = s->logicalLength;
            NodeAppend(s, elemAddr);
            return true;
        }

        void *left = NodePos(s, *indexAddr);
        return NodeInsert(s, left, elemAddr);
    }

    if (cmp > 0)
    {
        // right
        int *indexAddr = (int *)((char *)node + s->elemSize + sizeof(int));

        if (*indexAddr == -1)
        {
            *indexAddr = s->logicalLength;
            NodeAppend(s, elemAddr);
            return true;
        }

        void *right = NodePos(s, *indexAddr);
        return NodeInsert(s, right, elemAddr);
    }

    return false;
}

void SetNew(sortedset *s, int elemSize, int (*cmpfn)(const void *, const void *))
{
    s->elemSize = elemSize;
    s->cmpfn = cmpfn;
    s->logicalLength = 0;
    s->allocatedLength = 4;
    s->elems = malloc(sizeof(int) + s->allocatedLength * (s->elemSize + 2 * sizeof (int)));
    assert(s->elems != NULL);
    *s->elems = -1;
}

bool SetAdd(sortedset *s, const void *elemPtr)
{
    if (s->logicalLength == 0)
    {
        NodeAppend(s, elemPtr);
        s->logicalLength++;
        return true;
    }

    if (s->logicalLength == s->allocatedLength)
        SetGrow(s);

    void *root = NodePos(s, 0);
    bool result =  NodeInsert(s, root, elemPtr);
    if (result == true) s->logicalLength++;
    return result;
}

void *SetSearch(sortedset *s, const void *elemPtr)
{
    void *root = NodePos(s, 0);
    return NodeSearch(s, root, elemPtr);
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
    printf("logicalLength : %d\n", (&s)->logicalLength);
    SetAdd(&s, &left1);
    printf("logicalLength : %d\n", (&s)->logicalLength);
    SetAdd(&s, &right1);
    printf("logicalLength : %d\n", (&s)->logicalLength);
    SetAdd(&s, &l1l1);
    printf("logicalLength : %d\n", (&s)->logicalLength);
    SetAdd(&s, &l1r1);
    printf("logicalLength : %d\n", (&s)->logicalLength);
    SetAdd(&s, &r1r1);
    printf("logicalLength : %d\n", (&s)->logicalLength);

    void *sleft1 = SetSearch(&s, &left1);
    printf("left1 :%d\n", *(int *)sleft1);

    void *sl1l1 = SetSearch(&s, &l1l1);
    printf("l1l1 :%d\n", *(int *)sl1l1);

    printf("%d\n", (&s)->allocatedLength);

    for (int i = 0; i < (&s)->logicalLength; i++)
    {
        int *v = (int *)((char *)((&s)->elems + 1) + i * ((&s)->elemSize + 2 * sizeof(int)));
        printf("%d ", *v);
    }
}
