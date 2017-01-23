#include<stdio.h>
#include<string.h>

void *lsearch (void *key, void *base, int n, int elemSize, int (*cmpfn)(void *, void *))
{
    for (int i = 0; i < n; i++) {
        void *elemAddr = (char *)base + i * elemSize;
        if (cmpfn(key, elemAddr) == 0) return elemAddr;
    }

    return NULL;
}

int cmpfn(void *key, void* elemAddr)
{
    return (*(int *)key) - (*(int *)elemAddr);
}

int StrCmp(void * vp1, void * vp2)
{
    char *s1 = *(char **)vp1;
    char *s2 = *(char **)vp2;

    return strcmp(s1, s2);
}

int main(void)
{
/*    int array[] = { 5, 2, 3, 1, 7, 6 };*/
    /*int n = 6;*/
    /*int key = 0;*/
    /*int elemSize = sizeof(int);*/

    /*int **found = lsearch(&key, array, n, elemSize, cmpfn);*/

    /*if (found == NULL) printf("sad\n");*/
    /*else printf("%i\n", *found);*/

    /*printf("%lu\n", sizeof(int));*/

    char *nodes[] = {"Ab", "F#", "B", "Gb", "D"};
    char *fN = "F#";

    char **found = lsearch(&fN, nodes, 5, sizeof(char *), StrCmp);

    if (found == NULL) printf("sad\n");
    else printf("%s\n", *found);
}
