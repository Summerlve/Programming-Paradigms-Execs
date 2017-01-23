#include "stack.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void StackNew(stack *s, int elemSize, void (*freefn)(void *))
{
    assert(elemSize > 0);
    s->logicalLength = 0;
    s->allocLength = 4;
    s->elemSize = elemSize;
    s->elems = malloc(s->allocLength * s->elemSize);
    s->freefn = freefn;
    assert(s->elems != NULL);
}

void StackDispose(stack *s)
{
    if (s->freefn != NULL)
    {
        for (int i = 0; i < s->logicalLength; i++)
        {
            s->freefn((char *)s->elems + i * s->elemSize);
        }
    }

    free(s->elems);
}

static void StackGrow(stack *s)
{
    s->allocLength *= 2;
    s->elems = realloc(s->elems, s->allocLength * s->elemSize);
    assert(s->elems != NULL);
}

void StackPush(stack *s, void *elemAddr)
{
    if (s->logicalLength == s->allocLength)
    {
        StackGrow(s);
    }

    void *target = (char *)s->elems + s->logicalLength * s->elemSize;
    memcpy(target, elemAddr, s->elemSize);
    s->logicalLength++;
}

void StackPop(stack *s, void *elemAddr)
{
    assert(s->logicalLength > 0);
    s->logicalLength--;
    void *source = (char *)s->elems + s->logicalLength * s->elemSize;
    memcpy(elemAddr, source, s->elemSize);
}
