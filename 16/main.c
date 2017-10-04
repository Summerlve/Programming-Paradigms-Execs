#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *saySomething(void *arg)
{
    printf("%s\n", (char *)arg);
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t sayHello;
    pthread_create(&sayHello, NULL, saySomething, "Hello");
    printf("main thread 1\n");
    pthread_join(sayHello, NULL);
    printf("main thread 2\n");
    return 0;
}
