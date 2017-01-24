#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<stdbool.h>

void *ListAdd(void *list, char *str)
{
    int strLen = strlen(str);
    if (list == NULL)
    {
        list = malloc(sizeof(void *) + strLen + 1);
        assert(list != NULL);
        *(void **)list = NULL;
        memcpy((char *)list + sizeof(void *), str, strLen + 1);
        return list;
    }

    void *currentNode = list;
    void *previousNode = NULL;
    while(currentNode != NULL) {
        previousNode = currentNode;
        currentNode = *(void **)currentNode;
    }

    *(void **)previousNode = malloc(sizeof(void *) + strLen + 1);
    void *newNode = *(void **)previousNode;
    assert(newNode != NULL);
    *(void **)newNode = NULL;
    memcpy((char *)newNode + sizeof(void *), str, strLen + 1);

    return list;
}

bool ListDispose(void *list)
{
    if (list == NULL) return true;
    void *currentNode = list;

    void *nodeAddrs = malloc(sizeof(void *));
    int count = 0;
    assert(nodeAddrs != NULL);

    while (currentNode != NULL)
    {
        memcpy((char *)nodeAddrs + count * sizeof(void *), &currentNode, sizeof(void *));
        count++;
        nodeAddrs = realloc(nodeAddrs, (count + 1) * sizeof(void *));
        currentNode = *(void **)currentNode;
    }

    for (int i = 0; i < count; i++)
    {
        void *val = (char *)nodeAddrs + sizeof(void *) * i;
        free(*(void **)val);
    }

    return true;
}

int *serializeList(const void *list)
{
    int *count = malloc(sizeof(int));
    *count = 0;

    if (list == NULL)
        return count;

    int currentLen = sizeof(int);

    void *temp = malloc(sizeof(void *));
    memcpy(temp, &list, sizeof(void *));
    void *currentNode = *(void **)temp;

    while(currentNode != NULL)
    {
        char *val = (char *)currentNode + sizeof(void *);

        int valLen = strlen(val);
        count = realloc(count, currentLen + valLen + 1);
        assert(count != NULL);
        (*count)++;

        void *tail = (char *)count + currentLen;
        memcpy(tail, val, valLen + 1);
        currentLen += valLen + 1;
        currentNode = *(void **)currentNode;
    }

    return count;
}

int main(void)
{
    void *list = NULL;
    list = ListAdd(list, "Red");
    list = ListAdd(list, "Yellow");
    list = ListAdd(list, "Pink");
    list = ListAdd(list, "Green");
    list = ListAdd(list, "Purple");

    int *serialized = serializeList(list);
    printf("the length of list is: %d\n", *serialized);

    char *val = (char *)serialized + sizeof(int);
    int count = 0;
    int index = 0;

    while(true)
    {
        char cur = val[index];

        if (cur == '\0')
        {
            count ++;
            printf("\n");

            if (count == 5) break;
            index ++;
            continue;
        }

        printf("%c", cur);
        index ++;
    }

    ListDispose(list);

    void *emptyList = NULL;
    int *serializedEmptyList = serializeList(emptyList);
    printf("%d\n", *serializedEmptyList);
    ListDispose(emptyList);

    return 0;
}
