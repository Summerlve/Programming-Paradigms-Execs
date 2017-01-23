#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef enum {
    Integer, String, List, Nil
} nodeType;

char *ConcatStrings(const char *fir, const char *sec)
{
    int size = strlen(fir) + strlen(sec) + 1;
    char *result = malloc(size);
    memcpy(result, fir, strlen(fir));
    memcpy(result + strlen(fir), sec, strlen(sec) + 1);
    return result;
}

char *ConcatAll(nodeType *list)
{
    switch (*list)
    {
        case Integer:
        case Nil: return strdup("");
        case String:
            return strdup((char *)(list + 1));
    }

    nodeType **lists = (nodeType **)(list + 1);
    char *front = ConcatAll(lists[0]);
    char *back = ConcatAll(lists[1]);
    char *result  = ConcatStrings(front, back);
    free(front);
    free(back);
    return result;
}

int main(void)
{
}
