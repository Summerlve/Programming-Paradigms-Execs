#include <stdlib.h>
#include "stack.h"
#include <assert.h>

void StackNew(stack *s)
{
    s->logicalLength = 0;
    s->allocLength = 4;
    s->elems = malloc(4 * sizeof(int));
    assert(s->elems != NULL);
}

void StackDispose(stack *s)
{
    free(s->elems);
}

void StackPush(stack *s, int value)
{
    if (s->allocLength == s->logicalLength)
    {
        s->allocLength *= 2;
        s->elems = realloc(s->elems, s->allocLength * sizeof(int));
        assert(s->elems != NULL);
    }

    s->elems[s->logicalLength] = value;
    s->logicalLength++;
}

int StackPop(stack *s)
{
    assert(s->logicalLength > 0);
    s->logicalLength--;
    return s->elems[s->logicalLength];
}
