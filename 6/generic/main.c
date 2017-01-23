#include <stdio.h>
#include "stack.c"

void StringFree(void *elem)
{
    free(*(char **)elem);
}

int main(void)
{
    const char *friends[] = {"Al", "Bob", "Carl"};
    stack stringStack;
    StackNew(&stringStack, sizeof(char *), StringFree);

    for (int i = 0; i < 3; i++) {
        char *current = strdup(friends[i]);
        StackPush(&stringStack, &current);
    }

    char *top;
    for (int i = 0; i < 3; i++) {
        StackPop(&stringStack, &top);
        printf("%s\n", top);
        free(top);
    }

    StackDispose(&stringStack);

    int nums[] = {1, 2, 3, 4};
    stack intStack;
    StackNew(&intStack, sizeof(int), NULL);

    for (int i = 0; i < 4; i++) {
        printf("%p\n", &nums[i]);
        StackPush(&intStack, &nums[i]);
    }

    int *num;
    for (int i = 0; i < 4; i++) {
        StackPop(&intStack, num);
        printf("%d\n", *num);
    }

    StackDispose(&intStack);
}
