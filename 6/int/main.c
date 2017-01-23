#include <stdio.h>
#include "stack.c"

int main(void)
{
    stack s;
    StackNew(&s);
    StackPush(&s, 1);
    StackPush(&s, 2);

    printf("%d\n", StackPop(&s));
    printf("%d\n", StackPop(&s));

    StackDispose(&s);
}
