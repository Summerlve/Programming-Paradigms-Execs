typedef struct {
    void *elems;
    int elemSize;
    int logicalLength;
    int allocLength;
    void (*freefn)(void *);
} stack;

void StackNew(stack *s, int elemSize, void (*freefn)(void *));
void StackDispose(stack *s);
void StackPush(stack *s, void *elemAddr);
void StackPop(stack *s, void *elemAddr);
static void StackGrow(stack *s);
