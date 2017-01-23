#include<stdio.h>
#include<string.h>

void rotate(void *front, void *middle, void *end)
{
    int frontSize = (char *)middle - (char *)front;
    int backSize = (char *)end - (char *)middle;

    char buffer[frontSize];
    memcpy(buffer, front, frontSize);
    memmove(front, middle, backSize);
    memcpy((char *)end - frontSize, buffer, frontSize);
}


int main(void)
{
    int nums[] = {1, 2, 3, 4, 5};
    rotate(&nums[0], &nums[2], &nums[4] + 1);

    for(int i = 0; i < 5; i++)
    {
        printf("%d\n", nums[i]);
    }

    char *strings[] = {"fuck", "ri", "wocao"};
    rotate(&strings[0], &strings[1], &strings[2] + 1);

    for (int i = 0; i < 3; i++)
    {
        printf("%s\n", strings[i]);
    }
}
