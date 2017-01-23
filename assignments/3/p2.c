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

}

void ListAdd()

