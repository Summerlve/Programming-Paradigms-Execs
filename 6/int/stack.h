typedef struct {
    int *elems;
    int logicalLength;
    int allocLength;
} stack;

void StackNew(stack *s);
void StackDispose(stack *s);
void StackPush(stack *s, int value);
int StackPop(stack *s);
