#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<stdbool.h>

void *ListNew()
{
    void *list = malloc(sizeof(void *));
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
        memcpy(nodeAddrs, &currentNode, sizeof(void *));
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

void ListAdd(void *list, char *str)
{
    int strLen = strlen(str);
    if (list == NULL)
    {
        list = malloc(sizeof(void *) + strLen + 1);
        assert(list != NULL);
        *(char **)list = NULL;
        memcpy((char *)list + sizeof(void *), str, strLen + 1);
        return ;
    }

    void *currentNode = list;
    while(currentNode != NULL)
        currentNode = *(void **)currentNode;

    currentNode = malloc(sizeof(void *) + strLen + 1);
    assert(currentNode !=NULL);
    *(void **)currentNode = NULL;
    memcpy((char *)currentNode + sizeof(void *), str, strLen + 1);
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
        char *val = (char *)currentNode + sizeof(int);

        int valLen = strlen(val);
        count = realloc(count, currentLen + valLen + 1);
        assert(count != NULL);

        void *tail = (char *)count + currentLen;
        memcpy(tail, val, valLen + 1);
        currentLen += valLen + 1;
        currentNode = *(void **)currentNode;
    }

    return count;
}

int main(void)
{
    void *list = ListNew();
    ListAdd(list, "Red");
    ListAdd(list, "Yellow");
    ListAdd(list, "Pink");
    ListAdd(list, "Green");
    ListAdd(list, "Purple");

    int *serialized = serializeList(list);
    printf("the length of list is: %d\n", *serialized);
    return 0;
}

